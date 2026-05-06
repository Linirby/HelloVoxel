#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "render/core/texture.hpp"

namespace lili {

class TextureRegistry {
public:
	static TextureRegistry &get();

	uint16_t register_texture(const std::string &key, Texture *texture);

	bool has_texture(const std::string &key) const;
	uint16_t get_texture_id(const std::string &key) const;
	Texture *get_texture(const std::string &key) const;
	Texture *get_texture(uint16_t texture_id) const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<Texture *> id_to_texture;

	TextureRegistry();
};

}  // namespace lili
