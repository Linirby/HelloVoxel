/// Runtime API for loading, drawing, and editing voxel worlds.
#pragma once

#include <unordered_map>
#include <memory>

#include "geometry/mat4x4.hpp"

#include "meshing/mesher.hpp"

#include "render/renderer.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/model.hpp"

#include "world/map.hpp"

namespace lili {

struct ChunkRenderData {
	std::unique_ptr<GPUMesh> mesh = nullptr;
	std::unique_ptr<Model> model = nullptr;
	Mat4 transform;
};

class WorldRuntime {
public:
	/// Create a world runtime.
	WorldRuntime();
	/// Destroy the world runtime.
	~WorldRuntime() = default;

	/// Set the atlas texture and grid size.
	void set_atlas_map(
		Renderer *renderer, const std::string &file_path, int n_cols, int n_rows
	);

	/// Load a map from disk.
	void load_map(const std::string &map_path);
	/// Save the current map to disk.
	void save_map(const std::string &file_name);
	/// Submit all chunks for rendering.
	void draw_map();
	/// Clear chunk render data.
	void clear_map();

	/// Get a copy of the current map.
	Map get_map() const;

	/// Add a block at the given position.
	void add_block(uint8_t block_id, const Vec3 &pos);
	/// Remove a block at the given position.
	void remove_block(const Vec3 &pos);

private:
	Renderer *renderer;

	std::unique_ptr<Texture> albedo_map;
	AtlasProperties world_atlas_props;
	std::unique_ptr<Material> world_material;
	Map map;
	std::unordered_map<uint64_t, ChunkRenderData> chunk_models;

	void load_chunks();

	void load_unique_chunk(uint64_t key);
	void remesh_chunks_affected_by_block(int x, int y, int z);
};

}  // namespace lili
