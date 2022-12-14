#pragma once
#include "UCamera.h"

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
	class LIB_API UPerspectiveCamera : public UCamera
	{
	public:
		float fovRadians, near, far;
		
		UPerspectiveCamera(const std::string& name, float fovRadians, float near, float far);
		UPerspectiveCamera(const std::string& name);
		
		const glm::mat4& getCameraMatrix() const override;
	};
}

