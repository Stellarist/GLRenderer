#include "Component.hpp"

Component::Component(const std::string& name) :
    name(name)
{}

const std::string& Component::getName() const
{
	return name;
}

void Component::setName(const std::string& name)
{
	this->name = name;
}
