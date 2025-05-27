#pragma once

#include <memory>
#include <glad/glad.h>

struct GLTexture {
	unsigned int id;
	unsigned int format;
	unsigned int data_type;

	GLTexture();
	GLTexture(unsigned int format, unsigned int data_type);
	GLTexture(const void* data, int width, int height,
	          unsigned int format, unsigned int data_type);

	GLTexture(const GLTexture&) = delete;
	GLTexture& operator=(const GLTexture&) = delete;

	GLTexture(GLTexture&& other);
	GLTexture& operator=(GLTexture&& other);

	~GLTexture();

	unsigned int getId() const;

	void upload(const void* data, int width, int height);

	void bind() const;
	void unbind() const;
	void activate(unsigned int unit) const;
	void deactivate(unsigned int unit);

	void setParameteri(std::initializer_list<std::pair<int, int>> param) const;
	void generateMipmap() const;
};
