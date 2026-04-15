#ifndef TOOLS_MAP_LOADER_HPP
# define TOOLS_MAP_LOADER_HPP

# include <string>

class MapLoader {
public:
	MapLoader(const std::string &map_path);
	~MapLoader();
};

#endif
