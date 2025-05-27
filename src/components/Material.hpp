#pragma once

#include <string>
#include <vector>
#include <typeinfo>
#include <unordered_map>
#include <glm/glm.hpp>

#include "base/Component.hpp"
#include "Texture.hpp"

enum class AlphaMode {
	Opaque,
	Mask,
	Blend,
	Other
};

class Material : public Component {
private:
	glm::vec3 emissive{0.0f, 0.0f, 0.0f};
	bool      double_sided{false};
	float     alpha_cutoff{0.5f};
	AlphaMode alpha_mode{AlphaMode::Opaque};

	std::unordered_map<std::string, Texture*> textures;

public:
	Material(const std::string& name);
	Material(Material&& other) = default;
	virtual ~Material() = default;

	virtual std::type_index getType() override;

	auto getEmissive() -> glm::vec3;
	void setEmissive(const glm::vec3& emissive);

	bool getDoubleSided();
	void setDoubleSided(bool double_sided);

	float getAlphaCutoff();
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
	virtual ~PBRMaterial() = default;

	virtual std::type_index getType() override;

	auto setBaseColorFactor(const glm::vec4& base_color_factor) -> void;
	auto getBaseColorFactor() const -> glm::vec4;

	auto setMetallicFactor(float metallic_factor) -> void;
	auto getMetallicFactor() const -> float;

	auto setRoughnessFactor(float roughness_factor) -> void;
	auto getRoughnessFactor() const -> float;
};
