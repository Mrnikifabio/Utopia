#pragma once
#include "OVOStrategy.h"
#include "UNode.h"
#include <stdexcept>
#include <memory>

namespace utopia
{
	class OVONodeAbstractStrategy :
		public OVOStrategy
	{
	public:
		OVONodeAbstractStrategy() {};
		virtual ~OVONodeAbstractStrategy() = 0;
		int populateUNode(UNode& nodeToMake, Buffer& buffer);
		void populateWithChildren(UNode& node, std::ifstream& inFile, const unsigned int children);
	};
}

