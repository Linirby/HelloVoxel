#include "runtime/world_runtime.hpp"

#include "world/map_manager.hpp"
#include "meshing/mesher.hpp"

namespace lili {

WorldRuntime::WorldRuntime() {
	renderer = nullptr;
	albedo_map = nullptr;
	world_atlas_props = { .cols = 1, .rows = 1 };
	world_material = std::make_unique<Material>();
	map = {};
}

void WorldRuntime::set_atlas_map(
	Renderer *renderer, const std::string &file_path, int n_cols, int n_rows
) {
	this->renderer = renderer;
	if (!world_material)
		world_material = std::make_unique<Material>();
	albedo_map = std::make_unique<Texture>(
		renderer->get_device(), file_path
	);
	world_atlas_props.cols = n_cols;
	world_atlas_props.rows = n_rows;
	world_material->albedo_map = albedo_map.get();
}

void WorldRuntime::load_map(const std::string &map_path) {
	map = lili::load_map(map_path);
	load_chunks();
}

void WorldRuntime::save_map(const std::string &file_name) {
	lili::save_map(file_name, map);
}

void WorldRuntime::draw_map() {
	for (const auto &data : chunk_models) {
		renderer->submit(
			*data.second.model,
			data.second.transform,
			RenderLayer::World3D
		);
	}
}

void WorldRuntime::clear_map() {
	chunk_models.clear();
}

Map WorldRuntime::get_map() const {
	return map;
}

void WorldRuntime::add_block(uint8_t block_id, const Vec3 &pos) {
	uint8_t old_block = map.get_block_global(pos.x, pos.y, pos.z);
	if (old_block == 0) {
		map.set_block_global(block_id, pos.x, pos.y, pos.z);
		remesh_chunks_affected_by_block(pos.x, pos.y, pos.z);
	}
}

void WorldRuntime::remove_block(const Vec3 &pos) {
	uint8_t old_block = map.get_block_global(pos.x, pos.y, pos.z);
	if (old_block != 0) {
		map.set_block_global(0, pos.x, pos.y, pos.z);
		remesh_chunks_affected_by_block(pos.x, pos.y, pos.z);
	}
}

void WorldRuntime::load_chunks() {
	if (!renderer)
		throw std::runtime_error("Renderer not bind in WorldRuntime");
	if (map.chunks.empty())
		throw std::runtime_error("Map don't have chunks");
	for (const auto &pair : map.chunks)
		load_unique_chunk(pair.first);
}

void WorldRuntime::load_unique_chunk(uint64_t key) {
	if (!renderer)
		throw std::runtime_error("Renderer not bind in WorldRuntime");
	if (!world_material)
		throw std::runtime_error("World material not initialized in WorldRuntime");
	if (!world_material->albedo_map)
		throw std::runtime_error("World material albedo map not initialized in WorldRuntime");

	auto chunk_it = map.chunks.find(key);
	if (chunk_it == map.chunks.end()) {
		chunk_models.erase(key);
		return;
	}
	ChunkMesher chunk_mesher = ChunkMesher(chunk_it->second, world_atlas_props);
	MeshData chunk_data = chunk_mesher.generate_mesh();
	if (chunk_data.vertices.empty()) {
		chunk_models.erase(key);
		return;
	}
	std::unique_ptr<GPUMesh> chunk_mesh = std::make_unique<GPUMesh>(
		renderer->get_device(), chunk_data
	);
	if (!chunk_mesh)
		throw std::runtime_error("Failed to create chunk GPUMesh");
	std::unique_ptr<Model> chunk_model = std::make_unique<Model>(
		chunk_mesh.get(), world_material.get()
	);
	if (!chunk_model)
		throw std::runtime_error("Failed to create chunk model");

	int chunk_x = static_cast<int16_t>(key >> 32);
	int chunk_y = static_cast<int16_t>(key >> 16);
	int chunk_z = static_cast<int16_t>(key);

	Mat4 transform = Mat4::translate({
		static_cast<float>(chunk_x * Chunk::SIZE),
		static_cast<float>(chunk_y * Chunk::SIZE),
		static_cast<float>(chunk_z * Chunk::SIZE)
	});
	chunk_models[key] = ChunkRenderData{
		std::move(chunk_mesh), std::move(chunk_model), transform
	};
}

void WorldRuntime::remesh_chunks_affected_by_block(int x, int y, int z) {
	const int chunk_x = x >> 4;
	const int chunk_y = y >> 4;
	const int chunk_z = z >> 4;
	const int local_x = x & 15;
	const int local_y = y & 15;
	const int local_z = z & 15;

	std::vector<uint64_t> keys;
	auto push_unique_key = [&](int cx, int cy, int cz) {
		const uint64_t key = map.get_chunk_key(cx, cy, cz);
		for (const uint64_t existing_key : keys) {
			if (existing_key == key) return;
		}
		keys.push_back(key);
	};

	push_unique_key(chunk_x, chunk_y, chunk_z);
	if (local_x == 0) push_unique_key(chunk_x - 1, chunk_y, chunk_z);
	if (local_x == 15) push_unique_key(chunk_x + 1, chunk_y, chunk_z);
	if (local_y == 0) push_unique_key(chunk_x, chunk_y - 1, chunk_z);
	if (local_y == 15) push_unique_key(chunk_x, chunk_y + 1, chunk_z);
	if (local_z == 0) push_unique_key(chunk_x, chunk_y, chunk_z - 1);
	if (local_z == 15) push_unique_key(chunk_x, chunk_y, chunk_z + 1);

	for (const uint64_t key : keys) {
		load_unique_chunk(key);
	}
}

}  // namespace lili
