#include "world/texture_registry.hpp"

#include <stdexcept>

namespace lili {

TextureRegistry &TextureRegistry::get() {
	static TextureRegistry instance;
	return instance;
}

uint16_t TextureRegistry::register_texture(
	const std::string &key, Texture *texture
) {
	if (!texture)
		throw std::runtime_error("Cannot register a null texture.");

	auto it = key_to_id.find(key);
	if (it != key_to_id.end()) {
		id_to_texture[it->second] = texture;
		return it->second;
	}

	if (id_to_texture.size() >= UINT16_MAX)
		throw std::runtime_error("Texture registry reached uint16_t capacity.");

	uint16_t new_id = static_cast<uint16_t>(id_to_texture.size());
	key_to_id[key] = new_id;
	id_to_texture.push_back(texture);
	return new_id;
}

bool TextureRegistry::has_texture(const std::string &key) const {
	return key_to_id.contains(key);
}

uint16_t TextureRegistry::get_texture_id(const std::string &key) const {
	auto it = key_to_id.find(key);
	if (it == key_to_id.end())
		throw std::runtime_error("Texture key not found: " + key);
	return it->second;
}

Texture *TextureRegistry::get_texture(const std::string &key) const {
	return get_texture(get_texture_id(key));
}

Texture *TextureRegistry::get_texture(uint16_t texture_id) const {
	if (texture_id >= id_to_texture.size())
		throw std::runtime_error("Texture ID out of range.");
	return id_to_texture[texture_id];
}

TextureRegistry::TextureRegistry() {
	id_to_texture.push_back(nullptr);
	key_to_id["core:missing"] = 0;
}

}  // namespace lili
