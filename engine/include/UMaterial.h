#pragma once

#include "UObject.h"
#include <iostream>
#include "glm/glm.hpp"
#include "UTexture.h"

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
    class UMaterial : public UObject
    {

    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;
        static std::shared_ptr<UMaterial> m_defaultMaterial;

    public:
        LIB_API void disable();
        void render() override;
        LIB_API virtual ~UMaterial() noexcept;
        LIB_API UMaterial(const std::string& name);
        LIB_API UMaterial(const std::string& name, const glm::vec4& emission, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, int shininess);
        LIB_API void setEmission(const glm::vec4& emission);
        LIB_API void setAmbient(const glm::vec4& ambient);
        LIB_API void setDiffuse(const glm::vec4& diffuse);
        LIB_API void setSpecular(const glm::vec4& specular);
        LIB_API void setShininess(int shininess);
        LIB_API void setTexture(std::shared_ptr<UTexture> texture);
        LIB_API static const std::shared_ptr<UMaterial> getDefaultMaterial();
    };
}

