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
	private:
		float m_right, m_left, m_top, m_bottom;

	public:
		float getRight() const;
		float getLeft() const;
		float getTop() const;
		float getBottom() const;

		void setRight(const float right);
		void setLeft(const float left);
		void setTop(const float top);
		void setBottom(const float bottom);

		UOrthographicCamera(const std::string& name);
		const glm::mat4 getCameraMatrix() const override;
	};
}

