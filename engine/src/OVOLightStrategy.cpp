#include "OVOLightStrategy.h"
#include "ULight.h"
#include "UDirectionalLight.h"
#include "UOmniLight.h"
#include "USpotLight.h"
#include <cstring>


using namespace utopia;

enum class OVOLightStrategy::Subtype : int
{
	// Foundation types:
	OMNI = 0,
	DIRECTIONAL,
	SPOT
};

OVOLightStrategy::OVOLightStrategy()
{
}

OVOLightStrategy::~OVOLightStrategy()
{
}

OVOLightStrategy& OVOLightStrategy::getInstance()
{
	static OVOLightStrategy instance;
	return instance;
}

std::unique_ptr<UNode> OVOLightStrategy::decodeChunk(std::ifstream& inFile)
{
	auto buffer = getChunksByte(inFile);
	unsigned int children = 0;

	buffer->position = tellUNodeSize(*buffer);

	// Read light stuff
	// Light subtype (see OvLight SUBTYPE enum):
	unsigned char subtype;
	memcpy(&subtype, buffer->data.get() + buffer->position, sizeof(unsigned char));
	buffer->position += sizeof(unsigned char);

	// Light color:
	glm::vec3 color;
	memcpy(&color, buffer->data.get() + buffer->position, sizeof(glm::vec3));
	buffer->position += sizeof(glm::vec3);

	// Light influence radius
	float radius;
	memcpy(&radius, buffer->data.get() + buffer->position, sizeof(float));
	buffer->position += sizeof(float);

	// Light direction
	glm::vec3 direction;
	memcpy(&direction, buffer->data.get() + buffer->position, sizeof(glm::vec3));
	buffer->position += sizeof(glm::vec3);

	// Light cutoff
	float cutoff;
	memcpy(&cutoff, buffer->data.get() + buffer->position, sizeof(float));
	buffer->position += sizeof(float);

	// Spot light exponent
	float exponent;
	memcpy(&exponent, buffer->data.get() + buffer->position, sizeof(float));
	buffer->position += sizeof(float);

	// Shadow flag
	bool shadow;
	memcpy(&shadow, buffer->data.get() + buffer->position, sizeof(bool));
	buffer->position += sizeof(bool);

	// Volumetric flag
	bool volumetric;
	memcpy(&volumetric, buffer->data.get() + buffer->position, sizeof(bool));
	buffer->position += sizeof(bool);

	auto node = std::unique_ptr<ULight>(new ULight("")); //name will be correctly setted by populate node

	switch ((Subtype) subtype)
	{
		case Subtype::DIRECTIONAL: node = std::unique_ptr<UDirectionalLight>(new UDirectionalLight("")); break;
		case Subtype::OMNI: node = std::unique_ptr<UOmniLight>(new UOmniLight("")); break;
		case Subtype::SPOT: node = std::unique_ptr<USpotLight>(new USpotLight("", cutoff, direction)); break;
	}

	// reset buffer position
	buffer->position = 0;

	children = populateUNode(*node, *buffer);

	//recursive child nodes
	populateWithChildren(*node, inFile, children);

	return std::move(node);
}
