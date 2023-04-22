#pragma once
#pragma once

#include "UTexture.h"
#include <iostream>
#include "glm/glm.hpp"
#include <functional>
#include <unordered_map>

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

    class U2DTexture : public UTexture
    {
    private:
        static std::unordered_map<std::string, std::shared_ptr<U2DTexture>> m_textures;
        static std::shared_ptr<U2DTexture> m_defaultTexture;

        U2DTexture(const std::string& name, unsigned int texId);
        U2DTexture(const std::string& name);
        friend UTextureFactory;
        static std::shared_ptr<U2DTexture> createWhiteTexture();

        unsigned int getGlTextureType() override; 

    public:
        LIB_API virtual ~U2DTexture() noexcept;

        LIB_API static std::shared_ptr<U2DTexture> loadTexture(const std::string& name, int component, int width, int height, unsigned int format, unsigned int type, const void* data);
        LIB_API static const std::shared_ptr<U2DTexture> getDefaultTexture();


        LIB_API void enableTextureRepeat() override;
        LIB_API void enableTextureClampToEdge() override;

        LIB_API static void forEach(std::function<void(std::shared_ptr<U2DTexture>)> apply);
        LIB_API static std::shared_ptr<U2DTexture> getByName(const std::string& name);
        LIB_API static void add(const std::string& name, std::shared_ptr<U2DTexture> texture);
        LIB_API static bool contains(const std::string& name);
        LIB_API static unsigned int size();
    };
}

