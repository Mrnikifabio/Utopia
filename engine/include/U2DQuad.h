#pragma once
#include "UObject.h"
#include <glm/glm.hpp>

namespace utopia {

	class U2DQuad : UObject
	{
	private:
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;
	public:
		U2DQuad(const std::string& name, 
			const int width,
			const int height,
			const unsigned int orthoWidth,
			const unsigned int orthoHeight,
			const glm::mat4& postionMatrix);
		virtual ~U2DQuad() noexcept;
		void activeAsBuffer();
		void disableAsBuffer();
		unsigned int getTextureID() const;
		void render();
		bool init();
	};
}