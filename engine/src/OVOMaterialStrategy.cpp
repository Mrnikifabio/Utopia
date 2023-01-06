#include "OVOMaterialStrategy.h"
#include "OVOMeshStrategy.h"
#include "OVOFactory.h"
#include "glm/gtc/packing.hpp"
#include "Utopia.h"
#include "UTextureFactory.h"


using namespace utopia;


OVOMaterialStrategy& OVOMaterialStrategy::getInstance()
{
	static OVOMaterialStrategy m_instance;
	return m_instance;
}

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

	//albedo
	glm::vec3 albedo;
	std::memcpy(&albedo, buffer.data.get() + buffer.position, sizeof(glm::vec3));
	buffer.position += sizeof(glm::vec3);

	//ambient=albedox0.2
	material.setAmbient(glm::vec4(albedo * 0.2f, 1.0f));

	//specular=albedox0.4
	material.setSpecular(glm::vec4(albedo * 0.4f, 1.0f));

	//diffuse=albedox0.6
	material.setDiffuse(glm::vec4(albedo * 0.6f, 1.0f));


	float roughness;
	std::memcpy(&roughness, buffer.data.get() + buffer.position, sizeof(float));
	buffer.position += sizeof(float);
	//shininess=(1−roughness)x128
	material.setShininess((int)((1.f - sqrt((int)roughness)) * 128));

	float metalness;
	std::memcpy(&metalness, buffer.data.get() + buffer.position, sizeof(float));
	buffer.position += sizeof(float);

	float tranparency;
	std::memcpy(&tranparency, buffer.data.get() + buffer.position, sizeof(float));
	buffer.position += sizeof(float);


	// Optional target node, [none] if not used:
	std::string albedoMap = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(albedoMap.length() + 1);
	if (albedoMap != "[none]")
	{
		std::cout<<"loading texture "<< albedoMap<<std::endl;

		if (Utopia::getInstance().containTexture(albedoMap))
		{
			std::cout << "texture " << albedoMap << " presente" << std::endl;
			material.setTexture(Utopia::getInstance().getTextureByName(albedoMap));
		}
		else
			material.setTexture(UTextureFactory::getInstance().fromFile("assets/"+albedoMap));
	}
	

	// Optional target node, [none] if not used:
	std::string normalMap = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(normalMap.length() + 1);


	// Optional target node, [none] if not used:
	std::string heightMap = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(heightMap.length() + 1);

	
	// Optional target node, [none] if not used:
	std::string roughtnessMap = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(roughtnessMap.length() + 1);

	// Optional target node, [none] if not used:
	std::string metalnessMap = std::string(buffer.data.get() + buffer.position);
	buffer.position += static_cast<unsigned int>(metalnessMap.length() + 1);


}
