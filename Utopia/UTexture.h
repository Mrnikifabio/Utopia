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
    class UTextureFactory;

    class UTexture : public UObject
    {
    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;
        static int getMaxAnisotropicLevel();
        UTexture(const std::string& name, unsigned int texId);
        friend UTextureFactory;

    public:
        LIB_API virtual ~UTexture() noexcept;

        void render() override;

        LIB_API static void enableNearestFilter();
        LIB_API static void enableNearestBipmapNearestFilter();
        LIB_API static void enableLinearFilter();
        LIB_API static void enableLinearBipmapNearestFilter();
        LIB_API static void enableLinearBipmapLinearFilter();


        LIB_API static void enableTexturesRepeat();
        LIB_API static void enableTexturesClampToEdge();
        LIB_API static void setAnisotropyLevel(int level);
        LIB_API void updateTextureParameteri(void (*parametriSetMethod)(void));
    };
}

