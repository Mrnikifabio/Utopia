#include <UHands.h>
#include <ULeap.h>
#include <GL/glew.h>
#include <UMesh.h>
#include <Utopia.h>
#include <UCamera.h>
#include <OVOFactory.h>

using namespace utopia;




struct UHands::pimpl {
    std::unique_ptr<ULeap> m_leap;
    bool m_initiated;
    std::array<std::shared_ptr<UNode>, 2> m_hands;
    float m_leapDownScaleFactor;
    //in units
    float m_handsXDistance;
    float m_handsYDistance;
    float m_handsZDistance;
    pimpl() :       m_leap{ std::unique_ptr<ULeap>(new ULeap()) },
        m_initiated{false},
        m_hands{ std::shared_ptr<UMesh>(new UMesh("hand_0")), std::shared_ptr<UMesh>(new UMesh("hand_1")) },
        m_leapDownScaleFactor{ 0.003f},
        m_handsXDistance{0.0f},
        m_handsYDistance{-1.0f},
        m_handsZDistance{1.5f}
    {}
};

std::shared_ptr<UNode> getSphere() {
    return OVOFactory::getInstance().fromFile("sphere.ovo")->detachChild(0);
}
    
bool UHands::init() {
    for (unsigned int h = 0; h < 2; h++)
    {
        auto hs = std::to_string(h);

        // Elbow:
        auto elbow = getSphere();
        m_pimpl->m_hands[h]->addChild(elbow);

        // Wrist:
        auto wirst = getSphere();
        elbow->addChild(wirst);

        // Palm:
        auto palm = getSphere();
        wirst->addChild(palm);

        // Distal ends of bones for each digit:
        for (unsigned int d = 0; d < 5; d++)
        {
            auto fingerJoint = palm;
            std::shared_ptr<UNode> newFingerJoint;
            for (unsigned int b = 0; b < 4; b++)
            {
                newFingerJoint = getSphere();
                fingerJoint->addChild(newFingerJoint);
                fingerJoint = newFingerJoint;
            }
        }
    }

    //fingers begin from the thumb
    /*m_pimpl->m_hands[eLeapHandType::eLeapHandType_Left] = OVOFactory::getInstance().fromFile("hand.ovo");

    m_pimpl->m_hands[eLeapHandType::eLeapHandType_Right] = OVOFactory::getInstance().fromFile("hand.ovo");

    if (m_pimpl->m_hands[eLeapHandType::eLeapHandType_Left] == nullptr || m_pimpl->m_hands[eLeapHandType::eLeapHandType_Right] == nullptr)
        return false;

    m_pimpl->m_hands[eLeapHandType::eLeapHandType_Left]->setName("handLeft");
    m_pimpl->m_hands[eLeapHandType::eLeapHandType_Right]->setName("handRight");*/

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

    float mmToM = 1.0f / m_pimpl->m_leapDownScaleFactor;
    // Render hands using spheres:
    for (unsigned int h = 0; h < l->nHands; h++)
    {
        LEAP_HAND hand = l->pHands[h];
        this->addChild(m_pimpl->m_hands[hand.type]);

        glm::mat4 camMV = UCamera::getMainCamera().lock()->getFinalWorldCoordinates();
        glm::mat4 camProjMat = UCamera::getMainCamera().lock()->getCameraMatrix();
        glm::vec3 camPos = glm::vec3(camMV[3][0], camMV[3][1], camMV[3][2]);
        camMV[3][0] = 0.0f;
        camMV[3][1] = 0.0f;
        camMV[3][2] = 0.0f;
        glm::vec3 camDirX = glm::normalize(glm::mat3(camMV) * glm::vec3(camProjMat[0][0], camProjMat[1][0], camProjMat[2][0]));
        glm::vec3 camDirY = glm::normalize(glm::mat3(camMV) * glm::vec3(camProjMat[0][1], camProjMat[1][1], camProjMat[2][1]));
        glm::vec3 camDirZ = glm::normalize(glm::mat3(camMV) * glm::vec3(camProjMat[0][2], camProjMat[1][2], camProjMat[2][2]));
        this->setModelView(glm::translate(glm::mat4(1.0f), camPos + m_pimpl->m_handsXDistance * camDirX + m_pimpl->m_handsYDistance * camDirY + m_pimpl->m_handsZDistance * camDirZ));

        // Elbow:
        glm::vec3 pos = glm::vec3(hand.arm.prev_joint.x, hand.arm.prev_joint.y, hand.arm.prev_joint.z) / mmToM;
        glm::mat4 c = glm::translate(camMV, pos);
        auto elbow = m_pimpl->m_hands[hand.type]->getChild(0);
        elbow.lock()->setModelView(c);

        
        // Wrist:
        glm::vec3 oldPos = pos;
        pos = glm::vec3(hand.arm.next_joint.x, hand.arm.next_joint.y, hand.arm.next_joint.z) / mmToM;
        c = glm::translate(glm::mat4(1.0f), pos - oldPos);
        auto wirstNode = elbow.lock()->getChild(0);
        wirstNode.lock()->setModelView(c);

        // Palm:
        oldPos = pos;
        pos = glm::vec3(hand.palm.position.x, hand.palm.position.y, hand.palm.position.z) / mmToM;
        c = glm::translate(glm::mat4(1.0f), pos - oldPos);
        auto palmNode = wirstNode.lock()->getChild(0);
        palmNode.lock()->setModelView(c);
        glm::vec3 palmPos = pos;

        // Distal ends of bones for each digit:
        for (unsigned int d = 0; d < 5; d++)
        {
            LEAP_DIGIT finger = hand.digits[d];
            auto fingerNode = palmNode;
            for (unsigned int b = 0; b < 4; b++)
            {
                if (b == 0) {
                    fingerNode = fingerNode.lock()->getChild(d); // choose the finger
                    oldPos = palmPos;
                }
                else{
                    fingerNode = fingerNode.lock()->getChild(0); // choose the joint
                    oldPos = pos;
                }
                LEAP_BONE bone = finger.bones[b];
                pos = glm::vec3(bone.next_joint.x, bone.next_joint.y, bone.next_joint.z) / mmToM;
                c = glm::translate(glm::mat4(1.0f), pos - oldPos);
                fingerNode.lock()->setModelView(c);
            }
        }
    }
    /*
    float mmToM = 10.0f;

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
        this->setModelView(glm::translate(camMV, camPos - camDirY + 2.0f * camDirZ));

        // Elbow:
        glm::mat4 c = glm::translate(glm::mat4(1.0f), glm::vec3(hand.arm.prev_joint.x, hand.arm.prev_joint.y, hand.arm.prev_joint.z) / 1000.0f);

        // Wrist:
        c = glm::translate(c, glm::vec3(hand.arm.next_joint.x, hand.arm.next_joint.y, hand.arm.next_joint.z) / 1000.0f);

        

        m_pimpl->m_hands[hand.type]->setModelView(c);
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
        // Palm:
        glm::vec3 palmPos = glm::vec3(hand.palm.position.x, hand.palm.position.y, hand.palm.position.z) / mmToM;
        c = glm::translate(glm::mat4(1.0f), palmPos);
        auto palmNode = m_pimpl->m_hands[hand.type]->getChild(0);
        //palmNode.lock()->setModelView(scale * c);

        
        glm::vec3 v1 = glm::vec3(hand.digits[0].bones[0].next_joint.x, hand.digits[0].bones[0].next_joint.y, hand.digits[0].bones[0].next_joint.z) / mmToM;
        glm::vec3 v2 = glm::vec3(hand.digits[2].bones[0].next_joint.x, hand.digits[2].bones[0].next_joint.y, hand.digits[2].bones[0].next_joint.z) / mmToM;
        glm::vec3 up = -glm::normalize(glm::cross(glm::normalize(v1), glm::normalize(v2)));
        glm::vec3 direction = glm::normalize(v2-palmPos);
        glm::mat4 pm = glm::lookAt(palmPos, direction, up);
        palmNode.lock()->setModelView(scale * pm);

        // Distal ends of bones for each digit:
        for (unsigned int d = 0; d < 5; d++)
        {
            LEAP_DIGIT finger = hand.digits[d];
            auto fingerNode = palmNode;
            for (unsigned int b = 0; b < 3; b++)
            {
                if (b == 0)
                    fingerNode = fingerNode.lock()->getChild(d); // choose the finger
                else
                    fingerNode = fingerNode.lock()->getChild(0); // choose the joint
                LEAP_BONE bone = finger.bones[b];
                glm::vec3 jointPos = glm::vec3(bone.next_joint.x, bone.next_joint.y, bone.next_joint.z) / mmToM;
                c = glm::translate(glm::mat4(1.0f), jointPos);
                fingerNode.lock()->setModelView(c);
            }
        }
    }*/
}

void utopia::UHands::setXDistanceFromCam(float units)
{
    m_pimpl->m_handsXDistance = units;
}

void utopia::UHands::setYDistanceFromCam(float units)
{
    m_pimpl->m_handsYDistance = units;
}

void utopia::UHands::setZDistanceFromCam(float units)
{
    m_pimpl->m_handsZDistance = units;
}

void utopia::UHands::setDownScaleFactorForLeap(float factor)
{
    m_pimpl->m_leapDownScaleFactor = factor;
}

UHands::UHands() : UNode{ "hands" }, m_pimpl{new pimpl()}
{
}

UHands::~UHands() {
    m_pimpl->m_leap->free();
};
