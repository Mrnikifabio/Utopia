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
    class UDirectionalLight : public ULight
    {
    public:
        LIB_API UDirectionalLight(const std::string& name);
        LIB_API virtual ~UDirectionalLight() noexcept;
        void render() override;
    };
}