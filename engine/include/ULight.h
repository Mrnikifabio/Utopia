#pragma once
#include "UNode.h"
#include <stack>

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
    class ULight : public UNode
    {
    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;
        static std::stack<int> m_freeLightIDs;

    public:
	
        LIB_API ULight(const std::string& name);
        LIB_API virtual ~ULight() noexcept;
        void render() override;
        LIB_API static void initIDs();

        LIB_API void setAmbient(const glm::vec4& ambient);
        LIB_API void setDiffuse(const glm::vec4& diffuse);
        LIB_API void setSpecular(const glm::vec4& specular);
        LIB_API void setGlobalAmbient(const glm::vec4& gAmbient);
        LIB_API auto getAmbient()									const -> const glm::vec4&;
        LIB_API auto getDiffuse()									const -> const glm::vec4&;
        LIB_API auto getSpecular()									const -> const glm::vec4&;
        LIB_API auto getGlobalAmbient()								const -> const glm::vec4&;
        LIB_API auto getLightID()									const -> int;
    };
}