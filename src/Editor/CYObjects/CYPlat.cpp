#include "CYPlat.h"

CYPlat::CYPlat(Property::Position position, Property::Material material, u8 floor, u8 height, u8 size)
	: position(position)
	, material(material)
	, floor(floor)
	, height(height)
	, size(size)
{}


void CYPlat::createMesh(const WorldTextures& wTex)
{
	Mesh mesh;
	float pHeight = ((float)floor + MeshBuilder::getPlatGeometricHeight(height)) * WORLD_HEIGHT;
	auto pSize = MeshBuilder::getPlatGeometricSize(size);

	glm::vec2 platMin = { (float)position.x - pSize / 2.f, (float)position.y - pSize / 2.f };
	glm::vec2 platMax = { (float)position.x + pSize / 2.f, (float)position.y + pSize / 2.f };

	std::array<glm::vec3, 4> fVertices;
	fVertices[1] = glm::vec3((platMin.x) / WORLD_SIZE, pHeight / WORLD_SIZE, (platMin.y) / WORLD_SIZE);
	fVertices[2] = glm::vec3((platMax.x) / WORLD_SIZE, pHeight / WORLD_SIZE, (platMin.y) / WORLD_SIZE);
	fVertices[3] = glm::vec3((platMax.x) / WORLD_SIZE, pHeight / WORLD_SIZE, (platMax.y) / WORLD_SIZE);
	fVertices[0] = glm::vec3((platMin.x) / WORLD_SIZE, pHeight / WORLD_SIZE, (platMax.y) / WORLD_SIZE);
	std::array<glm::vec3, 4> bVertices = { fVertices[3], fVertices[2], fVertices[1], fVertices[0] };

	// FRONT FACE
	MeshBuilder::addQuadToMesh(mesh, fVertices, material);
	MeshBuilder::applyPlatTextureCoords(mesh, fVertices, material, wTex);
	
	// BACK FACE
	MeshBuilder::addQuadToMesh(mesh, bVertices, material);
	MeshBuilder::applyPlatTextureCoords(mesh, bVertices, material, wTex);

	m_geometryMesh = std::move(mesh);

	// Create AABB for octree placement
	m_objectAABB = { {platMin.x, pHeight, platMin.y }, { platMax.x, pHeight, platMax.y } };

	// Create Collision Mesh
	m_collisionMesh.clear();
	CPolygon c_front1({ fVertices[0], fVertices[1], fVertices[3] });
	CPolygon c_front2({ fVertices[1], fVertices[2], fVertices[3] });
	CPolygon c_front3({ fVertices[3], fVertices[1], fVertices[0] });
	CPolygon c_front4({ fVertices[3], fVertices[2], fVertices[1] });

	m_collisionMesh.push_back(std::move(c_front1));
	m_collisionMesh.push_back(std::move(c_front2));
	m_collisionMesh.push_back(std::move(c_front3));
	m_collisionMesh.push_back(std::move(c_front4));
}

Mesh& CYPlat::getMesh()
{
	return m_geometryMesh;
};

MinBoundingBox& CYPlat::getAABB()
{
	return m_objectAABB;
}

