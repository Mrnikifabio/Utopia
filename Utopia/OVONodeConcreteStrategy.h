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

		static OVONodeConcreteStrategy& getInstance()
		{
			static OVONodeConcreteStrategy m_instance;
			return m_instance;
		}

		OVONodeConcreteStrategy(OVONodeConcreteStrategy const&) = delete;
		void operator=(OVONodeConcreteStrategy const&) = delete;
	};
}

