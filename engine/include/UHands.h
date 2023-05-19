#pragma once
#include "UObject.h"
#include <UMesh.h>
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
		LIB_API void setXDistanceFromCam(float units);
		LIB_API void setYDistanceFromCam(float units);
		LIB_API void setZDistanceFromCam(float units);
		LIB_API void setDownScaleFactorForLeap(float factor);
		LIB_API bool checkIfHandsAreIn(std::shared_ptr<UMesh> mesh);
	};
}