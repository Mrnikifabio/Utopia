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
    class USpotLight : public ULight
    {
    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;
    public:
		LIB_API virtual ~USpotLight() noexcept;
		void render() override;
		LIB_API USpotLight(const std::string& name, const float cutoff, const glm::vec3& direction, const float constantAttenuation = 1.0f, const float linearAttenuation = 0.0f, const float quadraticAttenuation = 0.0f);
		
		LIB_API void setCutoff(const float& cutoff);
		LIB_API void setDirection(const glm::vec3& direction);
		LIB_API void setConstantAttenuation(const float& constantAttenuation);
		LIB_API void setLinearAttenuation(const float& linearAttenuation);
		LIB_API void setQuadraticAttenuation(const float& quadraticAttenuation);
		LIB_API auto getCutoff()									const -> const float&;
		LIB_API auto getDirection()									const -> const glm::vec3&;
		LIB_API auto getConstantAttenuation()						const -> const float&;
		LIB_API auto getLinearAttenuation()							const -> const float&;
		LIB_API auto getQuadraticAttenuation()						const -> const float&;
	};
}