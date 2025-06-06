#include "Camera.hpp"

Camera::Camera(const std::string& name) :
    Component(name)
{}

std::type_index Camera::getType()
{
	return typeid(Camera);
}

glm::mat4 Camera::getView()
{
	assert(node && "Camera component must be attached to a node");

	auto& transform = node->getComponent<Transform>();
	return glm::inverse(transform.getWorldMatrix());
}

void Camera::setNode(Node& node)
{
	this->node = &node;
}

Node* Camera::getNode()
{
	return node;
}

glm::mat4 Camera::getPreRotation() const
{
	return pre_rotation;
}

void Camera::setPreRotation(const glm::mat4& pre_rotation)
{
	this->pre_rotation = pre_rotation;
}

PerspectiveCamera::PerspectiveCamera(const std::string& name) :
    Camera(name)
{}

PerspectiveCamera::PerspectiveCamera(const std::string& name, float fov, float aspect_ratio, float near_plane, float far_plane) :
    Camera(name),
    fov(fov),
    aspect_ratio(aspect_ratio),
    near_plane(near_plane),
    far_plane(far_plane)
{}

std::type_index PerspectiveCamera::getType()
{
	return typeid(PerspectiveCamera);
}

float PerspectiveCamera::getFarPlane() const
{
	return far_plane;
}

void PerspectiveCamera::setFarPlane(float zfar)
{
	far_plane = zfar;
}

float PerspectiveCamera::getNearPlane() const
{
	return near_plane;
}

void PerspectiveCamera::setNearPlane(float znear)
{
	near_plane = znear;
}

float PerspectiveCamera::getAspectRatio() const
{
	return aspect_ratio;
}

void PerspectiveCamera::setAspectRatio(float aspect_ratio)
{
	this->aspect_ratio = aspect_ratio;
}

float PerspectiveCamera::getFov() const
{
	return fov;
}

void PerspectiveCamera::setFov(float fov)
{
	this->fov = fov;
}

glm::mat4 PerspectiveCamera::getProjection()
{
	return glm::perspective(fov, aspect_ratio, near_plane, far_plane);
}

OrthoCamera::OrthoCamera(const std::string& name) :
    Camera{name}
{}

OrthoCamera::OrthoCamera(const std::string& name, float left, float right, float bottom, float top, float near_plane, float far_plane) :
    Camera{name},
    left{left},
    right{right},
    top{top},
    bottom{bottom},
    near_plane{near_plane},
    far_plane{far_plane}
{}

std::type_index OrthoCamera::getType()
{
	return typeid(OrthoCamera);
}

float OrthoCamera::getLeft() const
{
	return left;
}

void OrthoCamera::setLeft(float new_left)
{
	left = new_left;
}

float OrthoCamera::getRight() const
{
	return right;
}

void OrthoCamera::setRight(float new_right)
{
	right = new_right;
}

float OrthoCamera::getTop() const
{
	return top;
}

void OrthoCamera::setTop(float new_top)
{
	top = new_top;
}

float OrthoCamera::getBottom() const
{
	return bottom;
}

void OrthoCamera::setBottom(float new_bottom)
{
	bottom = new_bottom;
}

void OrthoCamera::setNearPlane(float new_near_plane)
{
	near_plane = new_near_plane;
}

float OrthoCamera::getNearPlane() const
{
	return near_plane;
}

void OrthoCamera::setFarPlane(float new_far_plane)
{
	far_plane = new_far_plane;
}

float OrthoCamera::getFarPlane() const
{
	return far_plane;
}

glm::mat4 OrthoCamera::getProjection()
{
	return glm::ortho(left, right, bottom, top, far_plane, near_plane);
}
