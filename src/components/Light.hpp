#pragma once

#include <string>
#include <vector>
#include <typeinfo>
#include <glm/glm.hpp>

#include "base/Component.hpp"
#include "base/Node.hpp"

class Light : public Component {
private:
	Node*     node{nullptr};
	glm::vec3 color{1.0f, 1.0f, 1.0f};
	float     intensity{1.0f};

public:
	Light(const std::string& name);
	Light(Light&& other) = default;
	virtual ~Light() = default;

	virtual std::type_index getType() override;

	Node* getNode();
	void  setNode(Node& node);

	glm::vec3 getColor() const;
	void      setColor(const glm::vec3& color);

	float getIntensity() const;
	void  setIntensity(float intensity);
};

class DirectionalLight : public Light {
private:
	glm::vec3 direction{0.0f, 0.0f, -1.0f};

public:
	DirectionalLight(const std::string& name);
	virtual ~DirectionalLight() = default;

	virtual std::type_index getType() override;

	glm::vec3 getDirection() const;
	void      setDirection(const glm::vec3& direction);
};

class PointLight : public Light {
private:
	float range{10.0f};

public:
	PointLight(const std::string& name);
	virtual ~PointLight() = default;

	virtual std::type_index getType() override;

	float getRange() const;
	void  setRange(float range);
};

class SpotLight : public Light {
private:
	glm::vec3 direction{0.0f, 0.0f, -1.0f};
	float     range{10.0f};
	float     inner_cone_angle{0.0f};
	float     outer_cone_angle{0.0f};

public:
	SpotLight(const std::string& name);
	virtual ~SpotLight() = default;

	virtual std::type_index getType() override;

	glm::vec3 getDirection() const;
	void      setDirection(const glm::vec3& direction);

	float getRange() const;
	void  setRange(float range);

	float getInnerConeAngle() const;
	void  setInnerConeAngle(float angle);

	float getOuterConeAngle() const;
	void  setOuterConeAngle(float angle);
};
