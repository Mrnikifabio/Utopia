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
	class LIB_API UOrthographicCamera : public UCamera
	{
	public:
		float right, left, top, bottom, far, near;

		UOrthographicCamera(const std::string& name, float right, float left, float top, float bottom, float far, float near);
		UOrthographicCamera(const std::string& name);
		const glm::mat4& getCameraMatrix() const override;
	};
}

