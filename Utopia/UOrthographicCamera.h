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
	class UOrthographicCamera : public UCamera
	{
	private:
		float m_right, m_left, m_top, m_bottom;

	public:
		LIB_API float getRight() const;
		LIB_API float getLeft() const;
		LIB_API float getTop() const;
		LIB_API float getBottom() const;

		LIB_API void setRight(const float right);
		LIB_API void setLeft(const float left);
		LIB_API void setTop(const float top);
		LIB_API void setBottom(const float bottom);

		LIB_API UOrthographicCamera(const std::string& name);
		LIB_API const glm::mat4 getCameraMatrix() const override;
	};
}

