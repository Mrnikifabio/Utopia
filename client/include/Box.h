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

		std::shared_ptr<utopia::UNode> getHookPointNode() const;
		std::shared_ptr<utopia::UNode> getGroundNode() const;
		std::shared_ptr<utopia::UNode> getTopNode() const;

		void computeGravity();

		bool isHooked() const;
		void take();
		void release();
	};
}
