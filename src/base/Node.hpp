#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Component.hpp"
#include "components/Transform.hpp"

class Node : public Object {
private:
	size_t      id;
	std::string name;
	Node*       parent{nullptr};
	Transform   transform;

	std::vector<Node*> children;

	std::unordered_map<std::type_index, Component*> components;

public:
	Node(const size_t id, const std::string& name);
	virtual ~Node() = default;

	virtual std::type_index getType() override;

	const size_t getId() const;

	const std::string& getName() const;
	void               setName(const std::string& name);

	Node* getParent() const;
	void  setParent(Node& parent);

	Transform& getTransform();

	template <typename T>
	T&         getComponent() const;
	Component& getComponent(std::type_index type) const;
	void       setComponent(Component& component);

	template <typename T>
	bool hasComponent() const;
	bool hasComponent(std::type_index type) const;

	const std::vector<Node*>& getChildren() const;
	void                      addChild(Node& child);
};

template <typename T>
T& Node::getComponent() const
{
	return dynamic_cast<T&>(getComponent(typeid(T)));
}

template <typename T>
inline bool Node::hasComponent() const
{
	return hasComponent(typeid(T));
}
