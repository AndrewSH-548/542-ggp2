#pragma once
#include <memory>
#include "Transform.h"
#include "Mesh.h"

class Entity
{
public:
	Entity(const char* name, Mesh mesh);
	~Entity();

	std::shared_ptr<Transform> GetTransform();
	std::shared_ptr<Mesh> GetMesh();
	void SetMesh(Mesh);
	const char* name;

	void Draw();
private:
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Mesh> mesh;
};