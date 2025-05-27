#pragma once

#include <string>
#include <vector>
#include <typeinfo>
#include <glm/glm.hpp>

#include "base/Component.hpp"

class AABB : public Component {
private:
	glm::vec3 min;
	glm::vec3 max;

public:
	AABB();
	AABB(const glm::vec3& min, const glm::vec3& max);
	virtual ~AABB() = default;

	virtual std::type_index getType() override;

	void update(const glm::vec3& point);
	void update(const std::vector<glm::vec3>& vertex_data, const std::vector<unsigned int>& index_data);
	void transform(const glm::mat4& transform);
	void reset();

	glm::vec3 getScale() const;
	glm::vec3 getCenter() const;
	glm::vec3 getMin() const;
	glm::vec3 getMax() const;
};
