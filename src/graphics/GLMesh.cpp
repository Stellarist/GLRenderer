#include "GLMesh.hpp"

GLMesh::GLMesh(SubMesh* submesh) :
    submesh(submesh),
    vao(),
    vbo(submesh->getVertices().data(), submesh->getVertices().size()),
    ibo(submesh->getIndices().data(), submesh->getIndices().size())
{
#ifdef DEBUG
	std::println("Creating GLMesh for submesh: {}", submesh->getName());
	std::println("Vertices count: {}, Indices count: {}", submesh->getVerticesCount(), submesh->getIndicesCount());
	std::println("Vertex data size: {} bytes", submesh->getVertices().size() * sizeof(float));
	std::println("Index data size: {} bytes", submesh->getIndices().size() * sizeof(unsigned int));
	std::println("Vertex buffer ID: {}, Index buffer ID: {}", vbo.getId(), ibo.getId());
	std::println("Vertex array ID: {}", vao.id);
	std::println("Submesh shader name: {}", submesh->getShaderName());
	std::println("Submesh material: {}", submesh->getMaterial() ? submesh->getMaterial()->getName() : "None");
	std::println("Submesh visibility: {}", submesh->isVisible() ? "true" : "false");
	std::println("Submesh UID: {}", submesh->getUid());
	std::println("Submesh type: {}", submesh->getType().name());
	std::println("Submesh attributes:");
	for (const auto& [name, attr] : submesh->getAttributes()) {
		std::println("  - {}: format={}, count={}, stride={}, offset={}", name, attr.format, attr.count, attr.stride, attr.offset);
	}
	std::println("Submesh textures:");
	for (const auto& [key, value] : submesh->getMaterial()->getTextures())
		std::println("  - {}: {}", key, value->getName());
	std::println("GLMesh created successfully.");
	std::println("========================================");
#endif

	vao.bind();
	vbo.bind();
	ibo.bind();
	if (VertexAttribute position_attr; submesh->getAttribute("POSITION", position_attr))
		vao.pushAttribute(std::move(position_attr));
	if (VertexAttribute normal_attr; submesh->getAttribute("NORMAL", normal_attr))
		vao.pushAttribute(std::move(normal_attr));
	if (VertexAttribute texcoord_attr; submesh->getAttribute("TEXCOORD_0", texcoord_attr))
		vao.pushAttribute(std::move(texcoord_attr));
	if (VertexAttribute tangent_attr; submesh->getAttribute("TANGENT", tangent_attr))
		vao.pushAttribute(std::move(tangent_attr));
	if (VertexAttribute bitangent_attr; submesh->getAttribute("BITANGENT", bitangent_attr))
		vao.pushAttribute(std::move(bitangent_attr));
	vao.addBuffer(vbo);
	vao.unbind();
}

GLMesh::GLMesh(GLMesh&& other) noexcept :
    submesh(other.submesh),
    vao(std::move(other.vao)),
    vbo(std::move(other.vbo)),
    ibo(std::move(other.ibo)),
    textures(std::move(other.textures))
{
	other.submesh = nullptr;
}

GLMesh& GLMesh::operator=(GLMesh&& other) noexcept
{
	if (this != &other) {
		submesh = other.submesh;
		vao = std::move(other.vao);
		vbo = std::move(other.vbo);
		ibo = std::move(other.ibo);
		textures = std::move(other.textures);

		other.submesh = nullptr;
	}
	return *this;
}

void GLMesh::bind() const
{
	vao.bind();
}

void GLMesh::unbind() const
{
	vao.unbind();
}

const SubMesh* GLMesh::getSubMesh() const
{
	return submesh;
}

void GLMesh::setTexture(const std::string& name, GLTexture& texture)
{
	textures[name] = &texture;
}

GLTexture* GLMesh::getTexture(const std::string& name)
{
	auto it = textures.find(name);
	if (it != textures.end())
		return it->second;

	return nullptr;
}

const std::unordered_map<std::string, GLTexture*>& GLMesh::getTextures() const
{
	return textures;
}

void GLMesh::draw(GLShader& shader)
{
	if (!submesh || !submesh->isVisible())
		return;

	bind();
	unsigned int texture_unit = 0;
	for (const auto& [uniform_name, texture] : textures) {
		texture->activate(texture_unit);
		shader.setInt(uniform_name, static_cast<int>(texture_unit));
		texture_unit++;
	}
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ibo.count), GL_UNSIGNED_INT, 0);
	unbind();
}
