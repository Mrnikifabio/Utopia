#pragma once
#include "UObject.h"
#include <U3DRenderPipeline.h>

namespace utopia {

	class UHands : public UNode
	{
	private:
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;
	public:
		LIB_API UHands();
		LIB_API virtual ~UHands() noexcept;
		LIB_API bool init();
		//void render() override;
		LIB_API void update();
	};
}