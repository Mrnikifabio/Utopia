#pragma once
#include "UNode.h"
#include <memory>

#ifdef _WINDOWS 	
// Export API:
// Specifies i/o linkage (VC++ spec):
#ifdef UTOPIA_EXPORTS
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif      	
#else // Under Linux
#define LIB_API  // Dummy declaration
#endif

namespace utopia
{
    class LIB_API UCamera : public UNode
    {
    private:
        static std::weak_ptr<UCamera> m_mainCamera;
    public:
        UCamera(const std::string& name);
        virtual ~UCamera() noexcept;
        void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
        static void setMainCamera(std::weak_ptr<UCamera> camera);
        static std::weak_ptr<UCamera>& getMainCamera();
    };
}
