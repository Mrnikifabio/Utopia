#pragma once
#include "ULight.h"

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
    class LIB_API USpotLight : public ULight
    {
    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;
    public:
        virtual ~USpotLight() noexcept;
        void render() override;
		USpotLight(const std::string& name, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, int lightID, float cutoff, const glm::vec3& direction);        
		
        void setCutoff(const float& cutoff);
		void setDirection(const glm::vec3& direction);
		auto getCutoff()									const -> const float&;
		auto getDirection()									const -> const glm::vec3&;
	};
}