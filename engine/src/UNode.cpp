#include "UNode.h"
#include "Utopia.h"
#include <stdexcept>
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include "UCamera.h"

using namespace utopia;

struct UNode::pimpl {
	glm::mat4 m_nodeMatrix;
	UNode* m_parent;
	std::vector<std::shared_ptr<UNode>> m_children;

	pimpl() {
		this->m_parent = nullptr;
		this->m_nodeMatrix = glm::mat4(1);
	}
};

UNode::UNode(const std::string& name) : UObject{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) }{}

UNode::~UNode() noexcept = default;

bool UNode::setParent(UNode *parent)
{
	if (parent == nullptr)
	{
		m_pimpl->m_parent = nullptr;
		return true;
	}
	else if (parent->getId() != this->getId() && !isChildPresent(parent->getId()))
	{
		m_pimpl->m_parent = parent;
		return true;
	}
	else
		return false;
}

UNode* UNode::getParent() const
{
	return m_pimpl->m_parent;
}

void UNode::setModelView(const glm::mat4& mv)
{
	m_pimpl->m_nodeMatrix = mv;
}

glm::mat4 UNode::getFinalWorldCoordinates() const
{
	UNode* node = getParent();

	glm::mat4 m = getModelView();
	if (node != nullptr)
	{
		do
		{
			m = node->getModelView() * m;
		} while ((node = node->getParent()) != nullptr);
	}


	return m;
}

void UNode::render()
{
	if (!Utopia::getInstance().isRunning())
		return;

	//auto m = getFinalWorldCoordinates();
	auto m = getModelView();

	m = glm::inverse(UCamera::getMainCamera().lock()->getFinalWorldCoordinates()) * m;

	/*for (auto& child : m_pimpl->m_children)
		child->render();*/

	glLoadMatrixf(glm::value_ptr(m));

}

auto UNode::getModelView() const -> const glm::mat4&
{
	return m_pimpl->m_nodeMatrix;
}

auto UNode::addChild(std::shared_ptr<UNode> child) -> std::weak_ptr<UNode>
{
	if(child->getId() == this->getId())
		throw std::runtime_error("Trying to node as a child of itself");

	if (isChildPresent(child->getId()))
		throw std::runtime_error("Trying to add a node that is already a child");

	if(m_pimpl->m_parent != nullptr && child->getId() == m_pimpl->m_parent->getId())
		throw std::runtime_error("A parent can't add itself to its list of children");

	if (child->getParent() != nullptr)
		throw std::runtime_error(child->getName() + "can't be added because is son of " + child->getParent()->getName());

	child->setParent(this);
	m_pimpl->m_children.push_back(child);
	return std::weak_ptr<UNode>(child);
}

auto UNode::isChildPresent(const unsigned int id) const -> bool
{
	for (auto& child : m_pimpl->m_children)
	{
		if (child->getId() == id)
			return true;
		else if (child->isChildPresent(id))
			return true;
	}

	return false;
}

auto UNode::detachChild(const unsigned int i) -> std::shared_ptr<UNode>
{
	if (i < getChildCount())
	{
		m_pimpl->m_children[i]->setParent(nullptr);
		std::shared_ptr<UNode> child = m_pimpl->m_children[i];
		m_pimpl->m_children.erase(m_pimpl->m_children.begin() + i);
		return child;
	}
	else throw std::out_of_range("Child index is out of range");
}

auto utopia::UNode::detachChildById(const unsigned int id) -> std::shared_ptr<UNode>
{
	for (unsigned int i=0; i < getChildCount(); i++)
	{
		if (m_pimpl->m_children[i]->getId() == id)
		{
			m_pimpl->m_children[i]->setParent(nullptr);
			std::shared_ptr<UNode> child = m_pimpl->m_children[i];
			m_pimpl->m_children.erase(m_pimpl->m_children.begin() + i);
			return child;
		}
	}
	throw std::out_of_range("Child with id not present");
}


auto UNode::getChild(const unsigned int i) const -> std::weak_ptr<UNode>
{
	if(i < getChildCount())
		return std::weak_ptr<UNode>{m_pimpl->m_children[i]};
	else throw std::out_of_range("Child index is out of range");
}




auto UNode::getChildren() const -> const std::vector<std::shared_ptr<UNode>>&
{
	return m_pimpl->m_children;
}

auto utopia::UNode::getChildCount() const -> unsigned int
{
	return (unsigned int)m_pimpl->m_children.size();
}
