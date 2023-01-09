#include "OVONodeConcreteStrategy.h"

using namespace utopia;

OVONodeConcreteStrategy& OVONodeConcreteStrategy::getInstance()
{
	static OVONodeConcreteStrategy m_instance;
	return m_instance;
}

std::unique_ptr<UNode> OVONodeConcreteStrategy::decodeChunk(std::ifstream& inFile)
{

	auto buffer = getChunksByte(inFile);
	unsigned int children = 0;

	auto node = std::unique_ptr<UNode>(new UNode("")); //name will be correctly setted by populate node
	children = populateUNode(*node, *buffer);

	//recursive child nodes
	populateWithChildren(*node, inFile, children);

	return node;
}
