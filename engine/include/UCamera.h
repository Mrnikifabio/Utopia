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
    class UCamera : public UNode
    {
    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;
        static std::weak_ptr<UCamera> m_mainCamera;

    public:
        LIB_API UCamera(const std::string& name);
        LIB_API virtual ~UCamera() noexcept;

        LIB_API virtual const glm::mat4 getCameraMatrix() const = 0;

        LIB_API float getFar() const;
        LIB_API float getNear() const;
        LIB_API void setFar(const float far);
        LIB_API void setNear(const float near);

        LIB_API static void setMainCamera(std::weak_ptr<UCamera> camera);
        LIB_API static std::weak_ptr<UCamera>& getMainCamera();
    };
}
