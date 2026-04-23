#ifndef MAP_MANAGER_HPP
# define MAP_MANAGER_HPP

# include <string>

# include "map.hpp"
# include "entity/player.hpp"
# include "render/camera.hpp"

namespace lili {

void save_map(
	const std::string &file_path,
	const Map &game_map,
	const Player &player,
	const Camera &camera
);
Map load_map(const std::string &map_path, Player &player, Camera &camera);

}  // namespace lili

#endif  // MAP_MANAGER_HPP
