#include "world/material.hpp"

#include <stdexcept>

namespace lili {

Material::Material(Texture *texture) {
	albedo_map = texture;
}

MaterialRegistry &MaterialRegistry::get() {
	static MaterialRegistry instance;
	return instance;
}

uint16_t MaterialRegistry::register_material(
	const std::string &key, const Material &material
) {
	auto it = key_to_id.find(key);
	if (it != key_to_id.end()) {
		id_to_material[it->second] = material;
		return it->second;
	}

	if (id_to_material.size() >= UINT16_MAX)
		throw std::runtime_error(
			"Material registry reached uint16_t capacity."
		);

	uint16_t new_id = static_cast<uint16_t>(id_to_material.size());
	key_to_id[key] = new_id;
	id_to_material.push_back(material);
	return new_id;
}

bool MaterialRegistry::has_material(const std::string &key) const {
	return key_to_id.contains(key);
}

uint16_t MaterialRegistry::get_material_id(const std::string &key) const {
	auto it = key_to_id.find(key);
	if (it == key_to_id.end())
		throw std::runtime_error("Material key not found: " + key);
	return it->second;
}

const Material &MaterialRegistry::get_material(const std::string &key) const {
	return get_material(get_material_id(key));
}

const Material &MaterialRegistry::get_material(uint16_t material_id) const {
	if (material_id >= id_to_material.size())
		throw std::runtime_error("Material ID out of range.");
	return id_to_material[material_id];
}

const Material &MaterialRegistry::get_material(uint8_t material_id) const {
	return get_material(static_cast<uint16_t>(material_id));
}

MaterialRegistry::MaterialRegistry() {
	register_material("core:missing", Material());

	Material log_tint;
	log_tint.properties.color_tint = { 1.0f, 0.9f, 0.4f, 1.0f };
	register_material("core:log_tint", log_tint);
}

size_t MaterialRegistry::material_count() const {
	return id_to_material.size();
}

const Material *MaterialRegistry::material_data() const {
	return id_to_material.data();
}

}  // namespace lili
