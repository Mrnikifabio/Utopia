#pragma once
#include <memory>
#include "UObject.h"
#include "UMaterial.h"
#include "UNode.h"

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
	class U3DRenderPipeline : public UObject
	{
	private:
		struct pimpl;
		struct U3DRenderNode;
		std::unique_ptr<pimpl> m_pimpl;
	public:
		LIB_API U3DRenderPipeline(const std::string& name);
		LIB_API virtual ~U3DRenderPipeline() noexcept;
		LIB_API void pass(std::weak_ptr<UNode> node, const glm::mat4& mat = glm::mat4(1), std::shared_ptr<UMaterial> material = nullptr);
		LIB_API void clear();
		LIB_API void render() override;
	};
}
