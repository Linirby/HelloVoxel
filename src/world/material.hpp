#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "geometry/vec4.hpp"
#include "render/core/texture.hpp"

namespace lili {

struct MaterialProps {
	Vec4 color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	float roughness = 1.0f;
	float metallic = 0.0f;
	float emission = 0.0f;
	float padding = 0.0f;
};

struct Material {
	MaterialProps properties;
	Texture *albedo_map = nullptr;

	Material() = default;
	Material(Texture *texture);
};

class MaterialRegistry {
public:
	MaterialRegistry(const MaterialRegistry&) = delete;
	MaterialRegistry& operator=(const MaterialRegistry&) = delete;
	static MaterialRegistry &get();

	uint16_t register_material(const std::string &key, const Material &material);

	bool has_material(const std::string &key) const;
	uint16_t get_material_id(const std::string &key) const;
	const Material &get_material(const std::string &key) const;
	const Material &get_material(uint16_t material_id) const;
	const Material &get_material(uint8_t material_id) const;

	size_t material_count() const;
	const Material *material_data() const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<Material> id_to_material;

	MaterialRegistry();
};

}  // namespace lili
