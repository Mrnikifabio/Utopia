#pragma once

#include "U2DObject.h"

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

	class UText : public U2DObject
	{
	private:
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;
	public:

		enum Font : int
		{
			DEFAULT_1 = 0,
			DEFAULT_2,
			TIMES_ROMAN_10,
			TIMES_ROMAN_24,
			HELVETICA_10,
			HELVETICA_12,
			HELVETICA_18
		};

		LIB_API UText(const std::string& name, const std::string& text, const Font& font, const glm::vec2& position, const glm::vec3& color);
		LIB_API UText(const std::string& name);
		LIB_API const std::string& getText() const;
		LIB_API const glm::vec3& getColor() const;
		LIB_API const Font& getFont() const;
		LIB_API void setText(const std::string& text);
		LIB_API void setColor(const glm::vec3& color);
		LIB_API void setFont(const Font& font);
		LIB_API virtual ~UText() noexcept;
		LIB_API void render();
	};
}
