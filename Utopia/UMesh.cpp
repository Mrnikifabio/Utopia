#include "UMesh.h"

using namespace utopia;

UMesh::UMesh(const std::string& name) : UNode{name}
{
	
}

void UMesh::render()
{
	//render something somehow and then we make a call to UNode render, to render all of the children
	UNode::render();
}


