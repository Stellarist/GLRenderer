#pragma once

#include <typeindex>

static uint64_t id_counter = 0;

class Object {
private:
	uint64_t uid;

public:
	Object();
	virtual ~Object() = default;

	uint64_t getUid() const;

	virtual std::type_index getType() = 0;
};
