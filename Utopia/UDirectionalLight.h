#pragma once
#include "ULight.h"

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
    class LIB_API UDirectionalLight : public ULight
    {
    public:
		UDirectionalLight(const std::string& name, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, int lightID);
        virtual ~UDirectionalLight() noexcept;
        void render() override;
    };
}