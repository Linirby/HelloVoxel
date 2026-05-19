/// Physics interface for voxel worlds.
#pragma once

namespace lili {

class IVoxelWorld {
public:
	/// Destroy the voxel world interface.
	virtual ~IVoxelWorld() = default;

	/// Return whether a voxel is solid at the given coordinates.
	virtual bool is_solid_at(int x, int y, int z) const = 0;
};

}  // namespace lili
