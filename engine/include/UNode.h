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
	class UNode : public UObject
	{
	private:
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;
		bool setParent(UNode* parent);

	public:
		LIB_API UNode(const std::string& name);
		LIB_API virtual ~UNode() noexcept;

		void render()									    override;
		LIB_API void setModelView(const glm::mat4& mv);
		LIB_API auto addChild(std::shared_ptr<UNode> child)				  -> std::weak_ptr<UNode>;
		LIB_API auto detachChild(const unsigned int i)->std::shared_ptr<UNode>;
		LIB_API auto detachChildById(const unsigned int id)->std::shared_ptr<UNode>;
		LIB_API auto getParent()									const -> UNode*;
		LIB_API auto getModelView()									const -> const glm::mat4&;
		LIB_API auto isChildPresent(const unsigned int id)			const -> bool;
		LIB_API auto getChild(const unsigned int i)					const->std::weak_ptr<UNode>;
		LIB_API auto getChildren()									const -> const std::vector<std::shared_ptr<UNode>>&;
		LIB_API auto getChildCount()								const -> unsigned int;
		LIB_API auto getFinalWorldCoordinates()						const -> glm::mat4;
	};

}
