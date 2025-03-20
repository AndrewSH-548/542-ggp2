#include "Entity.h"
#include "Graphics.h"

using namespace std;

Entity::Entity(const char* name, Mesh mesh, Material material) {
	this->transform = make_shared<Transform>(Transform());
	this->mesh = make_shared<Mesh>(mesh);
	this->material = make_shared<Material>(material);
}

void Entity::Draw() {
	Graphics::CommandList->SetPipelineState(material->GetPipelineState().Get());
	Graphics::CommandList->SetGraphicsRootDescriptorTable(2, material->GetFinalGPUHandleForSRVs());
	mesh->Draw();
}

Entity::~Entity() {
}

void Entity::SetMesh(Mesh mesh) {
	this->mesh = make_shared<Mesh>(mesh);
}

void Entity::SetMaterial(Material material) {
	this->material = make_shared<Material>(material);
}