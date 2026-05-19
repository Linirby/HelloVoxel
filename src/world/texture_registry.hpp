/// Texture registry for world materials.
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "render/core/texture.hpp"

namespace lili {

class TextureRegistry {
public:
	/// Get the global texture registry instance.
	static TextureRegistry &get();

	/// Register a texture and return its ID.
	uint16_t register_texture(const std::string &key, Texture *texture);

	/// Check if a texture key is registered.
	bool has_texture(const std::string &key) const;
	/// Get a texture ID by key.
	uint16_t get_texture_id(const std::string &key) const;
	/// Get a texture by key.
	Texture *get_texture(const std::string &key) const;
	/// Get a texture by ID.
	Texture *get_texture(uint16_t texture_id) const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<Texture *> id_to_texture;

	TextureRegistry();
};

}  // namespace lili
