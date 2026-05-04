#include "material.hpp"

namespace lili {

Material::Material(Texture *texture) {
	albedo_map = texture;
}

}  // namespace lili
