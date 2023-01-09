#pragma once

#include <memory>
#include "UNode.h"
#include "Box.h"

namespace client
{
	class BoxesManager
	{
		struct pimpl;

	private:
		std::unique_ptr<pimpl> m_pimpl;

	public:
		BoxesManager();
		~BoxesManager();

		std::shared_ptr<Box> possibleBoxToHook(std::shared_ptr<utopia::UNode> hook, float distance);
		void setBoxes(std::vector <std::shared_ptr<Box>> boxes);
		
		void computeGravity();
	};
}