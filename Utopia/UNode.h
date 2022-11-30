#pragma once
#include "UObject.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>

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
	class LIB_API UNode : public UObject
	{
	private:
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;
		bool setParent(UNode* parent);

	public:
		UNode(const std::string& name);
		virtual ~UNode() noexcept;

		void render()									    override;
		void setModelView(const glm::mat4& mv);
		auto addChild(std::shared_ptr<UNode> child)				  -> std::weak_ptr<UNode>;
		auto detachChild(const unsigned int i)					  -> std::shared_ptr<UNode>;
		auto getParent()									const -> UNode*;
		auto getModelView()									const -> const glm::mat4&;
		auto isChildPresent(const unsigned int id)			const -> bool;
		auto getChild(const unsigned int i)					const -> std::weak_ptr<UNode>;
		auto getChildren()									const -> const std::vector<std::shared_ptr<UNode>>&;
		auto getChildCount()								const -> unsigned int;
		auto getFinalWorldCoordinates()						const -> glm::mat4&;
	};

}
