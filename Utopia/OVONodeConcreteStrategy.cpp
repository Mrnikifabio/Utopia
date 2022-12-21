#include "OVONodeConcreteStrategy.h"

using namespace utopia;

std::unique_ptr<UNode> OVONodeConcreteStrategy::decodeChunk(std::ifstream& inFile)
{
	auto buffer = getChunkBytes(inFile);
	unsigned int children = 0;

	auto node = std::make_unique<UNode>(""); //name will be correctly setted by populate node
	children = populateUNode(*node, *buffer);

	//recursive child nodes
	populateWithChildren(*node, inFile, children);

	return std::move(node);
}