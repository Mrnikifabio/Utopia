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
    class LIB_API UTexture : public UObject
    {

    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;

    public:
        void render() override;
        virtual ~UTexture() noexcept;
        UTexture(const std::string& name, unsigned int texId);
    };
}

