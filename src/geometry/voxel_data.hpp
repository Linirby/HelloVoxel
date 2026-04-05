#ifndef GEOMETRY_VOXEL_DATA_HPP
# define GEOMETRY_VOXEL_DATA_HPP

namespace lili {

inline constexpr int face_checks[6][3]{
	{  0,  1,  0 },  // Top
	{  0, -1,  0 },  // Bottom
	{  1,  0,  0 },  // Right
	{ -1,  0,  0 },  // Left
	{  0,  0,  1 },  // Front
	{  0,  0, -1 }   // Back
};

inline constexpr float face_vertices[6][4][3]{
	// Top (+Y)
	{
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f }
	},
	// Bottom (-Y)
	{
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f }
	},
	// Right (+X)
	{
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f }
	},
	// Left (-X)
	{
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f }
	},
	// Front (+Z)
	{
		{ 0.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f }
	},
	// Back (-Z)
	{
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	},
};

}  // namespace lili

#endif
