#pragma once
#include <iostream>
#include <memory>
#include "Utopia.h"
#include "UTexture.h"


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

		std::shared_ptr<UTexture> fromFile(const std::string& name);

	};

}

