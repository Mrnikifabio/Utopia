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
    class LIB_API UMaterial : public UObject
    {

    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;

    public:
        void disable();
        void render() override;
        virtual ~UMaterial() noexcept;
        UMaterial(const std::string& name);
        UMaterial(const std::string& name, const glm::vec4& emission, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, int shininess);
        void setEmission(const glm::vec4& emission);
        void setAmbient(const glm::vec4& ambient);
        void setDiffuse(const glm::vec4& diffuse);
        void setSpecular(const glm::vec4& specular);
        void setShininess(int shininess);
        void setTexture(std::shared_ptr<UTexture> texture);
        
    
    };
}

