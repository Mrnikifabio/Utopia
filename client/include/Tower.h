#include <memory>
#include "UNode.h"
#include "Utopia.h"
#include "Box.h"

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

		void setRoot(std::shared_ptr<utopia::UNode> root);
		void setHook(std::shared_ptr<utopia::UNode> hook);
		void setFisicalHook(std::shared_ptr<utopia::UNode> hook);
		void setTower(std::shared_ptr<utopia::UNode> tower);
		void setCable(std::shared_ptr<utopia::UNode> cable);

		std::shared_ptr<utopia::UNode> getRoot() const;
		std::shared_ptr<utopia::UNode> getHook() const;
		std::shared_ptr<utopia::UNode> getFisicalHook() const;
		std::shared_ptr<utopia::UNode> getTower() const;
		std::shared_ptr<utopia::UNode> getCable() const;

		bool isHooking() const;
		void take(std::shared_ptr<Box> box);
		void release();
		
		std::shared_ptr<utopia::UNode> getFisicalHook();

		void setHookLimitforward(float value);
		void setHookLimitbackward(float value);

		void setFisicalHookLimitUp(float value);
		void setFisicalHookLimitDown(float value);

		void rotateTower(glm::f32 angle);
		void moveFisicalHookUpDown(float distance);


		void moveHookBackwardForward(float distance);


	};
}

