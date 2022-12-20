#include "OVONodeAbstractStrategy.h"
#include "OVOFactory.h"
#include <iostream>

using namespace utopia;

int OVONodeAbstractStrategy::populateUNode(UNode& nodeToMake, Buffer& buffer)
{
	std::string name = std::string(buffer.data.get());
	nodeToMake.setName(name);
	std::cout << "Loading " << name << std::endl;
	buffer.position += (static_cast<unsigned int>(name.length()) + 1);

	//mesh modelview
	glm::mat4 mv;
	std::memcpy(&mv, buffer.data.get() + buffer.position, sizeof(glm::mat4));
	buffer.position += sizeof(glm::mat4);
	nodeToMake.setModelView(mv);

	unsigned int children;
	// Mesh nr. of children nodes:
	memcpy(&children, buffer.data.get() + buffer.position, sizeof(unsigned int));
	buffer.position += sizeof(unsigned int);

	// Optional target node, [none] if not used:
	std::string targetName = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(targetName.length() + 1);

	return children;
}

void OVONodeAbstractStrategy::populateWithChildren(UNode& node, std::ifstream& inFile, unsigned int children)
{
	unsigned int n = 0;
    while (node.getChildCount() < children)
    {
		n = node.getChildCount();
		auto child = OVOFactory::getInstance().recursiveFetch(inFile);
		if (child != nullptr)
			node.addChild(std::move(child));
		else
			children--; //children is not supported therefore we can't add it
    }
}


OVONodeAbstractStrategy::~OVONodeAbstractStrategy() {}