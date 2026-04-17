#ifndef MAP_LOADER_HPP
# define MAP_LOADER_HPP

# include <string>

# include "map.hpp"
# include "entity/player.hpp"

namespace lili {

Map load_map(const std::string &map_path, Player &player);

}  // namespace lili

#endif
