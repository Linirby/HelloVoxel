#pragma once

namespace lili {

class IVoxelWorld {
public:
	virtual ~IVoxelWorld() = default;

	virtual bool is_solid_at(int x, int y, int z) const = 0;
};

}  // namespace lili
