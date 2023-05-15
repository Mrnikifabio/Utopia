#include <UHands.h>
#include <ULeap.h>
#include <GL/glew.h>
#include <UMesh.h>
#include <Utopia.h>
#include <UCamera.h>

using namespace utopia;




struct UHands::pimpl {
    std::unique_ptr<ULeap> m_leap;
    bool m_initiated;
    std::array<std::shared_ptr<UMesh>, 2> m_hands;
    pimpl() :       m_leap{ std::unique_ptr<ULeap>(new ULeap()) },
        m_initiated{false},
        m_hands{ std::shared_ptr<UMesh>(new UMesh("hand_0")), std::shared_ptr<UMesh>(new UMesh("hand_1")) }
    {}
};

bool UHands::init() {
    GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles        
    GLfloat radius = 0.1f;
    int gradation = 3;
    
    auto lod = std::shared_ptr<UMesh::LOD>(new UMesh::LOD());
    int vertNumber = 0;
    for (alpha = 0.0; alpha < glm::pi<float>(); alpha += glm::pi<float>() / gradation)
        for (beta = 0.0f; beta < 2.01f * glm::pi<float>(); beta += glm::pi<float>() / gradation)
        {
            x = radius * cos(beta) * sin(alpha);
            y = radius * sin(beta) * sin(alpha);
            z = radius * cos(alpha);
            lod->vertices.push_back(UMesh::Vertex(glm::vec3(x, y, z), glm::vec3(1.0f), glm::vec2(1.0f), glm::vec4(1.0f)));
            
            if (vertNumber >= 2) {
                UMesh::Face face;
                face.verticesId[0] = vertNumber - 2;
                face.verticesId[1] = vertNumber - 1;
                face.verticesId[2] = vertNumber;
                lod->faces.push_back(face);
            }
            vertNumber++;
            x = radius * cos(beta) * sin(alpha + glm::pi<float>() / gradation);
            y = radius * sin(beta) * sin(alpha + glm::pi<float>() / gradation);
            z = radius * cos(alpha + glm::pi<float>() / gradation);
            lod->vertices.push_back(UMesh::Vertex(glm::vec3(x, y, z), glm::vec3(1.0f), glm::vec2(1.0f), glm::vec4(1.0f)));
            
            if (vertNumber >= 2) {
                UMesh::Face face;
                face.verticesId[0] = vertNumber - 2;
                face.verticesId[1] = vertNumber - 1;
                face.verticesId[2] = vertNumber;
                lod->faces.push_back(face);
            }
            vertNumber++;
        }
    lod->nOfvertices = vertNumber;

    for (unsigned int h = 0; h < 2; h++)
    {
        auto hs = std::to_string(h);
        
        // Elbow:
        auto elbow = std::shared_ptr<UMesh>(new UMesh("elbow_sphere_" + hs));
        elbow->pushLOD(lod);
        m_pimpl->m_hands[h]->addChild(elbow);

        // Wrist:
        auto wirst = std::shared_ptr<UMesh>(new UMesh("wirst_sphere_" + hs));
        wirst->pushLOD(lod);
        elbow->addChild(wirst);

        // Palm:
        auto palm = std::shared_ptr<UMesh>(new UMesh("palm_sphere_" + hs));
        palm->pushLOD(lod);
        wirst->addChild(palm);

        // Distal ends of bones for each digit:
        for (unsigned int d = 0; d < 5; d++)
        {
            auto fingerJoint = palm;
            std::shared_ptr<UMesh> newFingerJoint;
            for (unsigned int b = 0; b < 4; b++)
            {
                newFingerJoint = std::shared_ptr<UMesh>(new UMesh("finger_joint_" + std::to_string(b) + "_" + std::to_string(d) + "_" + hs));
                newFingerJoint->pushLOD(lod);
                fingerJoint->addChild(newFingerJoint);
                fingerJoint = newFingerJoint;
            }
        }

        //this->addChild(m_pimpl->m_hands[h]);
    }

    m_pimpl->m_initiated = m_pimpl->m_leap->init();
    if (!m_pimpl->m_initiated)
        std::cout << "[ERROR] Unable to init Leap Motion" << std::endl;
    return m_pimpl->m_initiated;
}

void UHands::update() {
    if (!m_pimpl->m_initiated)
        return;
    // Update Leap Motion status:
    m_pimpl->m_leap->update();
    const LEAP_TRACKING_EVENT* l = m_pimpl->m_leap->getCurFrame();
    for (int h = this->getChildCount()-1; h >= 0; h--) //clear hands
        this->detachChild(h);
    // Render hands using spheres:
    for (unsigned int h = 0; h < l->nHands; h++)
    {
        LEAP_HAND hand = l->pHands[h];
        this->addChild(m_pimpl->m_hands[hand.type]);
        glm::mat4 camMV = UCamera::getMainCamera().lock()->getModelView();
        glm::mat4 camProjMat = UCamera::getMainCamera().lock()->getCameraMatrix();
        glm::vec3 camPos = glm::vec3(camMV[3][0], camMV[3][1], camMV[3][2]);
        camMV[3][0] = 0.0f;
        camMV[3][1] = 0.0f;
        camMV[3][2] = 0.0f;
        glm::vec3 camDirX = glm::normalize(glm::mat3(camMV) * glm::vec3(camProjMat[0][0], camProjMat[1][0], camProjMat[2][0]));
        glm::vec3 camDirY = glm::normalize(glm::mat3(camMV) * glm::vec3(camProjMat[0][1], camProjMat[1][1], camProjMat[2][1]));
        glm::vec3 camDirZ = glm::normalize(glm::mat3(camMV) * glm::vec3(camProjMat[0][2], camProjMat[1][2], camProjMat[2][2]));
        this->setModelView(glm::translate(glm::mat4(1.0f), camPos - camDirY + 1.5f * camDirZ));
        // Elbow:
        glm::mat4 c = glm::translate(camMV, glm::vec3(hand.arm.prev_joint.x, hand.arm.prev_joint.y, hand.arm.prev_joint.z) / 1000.0f);
        auto elbow = m_pimpl->m_hands[hand.type]->getChild(0);
        elbow.lock()->setModelView(c);

        // Wrist:
        c = glm::translate(glm::mat4(1.0f), glm::vec3(hand.arm.next_joint.x, hand.arm.next_joint.y, hand.arm.next_joint.z) / 1000.0f);
        auto wirstNode = elbow.lock()->getChild(0);
        wirstNode.lock()->setModelView(c);

        // Palm:
        c = glm::translate(glm::mat4(1.0f), glm::vec3(hand.palm.position.x, hand.palm.position.y, hand.palm.position.z) / 1000.0f);
        auto palmNode = wirstNode.lock()->getChild(0);
        palmNode.lock()->setModelView(c);

        // Distal ends of bones for each digit:
        for (unsigned int d = 0; d < 5; d++)
        {
            LEAP_DIGIT finger = hand.digits[d];
            auto fingerNode = palmNode;
            for (unsigned int b = 0; b < 4; b++)
            {
                if(b == 0)
                    fingerNode = fingerNode.lock()->getChild(d); // choose the finger
                else
                    fingerNode = fingerNode.lock()->getChild(0); // choose the joint
                LEAP_BONE bone = finger.bones[b];
                c = glm::translate(glm::mat4(1.0f), glm::vec3(bone.next_joint.x, bone.next_joint.y, bone.next_joint.z) / 1000.0f);
                fingerNode.lock()->setModelView(c);
            }
        }
    }
    //UNode::render();
}

UHands::UHands() : UNode{ "hands" }, m_pimpl{new pimpl()}
{
}

UHands::~UHands() {
    m_pimpl->m_leap->free();
};
