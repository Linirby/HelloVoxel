#ifndef WORLD_MAP_MANAGER_HPP
# define WORLD_MAP_MANAGER_HPP

# include <string>

# include "world/map.hpp"

namespace lili {

void save_map(
	const std::string &file_path,
	const Map &game_map
);
Map load_map(const std::string &map_path);

}  // namespace lili

#endif  // WORLD_MAP_MANAGER_HPP
