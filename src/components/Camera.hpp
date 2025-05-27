#pragma once

#include <memory>
#include <string>
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include <glm/glm.hpp>

#include "Transform.hpp"
#include "base/Component.hpp"
#include "base/Node.hpp"

class Camera : public Component {
private:
	Node*     node{nullptr};
	glm::mat4 pre_rotation{1.0f};

public:
	Camera(const std::string& name);
	virtual ~Camera() = default;

	virtual std::type_index getType() override;

	virtual glm::mat4 getProjection() = 0;

	auto getView() -> glm::mat4;

	Node* getNode();
	void  setNode(Node& node);

	glm::mat4 getPreRotation() const;
	void      setPreRotation(const glm::mat4& pre_rotation);
};

class PerspectiveCamera : public Camera {
private:
	float aspect_ratio{1.0f};
	float fov{glm::radians(60.0f)};
	float far_plane{100.0};
	float near_plane{0.1f};

public:
	PerspectiveCamera(const std::string& name);
	virtual ~PerspectiveCamera() = default;

	virtual std::type_index getType() override;

	float getFarPlane() const;
	void  setFarPlane(float zfar);

	float getNearPlane() const;
	void  setNearPlane(float znear);

	float getAspectRatio() const;
	void  setAspectRatio(float aspect_ratio);

	float getFov() const;
	void  setFov(float fov);

	virtual glm::mat4 getProjection() override;
};

class OrthoCamera : public Camera {
private:
	float left{-1.0f};
	float right{1.0f};
	float top{1.0f};
	float bottom{-1.0f};
	float near_plane{0.0f};
	float far_plane{1.0f};

public:
	OrthoCamera(const std::string& name);
	OrthoCamera(const std::string& name, float left, float right, float bottom, float top, float near_plane, float far_plane);
	virtual ~OrthoCamera() = default;

	virtual std::type_index getType() override;

	float getLeft() const;
	void  setLeft(float left);

	float getRight() const;
	void  setRight(float right);

	float getTop() const;
	void  setTop(float top);

	float getBottom() const;
	void  setBottom(float bottom);

	float getNearPlane() const;
	void  setNearPlane(float near_plane);

	float getFarPlane() const;
	void  setFarPlane(float far_plane);

	virtual glm::mat4 getProjection() override;
};
