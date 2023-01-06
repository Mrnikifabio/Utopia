#pragma once
#include "OVONodeAbstractStrategy.h"

namespace utopia 
{
	class OVONodeConcreteStrategy :
		public OVONodeAbstractStrategy
	{
	private:
		OVONodeConcreteStrategy() {};
		~OVONodeConcreteStrategy() {};

	public:
		std::unique_ptr<UNode> decodeChunk(std::ifstream& inFile) override;

		static OVONodeConcreteStrategy& getInstance();
		

		OVONodeConcreteStrategy(OVONodeConcreteStrategy const&) = delete;
		void operator=(OVONodeConcreteStrategy const&) = delete;
	};
}

