#include <fstream>
#include <nlohmann/json.hpp>

#include "map_loader.hpp"
#include "map.hpp"

namespace lili {

Map load_map(const std::string &file_path, Player &player) {
	Map game_map;

	std::ifstream file(file_path);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open map file: " + file_path);
	}
	nlohmann::json map_data;
	file >> map_data;
	if (map_data.contains("spawn")) {
		if (!map_data.contains("x"))
			throw std::runtime_error("Spawn does not contains X coordinate");
		if (!map_data.contains("y"))
			throw std::runtime_error("Spawn does not contains Y coordinate");
		if (!map_data.contains("z"))
			throw std::runtime_error("Spawn does not contains Z coordinate");
		player.position.x = map_data["spawn"]["x"];
		player.position.y = map_data["spawn"]["y"];
		player.position.z = map_data["spawn"]["z"];
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
