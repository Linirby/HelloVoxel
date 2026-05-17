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
	WorldRuntime();
	~WorldRuntime() = default;

	void set_atlas_map(
		Renderer *renderer, const std::string &file_path, int n_cols, int n_rows
	);

	void load_map(const std::string &map_path);
	void save_map(const std::string &file_name);
	void draw_map();
	void clear_map();

	Map get_map() const;

	void add_block(uint8_t block_id, const Vec3 &pos);
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
