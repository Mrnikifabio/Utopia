#pragma once

#include "UObject.h"
#include <iostream>
#include "glm/glm.hpp"

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
        void render() override;
        virtual ~UMaterial() noexcept;
        UMaterial(const std::string& name);
        UMaterial(const std::string& name, glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, int shininess);
    };
}

