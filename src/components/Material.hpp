#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "base/Component.hpp"
#include "Texture.hpp"

enum class AlphaMode : uint8_t {
	OPAQUE,
	MASK,
	BLEND,
	OTHER
};

class Material : public Component {
private:
	glm::vec3 emissive{0.0f, 0.0f, 0.0f};
	bool      double_sided{false};
	float     alpha_cutoff{0.0f};
	AlphaMode alpha_mode{AlphaMode::OPAQUE};

	std::unordered_map<std::string, Texture*> textures;

public:
	Material(const std::string& name);

	Material(const Material&) = delete;
	Material& operator=(const Material&) = delete;

	Material(Material&& other) noexcept = default;
	Material& operator=(Material&& other) noexcept = default;

	~Material() override = default;

	std::type_index getType() override;

	auto getEmissive() -> glm::vec3;
	void setEmissive(const glm::vec3& emissive);

	bool getDoubleSided() const;
	void setDoubleSided(bool double_sided);

	float getAlphaCutoff() const;
	void  setAlphaCutoff(float alpha_cutoff);

	auto getAlphaMode() -> AlphaMode;
	void setAlphaMode(AlphaMode alpha_mode);

	auto getTextures() -> std::unordered_map<std::string, Texture*>&;
	auto getTextures() const -> const std::unordered_map<std::string, Texture*>&;
	void addTexture(const std::string& name, Texture* texture);
};

class PBRMaterial : public Material {
private:
	glm::vec4 base_color_factor{0.0f, 0.0f, 0.0f, 0.0f};
	float     metallic_factor{0.0f};
	float     roughness_factor{0.0f};

public:
	PBRMaterial(const std::string& name);

	PBRMaterial(const PBRMaterial&) = delete;
	PBRMaterial& operator=(const PBRMaterial&) = delete;

	PBRMaterial(PBRMaterial&& other) noexcept = default;
	PBRMaterial& operator=(PBRMaterial&& other) noexcept = default;

	~PBRMaterial() override = default;

	std::type_index getType() override;

	auto setBaseColorFactor(const glm::vec4& base_color_factor) -> void;
	auto getBaseColorFactor() const -> glm::vec4;

	auto setMetallicFactor(float metallic_factor) -> void;
	auto getMetallicFactor() const -> float;

	auto setRoughnessFactor(float roughness_factor) -> void;
	auto getRoughnessFactor() const -> float;
};
