#include "Entity.h"

using namespace std;

Entity::Entity(const char* name, Mesh mesh) {
	this->transform = make_shared<Transform>(Transform());
	this->mesh = make_shared<Mesh>(mesh);
}

void Entity::Draw() {
	mesh->Draw();
}

Entity::~Entity() {
}

shared_ptr<Transform> Entity::GetTransform() {
	return transform;
}

shared_ptr<Mesh> Entity::GetMesh() {
	return mesh;
}

void Entity::SetMesh(Mesh mesh) {
	this->mesh = make_shared<Mesh>(mesh);
}