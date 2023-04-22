#pragma once

#include <iostream>
#include <functional>
#include <array>

#include <glm/glm.hpp>

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
    class UTextureFactory;

    class UCubeMapTexture : public UTexture
    {
    private:
        UCubeMapTexture(const std::string& name, unsigned int texId);
        UCubeMapTexture(const std::string& name);
        friend UTextureFactory;

        unsigned int getGlTextureType() override;

    public:
        LIB_API virtual ~UCubeMapTexture() noexcept;

        static std::shared_ptr<UCubeMapTexture> load(const std::string& name, const std::array<UTextureData, 6>& data);

        LIB_API void enableTextureRepeat() override;
        LIB_API void enableTextureClampToEdge() override;
    };
}

