#pragma once
#include <memory>

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class Entity
{
public:
	Entity(const char* name, Mesh mesh, Material material);
	~Entity();

	std::shared_ptr<Transform> GetTransform() { return transform; }
	std::shared_ptr<Mesh> GetMesh() { return mesh; }
	std::shared_ptr<Material> GetMaterial() { return material; }
	void SetMesh(Mesh);
	void SetMaterial(Material);
	const char* name;

	void Draw();
private:
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
};