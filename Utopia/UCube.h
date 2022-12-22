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

    class LIB_API UCube : public UNode
    {
        struct pimpl;
    private:
        std::unique_ptr<pimpl> m_pimpl;
    public:
        UCube(const std::string& name);
        UCube(const std::string& name, std::weak_ptr<UMaterial> material);
        void render() override;
        virtual ~UCube() noexcept;
        std::weak_ptr<UMaterial> getMaterial();
        void setMaterial(std::weak_ptr<UMaterial> material);
    };
}

