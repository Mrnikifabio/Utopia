#pragma once
#include <iostream>
#include <memory>
#include "UTexture.h"
#include "utopia.h"

namespace utopia
{
	class  TextureFactory
	{
	private:

		TextureFactory();
		~TextureFactory();

	public:

		static TextureFactory& getInstance();

		TextureFactory(TextureFactory const&) = delete;
		void operator=(TextureFactory const&) = delete;

		std::shared_ptr<UTexture> fromFile(const std::string& name);
	};

}

