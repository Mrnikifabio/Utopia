#pragma once
#include "UMaterial.h"
#include "OVOStrategy.h"


namespace utopia
{
	class OVOMaterialStrategy : public OVOStrategy
	{
		
	private:
		OVOMaterialStrategy() {};
		~OVOMaterialStrategy() {};

		void readMaterial(UMaterial& material, Buffer& buffer);

	public:

		std::unique_ptr<UNode> decodeChunk(std::ifstream& inFile) override;


		static OVOMaterialStrategy& getInstance();
		

		OVOMaterialStrategy(OVOMaterialStrategy const&) = delete;
		void operator=(OVOMaterialStrategy const&) = delete;

	};
}

