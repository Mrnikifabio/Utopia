#pragma once
#include "UObject.h"
#include <glm/glm.hpp>

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

namespace utopia {

	class U2DObject : public UObject
	{
	private:
		glm::vec2 m_position;
	public:
		LIB_API U2DObject(const std::string& name, const glm::vec2& position);
		LIB_API U2DObject(const std::string& name);
		LIB_API virtual ~U2DObject() noexcept;

		LIB_API const glm::vec2& getPosition() const;
		LIB_API void setPosition(const glm::vec2& position);

		LIB_API virtual void render() = 0;
	};
}