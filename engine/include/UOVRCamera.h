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
	class UOVRCamera : public UCamera
	{
	private:
		glm::mat4 m_cameraMatrix;
	public:
		UOVRCamera(const std::string& name);
		void setCameraMatrix(const glm::mat4& mat);
		const glm::mat4 getCameraMatrix() const override;
	};
}
