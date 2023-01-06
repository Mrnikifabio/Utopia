#pragma once
#include "UNode.h"
#include "UMaterial.h"

#ifdef _WINDOWS 	
// Export API:
// Specifies i/o linkage (VC++ spec):
#ifdef UTOPIA_EXPORTS
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif      	
#else // Under Linux
#define LIB_API  // Dummy declaration
#endif

namespace utopia
{
    class OVOMeshStrategy;

    class LIB_API UMesh : public UNode
    {
        struct Vertex {
            glm::vec3 coord;
            glm::vec3 normal;
            glm::vec2 uv;
            glm::vec4 tangent;

            Vertex(const glm::vec3& coord, const glm::vec3& normal, 
                const glm::vec2& uv, const glm::vec4& tangent)
            {
                this->coord = coord;
                this->normal = normal;
                this->uv = uv;
                this->tangent = tangent;
            }
        };

        struct Face {
            std::shared_ptr<Vertex> vertices[3];
        };

        struct LOD {
            unsigned int vertices;
            std::vector<Face> faces;
        };

        struct pimpl;
    private:
        std::unique_ptr<pimpl> m_pimpl;
        UMesh(const std::string& name);
        UMesh(const std::string& name, std::weak_ptr<UMaterial> material);
        void pushLOD(std::unique_ptr<LOD>&&);
        friend OVOMeshStrategy;

    public:
        void render() override;
        virtual ~UMesh() noexcept;
        std::weak_ptr<UMaterial> getMaterial();
       void setMaterial(std::weak_ptr<UMaterial> material);
    };
}

