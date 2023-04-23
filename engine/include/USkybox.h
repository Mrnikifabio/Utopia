#pragma once

#include <iostream>

#include "UNode.h"
#include "UVertexShader.h"
#include "UFragmentShader.h"
#include "UProgramShader.h"
#include "UCubeMapTexture.h"

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
    class USkybox : public UNode
    {
    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;

        static std::shared_ptr<UVertexShader> m_vert;
        static std::shared_ptr<UFragmentShader> m_frag;
        static std::shared_ptr<UProgramShader> m_prog;

        static float cubeVertices[];
        static unsigned short cubeFaces[];

    public:
        LIB_API USkybox(const std::string& name);
        LIB_API void setTexture(std::shared_ptr<UCubeMapTexture> texture);
        LIB_API virtual ~USkybox() noexcept;
        void render() override;
    };
}

