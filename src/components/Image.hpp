#pragma once

#include <memory>
#include <string>
#include <vector>
#include <typeinfo>
#include <glm/glm.hpp>

#include "base/Component.hpp"

class Image : public Component {
private:
	std::vector<uint8_t> data;

	unsigned int format{0};
	unsigned int width{0};
	unsigned int height{0};

public:
	Image(const std::string& name);
	virtual ~Image() = default;

	virtual std::type_index getType() override;

	auto getData() const -> const std::vector<uint8_t>&;
	void setData(const std::vector<uint8_t>& new_data);

	auto getFormat() const -> unsigned int;
	void setFormat(unsigned int new_format);

	auto getWidth() const -> unsigned int;
	void setWidth(unsigned int new_width);

	auto getHeight() const -> unsigned int;
	void setHeight(unsigned int new_height);
};
