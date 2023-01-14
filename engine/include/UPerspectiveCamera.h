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
	class UPerspectiveCamera : public UCamera
	{
	private:
		float m_fovRadians;
	public:
		LIB_API float getFovRadians() const;
		LIB_API void setFovRadians(float fovRadians);
		LIB_API UPerspectiveCamera(const std::string& name);
		LIB_API const glm::mat4 getCameraMatrix() const override;
	};
}

