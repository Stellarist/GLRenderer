#pragma once

#include <string>
#include <typeindex>
#include <cassert>

#include "Image.hpp"
#include "base/Component.hpp"

class Texture : public Component {
private:
	Image* image{nullptr};

public:
	Texture(const std::string& name);
	Texture(Texture&& other) = default;
	virtual ~Texture() = default;

	virtual std::type_index getType() override;

	auto getImage() -> Image*;
	void setImage(Image& image);
};
