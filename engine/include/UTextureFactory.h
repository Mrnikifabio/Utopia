#pragma once
#include <iostream>
#include <memory>
#include "Utopia.h"
#include "U2DTexture.h"


namespace utopia
{
	class UTextureFactory
	{
	private:

		UTextureFactory();
		~UTextureFactory();

	public:

		static UTextureFactory& getInstance();

		UTextureFactory(UTextureFactory const&) = delete;
		void operator=(UTextureFactory const&) = delete;

		std::shared_ptr<U2DTexture> fromFile2D(const std::string& name);

	};

}

