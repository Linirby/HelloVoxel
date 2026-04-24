#include <fstream>
#include <nlohmann/json.hpp>

#include "map_manager.hpp"
#include "map.hpp"
#include "math/utils.hpp"

namespace lili {

void save_map(
	const std::string &file_path,
	const Map &game_map,
	const Player &player,
	const Camera &camera
) {
    nlohmann::json map_data;

    map_data["spawn"]["x"] = lili::floor(player.position.x) + 0.5;
    map_data["spawn"]["y"] = lili::floor(player.position.y);
    map_data["spawn"]["z"] = lili::floor(player.position.z) + 0.5;
    map_data["spawn"]["rotation"] = {
		lili::floor(camera.yaw), lili::floor(camera.pitch)
	}; 

    map_data["blocks"] = nlohmann::json::array();

    for (const auto &[key, chunk] : game_map.chunks) {
        int16_t chunk_x = static_cast<int16_t>(key >> 32);
        int16_t chunk_y = static_cast<int16_t>(key >> 16);
        int16_t chunk_z = static_cast<int16_t>(key);

        for (int lx = 0; lx < lili::Chunk::SIZE; ++lx) {
            for (int ly = 0; ly < lili::Chunk::SIZE; ++ly) {
                for (int lz = 0; lz < lili::Chunk::SIZE; ++lz) {
                    uint8_t block_id = chunk.get_block(lx, ly, lz);
                    if (block_id != 0) {
                        int global_x = (chunk_x * lili::Chunk::SIZE) + lx;
                        int global_y = (chunk_y * lili::Chunk::SIZE) + ly;
                        int global_z = (chunk_z * lili::Chunk::SIZE) + lz;
                        map_data["blocks"].push_back({
                            {"id", block_id},
                            {"x", global_x},
                            {"y", global_y},
                            {"z", global_z}
                        });
                    }
                }
            }
        }
    }
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error(
			"Failed to open map file for saving: " + file_path
		);
    }
    file << map_data.dump(4); 
}

Map load_map(const std::string &file_path, Player &player, Camera &camera) {
	Map game_map;

	std::ifstream file(file_path);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open map file: " + file_path);
	}
	nlohmann::json map_data;
	file >> map_data;
	if (map_data.contains("spawn")) {
		if (!map_data["spawn"].contains("x"))
			throw std::runtime_error("Spawn does not contains X coordinate");
		if (!map_data["spawn"].contains("y"))
			throw std::runtime_error("Spawn does not contains Y coordinate");
		if (!map_data["spawn"].contains("z"))
			throw std::runtime_error("Spawn does not contains Z coordinate");
		if (!map_data["spawn"].contains("rotation"))
			throw std::runtime_error("Spawn does not contatins a rotation");
		player.position.x = map_data["spawn"]["x"];
		player.position.y = map_data["spawn"]["y"];
		player.position.z = map_data["spawn"]["z"];
		camera.set_rotation(
			map_data["spawn"]["rotation"][0],
			map_data["spawn"]["rotation"][1]
		);
		player.velocity = { 0.0f, 0.0f, 0.0f };
	}
	if (map_data.contains("blocks")) {
		for (const auto &block : map_data["blocks"]) {
			if (!block.contains("id"))
				throw std::runtime_error("Block is missing the id");
			if (!block.contains("x"))
				throw std::runtime_error("Block is missing the X coordinate");
			if (!block.contains("y"))
				throw std::runtime_error("Block is missing the Y coordinate");
			if (!block.contains("z"))
				throw std::runtime_error("Block is missing the Z coordinate");
			game_map.set_block_global(
				block["id"],
				block["x"],
				block["y"],
				block["z"]
			);
		}
	}
	return game_map;
}

}  // namespace lili
