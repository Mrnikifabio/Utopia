#include "OVOMaterialStrategy.h"
#include "OVOMeshStrategy.h"
#include "OVOFactory.h"
#include "glm/gtc/packing.hpp"
#include "utopia.h"


using namespace utopia;

std::unique_ptr<UNode> OVOMaterialStrategy::decodeChunk(std::ifstream& inFile)
{
	const auto buffer = getChunksByte(inFile);

	auto materialToMake = std::make_shared<UMaterial>("");
	readMaterial(*materialToMake, *buffer);
	Utopia::getInstance().addMaterial(materialToMake.get()->getName(), materialToMake);

	return nullptr;
}

void OVOMaterialStrategy::readMaterial(UMaterial& material, Buffer& buffer)
{
	std::string name = std::string(buffer.data.get());
	material.setName(name);
	std::cout << "Loading " << name << std::endl;
	buffer.position += (static_cast<unsigned int>(name.length()) + 1);

	//emission
	glm::vec3 emission;
	std::memcpy(&emission, buffer.data.get() + buffer.position, sizeof(glm::vec3));
	buffer.position += sizeof(glm::vec3);
	material.setEmission(glm::vec4(emission, 1.0f));

	//albedo?
	glm::vec3 albedo;
	std::memcpy(&albedo, buffer.data.get() + buffer.position, sizeof(glm::vec3));
	material.setDiffuse(glm::vec4(albedo, 1.0f));

	buffer.position += sizeof(glm::vec3);

	float roughness;
	std::memcpy(&roughness, buffer.data.get() + buffer.position, sizeof(float));
	buffer.position += sizeof(float);

	float metalness;
	std::memcpy(&metalness, buffer.data.get() + buffer.position, sizeof(float));
	buffer.position += sizeof(float);

	float tranparency;
	std::memcpy(&tranparency, buffer.data.get() + buffer.position, sizeof(float));
	buffer.position += sizeof(float);


	// Optional target node, [none] if not used:
	std::string targetName = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(targetName.length() + 1);

	// Optional target node, [none] if not used:
	targetName = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(targetName.length() + 1);


	// Optional target node, [none] if not used:
	targetName = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(targetName.length() + 1);


	// Optional target node, [none] if not used:
	targetName = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(targetName.length() + 1);

	// Optional target node, [none] if not used:
	targetName = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(targetName.length() + 1);

}
