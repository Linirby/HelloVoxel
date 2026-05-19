/// Material registry and material definitions.
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

	/// Create a default material.
	Material() = default;
	/// Create a material with a given albedo texture.
	Material(Texture *texture);
};

class MaterialRegistry {
public:
	/// Non-copyable.
	MaterialRegistry(const MaterialRegistry&) = delete;
	/// Non-copyable.
	MaterialRegistry& operator=(const MaterialRegistry&) = delete;
	/// Get the global material registry instance.
	static MaterialRegistry &get();

	/// Register a material and return its ID.
	uint16_t register_material(const std::string &key, const Material &material);

	/// Check if a material key is registered.
	bool has_material(const std::string &key) const;
	/// Get a material ID by key.
	uint16_t get_material_id(const std::string &key) const;
	/// Get a material by key.
	const Material &get_material(const std::string &key) const;
	/// Get a material by ID.
	const Material &get_material(uint16_t material_id) const;
	/// Get a material by ID (uint8 overload).
	const Material &get_material(uint8_t material_id) const;

	/// Get the number of registered materials.
	size_t material_count() const;
	/// Get a pointer to the contiguous material data.
	const Material *material_data() const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<Material> id_to_material;

	MaterialRegistry();
};

}  // namespace lili
