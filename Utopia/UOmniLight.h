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
    class LIB_API UOmniLight : public ULight
    {
    private:
        struct pimpl;
        std::unique_ptr<pimpl> m_pimpl;
    public:
        virtual ~UOmniLight() noexcept;
        void render() override;
		UOmniLight(const std::string& name, const float constantAttenuation = 1.0f, const float linearAttenuation = 0.0f, const float quadraticAttenuation = 0.0f);
		void setConstantAttenuation(const float& constantAttenuation);
		void setLinearAttenuation(const float& linearAttenuation);
		void setQuadraticAttenuation(const float& quadraticAttenuation);
		auto getConstantAttenuation()						const -> const float&;
		auto getLinearAttenuation()							const -> const float&;
		auto getQuadraticAttenuation()						const -> const float&;
    };
}