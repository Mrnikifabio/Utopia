#pragma once
#include <iostream>
#include <memory>
#include "UTexture.h"
#include "utopia.h"

namespace utopia
{
	class  ImageReader
	{
	

	private:

		ImageReader() {};
		~ImageReader() {};

	public:

		static ImageReader& getInstance()
		{
			static ImageReader m_instance;
			return m_instance;
		}

		ImageReader(ImageReader const&) = delete;
		void operator=(ImageReader const&) = delete;

		std::shared_ptr<UTexture> fromFile(const std::string& name);
	};

}

