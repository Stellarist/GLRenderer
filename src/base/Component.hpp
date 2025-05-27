#pragma once

#include <string>
#include <typeindex>

#include "Object.hpp"

class Component : public Object {
private:
	std::string name;

public:
	Component() = default;
	Component(const std::string& name);
	Component(Component&&) = default;
	virtual ~Component() = default;

	virtual std::type_index getType() = 0;

	auto getName() const -> const std::string&;
	void setName(const std::string& name);
};
