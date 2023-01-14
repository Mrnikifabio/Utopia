#include "OVOFactory.h"
#include "OVOMeshStrategy.h"
#include "OVONodeConcreteStrategy.h"
#include "OVOLightStrategy.h"
#include "UObject.h"
#include "OVOObject.h"
#include "OVOStrategy.h"
#include "OVOMaterialStrategy.h"
#include <iostream>
#include <fstream>
#include <exception>

using namespace utopia;

// standalone function object
struct OVOObjectTypeHash {
    std::size_t operator()(OVOObject::Type e) const {
        return static_cast<std::size_t>(e);
    }
};

struct OVOFactory::pimpl {
    std::unordered_map<OVOObject::Type, std::reference_wrapper<OVOStrategy>, OVOObjectTypeHash> ovoStrategies;
};


OVOFactory::OVOFactory() : m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) }
{
    loadStrategies();
}

OVOFactory::~OVOFactory() = default;

void OVOFactory::loadStrategies()
{
    m_pimpl->ovoStrategies.insert(std::make_pair<OVOObject::Type, std::reference_wrapper<OVOMeshStrategy>>(OVOObject::Type::MESH, OVOMeshStrategy::getInstance()));
	m_pimpl->ovoStrategies.insert(std::make_pair<OVOObject::Type, std::reference_wrapper<OVOLightStrategy>>(OVOObject::Type::LIGHT, OVOLightStrategy::getInstance()));
    m_pimpl->ovoStrategies.insert(std::make_pair<OVOObject::Type, std::reference_wrapper<OVONodeConcreteStrategy>>(OVOObject::Type::NODE, OVONodeConcreteStrategy::getInstance()));
    m_pimpl->ovoStrategies.insert(std::make_pair<OVOObject::Type, std::reference_wrapper<OVOMaterialStrategy>>(OVOObject::Type::MATERIAL, OVOMaterialStrategy::getInstance()));
    m_pimpl->ovoStrategies.insert(std::make_pair<OVOObject::Type, std::reference_wrapper<OVOMaterialStrategy>>(OVOObject::Type::TEXTURE, OVOMaterialStrategy::getInstance()));
}

std::shared_ptr<UNode> OVOFactory::fromFile(const std::string& path)
{
    std::ifstream inFile("assets/"+path, std::ios::binary | std::ios_base::ate);
    std::shared_ptr<UNode> newNode = nullptr;

    unsigned int size = (unsigned int)inFile.tellg();
    inFile.seekg(std::ios_base::beg);

    while (inFile.tellg() < size)
    {
        newNode = recursiveFetch(inFile);
    }

    inFile.close();

    return newNode;
}

std::unique_ptr<UNode> OVOFactory::recursiveFetch(std::ifstream& inFile)
{
    std::unique_ptr<UNode> newNode = nullptr;

    unsigned int chunkId;
    inFile.read(reinterpret_cast<char*>(&chunkId), sizeof(unsigned int));
    auto type = static_cast<OVOObject::Type>(chunkId);

    if (m_pimpl->ovoStrategies.count(type))
        newNode = m_pimpl->ovoStrategies.at(type).get().decodeChunk(inFile);
    else
    {
        //no strategy found to decode chunk therefore we skip it
        unsigned int chunkSize;
        if (inFile.read(reinterpret_cast<char*>(&chunkSize), sizeof(unsigned int)))
            inFile.ignore(chunkSize);
        else
            throw std::runtime_error("bad file");
    }

    return newNode;
}

LIB_API OVOFactory& utopia::OVOFactory::getInstance()
{
    static OVOFactory m_instance;
    return m_instance;
}
