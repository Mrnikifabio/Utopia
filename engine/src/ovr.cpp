#include "ovr.h"
#include <GL/glew.h>
#include <GL/freeglut.h>   
#include <openvr.h>
#include <iostream>
#include <vector>  

struct utopia::OvVR::pimpl {
    // OVR objects:	
    vr::IVRSystem* vrSys;
    vr::IVRRenderModels* vrModels;
    vr::IVRCompositor* vrComp;
    vr::TrackedDevicePose_t vrPoses[vr::k_unMaxTrackedDeviceCount];
    std::vector<Controller*> controllers;

    pimpl() : vrSys(nullptr), vrModels(nullptr), vrComp(nullptr) 
    {
        controllers.clear();
    }
};

struct utopia::OvVR::Controller {
        unsigned int id;
        vr::VRControllerState_t pControllerState;
        glm::mat4 matrix;

        // Const/dest:
        Controller(const unsigned int c) : id(c), matrix(glm::mat4(1.0f)) {}
        Controller() : id(0), matrix(glm::mat4(1.0f)) {}
        ~Controller() {}

        unsigned int getID()
        {
            return id;
        }

        /**
         * Get controller's position matrix.
         * @param id controller id
         * @return model matrix ready for OpenGL
         */
        glm::mat4 getMatrix()
        {
            return matrix;
        }

        void setMatrix(const glm::mat4& m)
        {
            matrix = m;
        }


        /**
         * Get controller's axis position.
         * @param axis axis id, between 0 and 4
         * @param bPressed inout, button pressed (use masks to determine which ones)
         * @param bTouched inout, button touched (use masks to determine which ones)
         * @return axis values
         */
        glm::vec2 getAxis(unsigned int axisId, unsigned long long int& bPressed, unsigned long long int& bTouched)
        {
            if (axisId >= vr::k_unControllerStateAxisCount)
                return glm::vec2(0.0f);

            glm::vec2 res;
            res.x = pControllerState.rAxis[axisId].x;
            res.y = pControllerState.rAxis[axisId].y;
            bPressed = pControllerState.ulButtonPressed;
            bTouched = pControllerState.ulButtonTouched;

            // Done:
            return res;
        }


        /**
         * Returns true when the Nth button is enabled (given the input mask).
         * @param id button id
         * @param mask button mask
         * @return true when pressed, false otherwise
         */
        bool isButtonPressed(unsigned long long int id, unsigned long long int mask)
        {
            uint64_t buttonMask = 1ull << id;
            if (mask & buttonMask)
                return true;
            else
                return false;
        }
};

utopia::OvVR::OvVR(const std::string& name) : m_pimpl { std::unique_ptr<utopia::OvVR::pimpl>(new utopia::OvVR::pimpl()) }, UObject(name) {}

bool utopia::OvVR::init()
{
    vr::EVRInitError error = vr::VRInitError_None;

    // Init VR system:
    std::cout << "Using OpenVR " << vr::k_nSteamVRVersionMajor << "." << vr::k_nSteamVRVersionMinor << "." << vr::k_nSteamVRVersionBuild << std::endl;
    m_pimpl->vrSys = vr::VR_Init(&error, vr::VRApplication_Scene);
    if (error != vr::VRInitError_None)
    {
        m_pimpl->vrSys = nullptr;
        std::cout << "[ERROR] Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(error) << std::endl;
        return false;
    }

    // Init render models:
    m_pimpl->vrModels = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &error);
    if (m_pimpl->vrModels == nullptr)
    {
        m_pimpl->vrSys = nullptr;
        vr::VR_Shutdown();
        std::cout << "[ERROR] Unable to get render model interface: " << vr::VR_GetVRInitErrorAsEnglishDescription(error) << std::endl;
        return false;
    }

    // Initialize the compositor:
    m_pimpl->vrComp = vr::VRCompositor();
    if (!m_pimpl->vrComp)
    {
        m_pimpl->vrModels = nullptr;
        m_pimpl->vrSys = nullptr;
        vr::VR_Shutdown();
        std::cout << "[ERROR] Unable to get VR compositor" << std::endl;
        return false;
    }

    // Tweaks:        
    //vrComp->ShowMirrorWindow();        

    // Init controller tracking:
    m_pimpl->controllers.clear();
    for (unsigned int c = 0; c < vr::k_unMaxTrackedDeviceCount; c++)
    {
        if (m_pimpl->vrSys->GetTrackedDeviceClass(c) == vr::TrackedDeviceClass_Controller)
        {
            std::cout << "   Found controller at " << c << std::endl;
            Controller* cont = new Controller(c);

            unsigned int bufferLen = m_pimpl->vrSys->GetStringTrackedDeviceProperty(c, vr::Prop_RenderModelName_String, nullptr, 0, nullptr);
            if (bufferLen == 0)
            {
                std::cout << "[ERROR] Unable to get controller render model" << std::endl;
                delete cont;
                continue;
            }

            std::string result;
            result.resize(bufferLen);
            m_pimpl->vrSys->GetStringTrackedDeviceProperty(c, vr::Prop_RenderModelName_String, &result[0], bufferLen, nullptr);
            std::cout << "   Controller render model: '" << result << "'" << std::endl;
            m_pimpl->controllers.push_back(cont);
        }
    }

    // Done:
    return true;

}

bool utopia::OvVR::free()
{
    for (unsigned int c = 0; c < m_pimpl->controllers.size(); c++)
        delete m_pimpl->controllers[c];
    m_pimpl->controllers.clear();

    vr::VR_Shutdown();
    m_pimpl->vrComp = nullptr;
    m_pimpl->vrModels = nullptr;
    m_pimpl->vrSys = nullptr;

    // Done:      
    return true;
}

std::string utopia::OvVR::getTrackingSysName()
{
    unsigned int bufferLen = m_pimpl->vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, nullptr, 0, nullptr);
    if (bufferLen == 0)
        return std::string();

    std::string result;
    result.resize(bufferLen);
    m_pimpl->vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, &result[0], bufferLen, nullptr);
    return result;
}

bool utopia::OvVR::printRenderModels()
{
    for (unsigned int c = 0; c < m_pimpl->vrModels->GetRenderModelCount(); c++)
    {
        char buffer[256];
        m_pimpl->vrModels->GetRenderModelName(c, buffer, 256);
        std::cout << "   " << c << ") " << buffer << " model" << std::endl;

        for (unsigned int d = 0; d < m_pimpl->vrModels->GetComponentCount(buffer); d++)
        {
            char cbuffer[256];
            m_pimpl->vrModels->GetComponentName(buffer, d, cbuffer, 256);
            std::cout << "     " << d << ") " << cbuffer << std::endl;
        }
    }

    // Done:
    return true;
}

std::string utopia::OvVR::getManufacturerName()
{
    unsigned int bufferLen = m_pimpl->vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ManufacturerName_String, nullptr, 0, nullptr);
    if (bufferLen == 0)
        return std::string();

    std::string result;
    result.resize(bufferLen);
    m_pimpl->vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ManufacturerName_String, &result[0], bufferLen, nullptr);
    return result;
}

std::string utopia::OvVR::getModelNumber()
{
    unsigned int bufferLen = m_pimpl->vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ModelNumber_String, nullptr, 0, nullptr);
    if (bufferLen == 0)
        return std::string();

    std::string result;
    result.resize(bufferLen);
    m_pimpl->vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ModelNumber_String, &result[0], bufferLen, nullptr);
    return result;
}

unsigned int utopia::OvVR::getHmdIdealHorizRes()
{
    unsigned int result, dummy;
    m_pimpl->vrSys->GetRecommendedRenderTargetSize(&result, &dummy);
    return result;
}

unsigned int utopia::OvVR::getHmdIdealVertRes()
{
    unsigned int result, dummy;
    m_pimpl->vrSys->GetRecommendedRenderTargetSize(&dummy, &result);
    return result;
}

glm::mat4 ovr2ogl(const vr::HmdMatrix34_t& matrix)
{
    return glm::mat4(matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], 0.0f,
        matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], 0.0f,
        matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], 0.0f,
        matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], 1.0f);
}

glm::mat4 ovr2ogl(const vr::HmdMatrix44_t& matrix)
{
    return glm::mat4(matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], matrix.m[3][0],
        matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], matrix.m[3][1],
        matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], matrix.m[3][2],
        matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], matrix.m[3][3]);
}

bool utopia::OvVR::update()
{
    // Main update method:
    m_pimpl->vrComp->WaitGetPoses(m_pimpl->vrPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

    // If used, update controllers:
    for (auto c : m_pimpl->controllers)
    {
        if (m_pimpl->vrPoses[c->getID()].bPoseIsValid)
            c->setMatrix(ovr2ogl(m_pimpl->vrPoses[c->getID()].mDeviceToAbsoluteTracking));
    }

    // Done:
    return true;
}

glm::mat4 utopia::OvVR::getProjMatrix(OvEye eye, float nearPlane, float farPlane)
{
    switch (eye)
    {
    case EYE_LEFT: return ovr2ogl(m_pimpl->vrSys->GetProjectionMatrix(vr::Eye_Left, nearPlane, farPlane)); break;
    case EYE_RIGHT: return ovr2ogl(m_pimpl->vrSys->GetProjectionMatrix(vr::Eye_Right, nearPlane, farPlane)); break;
    default: return glm::mat4(1.0f);
    }
}

glm::mat4 utopia::OvVR::getEye2HeadMatrix(OvEye eye)
{
    switch (eye)
    {
    case EYE_LEFT: return ovr2ogl(m_pimpl->vrSys->GetEyeToHeadTransform(vr::Eye_Left)); break;
    case EYE_RIGHT: return ovr2ogl(m_pimpl->vrSys->GetEyeToHeadTransform(vr::Eye_Right)); break;
    default: return glm::mat4(1.0f);
    }
}

glm::mat4 utopia::OvVR::getModelviewMatrix()
{
    if (m_pimpl->vrPoses[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid == false)
        return glm::mat4(1.0f);

    glm::mat4 headPos = ovr2ogl(m_pimpl->vrPoses[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
    return headPos;
}

unsigned int utopia::OvVR::getNrOfControllers()
{
    return (unsigned int)m_pimpl->controllers.size();
}

utopia::OvVR::Controller* utopia::OvVR::getController(unsigned int pos) const
{
    if (pos >= m_pimpl->controllers.size())
        return nullptr;
    return m_pimpl->controllers.at(pos);
}

void utopia::OvVR::setReprojection(bool flag)
{
    m_pimpl->vrComp->ForceInterleavedReprojectionOn(flag);
}

void utopia::OvVR::pass(OvEye eye, unsigned int eyeTexture)
{
    const vr::Texture_t t = { reinterpret_cast<void*>(uintptr_t(eyeTexture)), vr::TextureType_OpenGL, vr::ColorSpace_Linear };
    switch (eye)
    {
    case EYE_LEFT:  m_pimpl->vrComp->Submit(vr::Eye_Left, &t); break;
    case EYE_RIGHT: m_pimpl->vrComp->Submit(vr::Eye_Right, &t); break;
    }
}

void utopia::OvVR::render()
{
    m_pimpl->vrComp->PostPresentHandoff();
}
