#include "Object.hpp"

Object::Object() :
    uid(id_counter++)
{}

uint64_t Object::getUid() const
{
	return uid;
}