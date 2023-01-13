#include "UText.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

using namespace utopia;

struct UText::pimpl {
	std::string m_text;
	glm::vec3 m_color;
	Font m_font;

	pimpl(const std::string& text, const Font& font, const glm::vec3 color) : m_text{ text }, m_color{ color }, m_font{ font } {}
	pimpl() : m_text{ "" }, m_color{ glm::vec3(0,0,0) }, m_font{ Font::DEFAULT_1 } {};
};

UText::UText(const std::string& name, const std::string& text, const Font& font, const glm::vec2& position, const glm::vec3& color) : U2DObject(name, position), m_pimpl{ std::unique_ptr<pimpl>(new pimpl(text, font, color))} {}
UText::UText(const std::string& name) : U2DObject(name), m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) } {}
UText::~UText() = default;

void UText::render()
{
	glColor3fv(glm::value_ptr(m_pimpl->m_color));
	glRasterPos2fv(glm::value_ptr(getPosition()));

	switch(getFont())
	{
        case DEFAULT_2:
            glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char*)m_pimpl->m_text.c_str());
            break;
        case TIMES_ROMAN_10:
            glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (unsigned char*)m_pimpl->m_text.c_str());
            break;
        case TIMES_ROMAN_24:
            glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)m_pimpl->m_text.c_str());
            break;
        case HELVETICA_10:
            glutBitmapString(GLUT_BITMAP_HELVETICA_10, (unsigned char*)m_pimpl->m_text.c_str());
            break;
        case HELVETICA_12:
            glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)m_pimpl->m_text.c_str());
            break;
        case HELVETICA_18:
            glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)m_pimpl->m_text.c_str());
            break;
        default:
            glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)m_pimpl->m_text.c_str());
            break;
	}

}

void UText::setText(const std::string& text)
{
	m_pimpl->m_text = text;
}

void UText::setColor(const glm::vec3& color)
{
	m_pimpl->m_color = color;
}

void UText::setFont(const Font& font)
{
	m_pimpl->m_font = font;
}

const std::string& UText::getText() const
{
	return m_pimpl->m_text;
}

const glm::vec3& UText::getColor() const
{
	return m_pimpl->m_color;
}

const UText::Font& UText::getFont() const
{
	return m_pimpl->m_font;
}
