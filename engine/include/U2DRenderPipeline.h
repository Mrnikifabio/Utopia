#pragma once
#include "UObject.h"
#include "U2DObject.h"
#include <glm/glm.hpp>

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

namespace utopia {

    class U2DRenderPipeline : public UObject
    {
	private:
		struct pimpl;
		struct U2DRenderNode;
		std::unique_ptr<pimpl> m_pimpl;
	public:
		LIB_API U2DRenderPipeline(const std::string& name);
		LIB_API virtual ~U2DRenderPipeline() noexcept;
		LIB_API void clear();
		LIB_API void pass(std::weak_ptr<U2DObject> node, const glm::vec2& pos);
		LIB_API void render() override;
    };

}


