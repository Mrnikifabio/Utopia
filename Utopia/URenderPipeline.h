#pragma once
#include <memory>
#include "UObject.h"
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
	class URenderPipeline : public UObject
	{
		struct pimpl;
		struct URenderNode;
	private:
		std::unique_ptr<pimpl> m_pimpl;
	public:
		LIB_API URenderPipeline(const std::string& name);
		LIB_API virtual ~URenderPipeline() noexcept;
		LIB_API void pass(std::weak_ptr<UNode> node, const glm::mat4& mat);
		void render() override;
	};
}
