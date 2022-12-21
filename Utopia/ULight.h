#pragma once
#include "UNode.h"

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
    class LIB_API ULight : public UNode
    {
    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;
    public:
		ULight(const std::string& name, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, const glm::vec4& gAmbient, const int lightID);
        virtual ~ULight() noexcept;
        void render() override;

        void setAmbient(const glm::vec4& ambient);
        void setDiffuse(const glm::vec4& diffuse);
        void setSpecular(const glm::vec4& specular);
		void setGlobalAmbient(const glm::vec4& gAmbient);
        auto getAmbient()									const -> const glm::vec4&;
        auto getDiffuse()									const -> const glm::vec4&;
        auto getSpecular()									const -> const glm::vec4&;
		auto getGlobalAmbient()								const -> const glm::vec4&;
        auto getLightID()									const -> int;
    };
}