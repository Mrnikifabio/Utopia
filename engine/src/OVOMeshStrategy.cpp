#include "OVOMeshStrategy.h"
#include "OVOFactory.h"
#include "UMesh.h"
#include "Utopia.h"
#include "glm/gtc/packing.hpp"


using namespace utopia;

OVOMeshStrategy& OVOMeshStrategy::getInstance()
{
    static OVOMeshStrategy m_instance;
    return m_instance;
}

enum class OVOMeshStrategy::Subtype: int
{
	// Foundation types:
	DEFAULT = 0,
		NORMALMAPPED,
		TESSELLATED,

		// Terminator:
		LAST,
};

/*
* WARNING THE ORDER IN WHICH EVERYTHING IS CALLED IS ESSENTIAL DON'T CHANGE IT
*/
std::unique_ptr<UNode> OVOMeshStrategy::decodeChunk(std::ifstream& inFile)
{
    const auto buffer = getChunksByte(inFile);

	unsigned int children = 0;
	auto mesh = std::unique_ptr<UMesh>(new UMesh("")); //name will be correctly setted by populate node

    children = populateUNode(*mesh, *buffer);

	//read mesh stuff
    // Mesh subtype (see OvMesh SUBTYPE enum):
    unsigned char subtype;
    memcpy(&subtype, buffer->data.get() + buffer->position, sizeof(unsigned char));
    buffer->position += sizeof(unsigned char);

    // Material name, or [none] if not used:
    std::string materialName = std::string(buffer->data.get()+buffer->position);
    buffer->position += (static_cast<unsigned int>(materialName.length()) + 1);

    //load material from static material hashMap if exist
    if(materialName !="[none]")
        loadMaterial(*mesh, materialName);



    // Mesh bounding sphere radius:
    buffer->position += sizeof(float);

    // Mesh bounding box minimum corner:
    buffer->position += sizeof(glm::vec3);

    // Mesh bounding box maximum corner:
    buffer->position += sizeof(glm::vec3);


    skipPhysics(*buffer);

    loadLODs(*mesh, *buffer);



    //recursive child nodes
    if(children > 0)
	    populateWithChildren(*mesh, inFile, children);

	return std::move(mesh);
}

void OVOMeshStrategy::loadMaterial(UMesh& mesh,const std::string& name)
{
        std::cout << "setting material "<<name << std::endl;
        mesh.setMaterial(Utopia::getInstance().getMaterialByName(name));
}


void OVOMeshStrategy::skipPhysics(Buffer& buffer)
{
    // Optional physics properties:
    unsigned char hasPhysics;
    memcpy(&hasPhysics, buffer.data.get() + buffer.position, sizeof(unsigned char));
    buffer.position += sizeof(unsigned char);

    if (hasPhysics)
    {
        buffer.position += 40; //skipping useless physics properties
        unsigned int nHull = 0;
        memcpy(&nHull, buffer.data.get() + buffer.position, sizeof(unsigned int));
        buffer.position += sizeof(unsigned int);
        buffer.position += 20;//skip the rest of physics header
        if (nHull)
        {
            for (unsigned int c = 0; c < nHull; c++)
            {
                // Hull number of vertices:
                unsigned int nrOfVertices;
                memcpy(&nrOfVertices, buffer.data.get() + buffer.position, sizeof(unsigned int));
                buffer.position += sizeof(unsigned int);

                // Hull number of faces:
                unsigned int nrOfFaces;
                memcpy(&nrOfFaces, buffer.data.get() + buffer.position, sizeof(unsigned int));
                buffer.position += sizeof(unsigned int);

                buffer.position += sizeof(glm::vec3); //skipping centroid

                // Skipping hull vertices:
                for (unsigned int c = 0; c < nrOfVertices; c++)
                    buffer.position += sizeof(glm::vec3);

                // Skipping hull faces:
                for (unsigned int c = 0; c < nrOfFaces; c++)
                    buffer.position += sizeof(unsigned int) * 3;
            }
        }
    }
}

void OVOMeshStrategy::loadLODs(UMesh& mesh, Buffer& buffer)
{
    // Nr. of LODs:
    unsigned int LODs;
    memcpy(&LODs, buffer.data.get() + buffer.position, sizeof(unsigned int));
    buffer.position += sizeof(unsigned int);

    for (unsigned int l = 0; l < LODs; l++)
    {
        auto lod = std::unique_ptr<UMesh::LOD>(new UMesh::LOD());

        // Nr. of vertices:
        unsigned int nrVertices, nrFaces;
        memcpy(&nrVertices, buffer.data.get() + buffer.position, sizeof(unsigned int));
        buffer.position += sizeof(unsigned int);

        lod->vertices = nrVertices;

        // ...and faces:
        memcpy(&nrFaces, buffer.data.get() + buffer.position, sizeof(unsigned int));
        buffer.position += sizeof(unsigned int);

        // Interleaved and compressed vertex/normal/UV/tangent data:
        std::vector<UMesh::Vertex> vertex;
        for (unsigned int c = 0; c < nrVertices; c++)
        {
            // Vertex coords:
            glm::vec3 coord;
            memcpy(&coord, buffer.data.get() + buffer.position, sizeof(glm::vec3));
            buffer.position += sizeof(glm::vec3);

            // Vertex normal:
            unsigned int normalData;
            memcpy(&normalData, buffer.data.get() + buffer.position, sizeof(unsigned int));
            glm::vec4 normal = glm::unpackSnorm3x10_1x2(normalData);
            buffer.position += sizeof(unsigned int);

            // Texture coordinates:
            unsigned int textureData;
            memcpy(&textureData, buffer.data.get() + buffer.position, sizeof(unsigned int));
            glm::vec2 uv = glm::unpackHalf2x16(textureData);
            buffer.position += sizeof(unsigned int);



            // Tangent vector:
            unsigned int tangentData;
            memcpy(&tangentData, buffer.data.get() + buffer.position, sizeof(unsigned int));
            glm::vec4 tangent = glm::unpackSnorm3x10_1x2(tangentData);
            buffer.position += sizeof(unsigned int);

            vertex.push_back(UMesh::Vertex(coord, normal, uv, tangent));
        }


        // Faces:



        for (unsigned int c = 0, i=0; c < nrFaces; c++,i++)
        {
            // Face indexes:
            unsigned int face[3];
            memcpy(face, buffer.data.get() + buffer.position, sizeof(unsigned int) * 3);
            buffer.position += sizeof(unsigned int) * 3;
            UMesh::Face f;




            f.vertices[0] = std::make_shared<UMesh::Vertex>(vertex[face[0]]);
            f.vertices[1] = std::make_shared<UMesh::Vertex>(vertex[face[1]]);
            f.vertices[2] = std::make_shared<UMesh::Vertex>(vertex[face[2]]);
            lod->faces.push_back(f);
        }

        mesh.pushLOD(std::move(lod));
    }
}
