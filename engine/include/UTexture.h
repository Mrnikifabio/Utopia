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
    class UTexture : public UObject
    {
    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;

        static std::shared_ptr<UTexture> m_defaultTexture;
        
        virtual unsigned int getGlTextureType() = 0;

    public:
        UTexture(const std::string& name, unsigned int texId);
        
        LIB_API virtual ~UTexture() noexcept;

        void render() override;

        LIB_API static int getMaxAnisotropicLevel();

        LIB_API void enableNearestFilter();
        LIB_API void enableNearestBitmapNearestFilter();
        LIB_API void enableLinearFilter();
        LIB_API void enableLinearBitmapNearestFilter();
        LIB_API void enableLinearBitmapLinearFilter();
        LIB_API void updateAnisotropyLevel(int value);
        
        LIB_API virtual void enableTextureRepeat() = 0;
        LIB_API virtual void enableTextureClampToEdge() = 0;

        unsigned int getTexId();
    };
}

