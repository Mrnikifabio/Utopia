#include <memory>
#include "UNode.h"
#include "utopia.h"

#pragma once
namespace client
{

	class Tower
	{
		struct pimpl;
	private:
		std::unique_ptr<pimpl> m_pimpl;

	public:
		Tower(std::shared_ptr<utopia::UNode> tower);
		Tower();
		~Tower();


		void setHook(std::shared_ptr<utopia::UNode> hook);
		void setFisicalHook(std::shared_ptr<utopia::UNode> hook);

		void setTower(std::shared_ptr<utopia::UNode> tower);

		void setHookLimitforward(float value);
		void setHookLimitbackward(float value);

		void setFisicalHookLimitUp(float value);
		void setFisicalHookLimitDown(float value);

		void rotateTower(glm::f32 angle);
		void moveFisicalHookUpDown(float distance);

		void moveHookBackwardForward(float distance);


	};
}

