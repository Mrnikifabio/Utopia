#pragma once

#include <memory>
#include "UNode.h"

namespace client
{
	class Box
	{
		struct pimpl;
	private:
		std::unique_ptr<pimpl> m_pimpl;

	public:

		Box();
		~Box();

		void setHookPointNode(std::shared_ptr<utopia::UNode> hookPointNode);
		void setGroundNode(std::shared_ptr<utopia::UNode> groundNode);
		void setTopNode(std::shared_ptr<utopia::UNode> topNode);

		std::shared_ptr<utopia::UNode> getHookPointNode();
		std::shared_ptr<utopia::UNode> getGroundNode();
		std::shared_ptr<utopia::UNode> getTopNode();

		void computeGravity();

		bool isHooked();
		void take();
		void release();
	};
}
