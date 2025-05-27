#pragma once

#include "base/Scene.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "resource/AssetManager.hpp"
#include "resource/GraphicsManager.hpp"

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

Scene* initDefaultScene()
{
	auto scene = std::make_shared<Scene>(0, "MainScene");
	AssetManager::instance().setScene(scene);
	GraphicsManager::instance().setScene(scene);

	return scene.get();
}

Camera* initDefaultCamera(Scene& scene)
{
	auto camera_node = std::make_unique<Node>(0, "MainCamera");
	auto camera_component = std::make_unique<PerspectiveCamera>("MainCamera");
	camera_component->setAspectRatio((float) SCR_WIDTH / (float) SCR_HEIGHT);
	camera_component->setFov(glm::radians(45.0f));
	camera_component->setNearPlane(0.1f);
	camera_component->setFarPlane(1000.0f);
	camera_node->getTransform().setTranslation(glm::vec3(0.0f, 0.0f, 3.0f));

	Camera* camera_ptr = camera_component.get();
	camera_component->setNode(*camera_node);
	camera_node->setComponent(*camera_component);
	scene.addComponent(std::move(camera_component));
	scene.getRoot().addChild(*camera_node);
	scene.addNode(std::move(camera_node));

	return camera_ptr;
}

Light* initDefaultLight(Scene& scene)
{
	auto light_node = std::make_unique<Node>(0, "Light");
	auto light_component = std::make_unique<DirectionalLight>("Light");
	light_component->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	light_component->setIntensity(1.0f);
	light_component->setDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	light_node->getTransform().setTranslation(glm::vec3(0.0f, 5.0f, 0.0f));

	Light* light_ptr = light_component.get();
	light_component->setNode(*light_node);
	light_node->setComponent(*light_component);
	scene.addComponent(std::move(light_component));
	scene.getRoot().addChild(*light_node);
	scene.addNode(std::move(light_node));

	return light_ptr;
}

Mesh* addMesh(Scene& scene, const std::string& model_path, const std::string& model_name)
{
	AssetManager& asset_manager = AssetManager::instance();

	auto* mesh = asset_manager.loadMesh(model_path, model_name);
	if (!mesh) {
		std::println("Failed to load mesh from path: {}", model_path);
		return nullptr;
	}

	auto* mesh_node = mesh->getNode();
	if (mesh_node) {
		auto& transform = mesh_node->getTransform();
		transform.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
		transform.setTranslation(glm::vec3(0.0f, -0.5f, 0.0f));
		transform.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	return mesh;
}

void checkGLError(const char* file, int line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::println("OpenGL error in {} at line {}: {}", file, line, error);
	}
}

void printSceneNodes(const Scene& scene)
{
	std::println("\n==================== Scene Nodes Tree ====================");
	std::println("Scene: {}", scene.getName());

	auto printNodeTree = [&](this auto&& printNodeTree, Node* node, const std::string& prefix = "", bool isLast = true) {
		if (!node)
			return;

		std::string nodeIcon = isLast ? "└── " : "├── ";
		std::string childPrefix = prefix + (isLast ? "    " : "│   ");
		std::println("{}{}[{}] {} (ID: {})", prefix, nodeIcon, node->getType().name(), node->getName(), node->getId());

		if (node->hasComponent<Transform>()) {
			const auto& transform = node->getComponent<Transform>();
			const auto& pos = transform.getTranslation();
			const auto& scale = transform.getScale();
			std::println("{}├── Transform: pos({:.2f}, {:.2f}, {:.2f}) scale({:.2f}, {:.2f}, {:.2f})",
			             childPrefix, pos.x, pos.y, pos.z, scale.x, scale.y, scale.z);
		}

		if (node->hasComponent<Camera>()) {
			const auto& camera = node->getComponent<Camera>();
			std::println("{}├── Camera: {}", childPrefix, camera.getName());
		}

		if (node->hasComponent<Light>()) {
			const auto& light = node->getComponent<Light>();
			const auto& color = light.getColor();
			std::println("{}├── Light: {} - color({:.2f}, {:.2f}, {:.2f}) intensity({:.2f})",
			             childPrefix, light.getName(), color.r, color.g, color.b, light.getIntensity());
		}

		if (node->hasComponent<Mesh>()) {
			const auto& mesh = node->getComponent<Mesh>();
			std::println("{}├── SubMesh: {} - submeshes({})",
			             childPrefix, mesh.getName(), mesh.getSubmeshes().size());
		}

		const auto& children = node->getChildren();
		for (size_t i = 0; i < children.size(); ++i) {
			bool isLastChild = (i == children.size() - 1);
			printNodeTree(children[i], childPrefix, isLastChild);
		}
	};

	const Node& root = const_cast<Scene&>(scene).getRoot();
	if (root.getChildren().empty()) {
		std::println("└── (No child nodes)");
		return;
	}

	const auto& children = root.getChildren();
	for (size_t i = 0; i < children.size(); ++i) {
		bool isLast = (i == children.size() - 1);
		printNodeTree(children[i], "", isLast);
	}

	std::println("=========================================================\n");
}

void printSceneComponents(const Scene& scene)
{
	std::println("\n================== Scene Components ==================");
	std::println("Scene: {}", scene.getName());

	auto getComponentTypeName = [](const std::type_index& type) {
		std::string typeName = type.name();
		if (typeName.find("Transform") != std::string::npos)
			return "Transform";
		if (typeName.find("PerspectiveCamera") != std::string::npos)
			return "PerspectiveCamera";
		if (typeName.find("OrthoCamera") != std::string::npos)
			return "OrthoCamera";
		if (typeName.find("Camera") != std::string::npos)
			return "Camera";
		if (typeName.find("DirectionalLight") != std::string::npos)
			return "DirectionalLight";
		if (typeName.find("PointLight") != std::string::npos)
			return "PointLight";
		if (typeName.find("SpotLight") != std::string::npos)
			return "SpotLight";
		if (typeName.find("Light") != std::string::npos)
			return "Light";
		if (typeName.find("SubMesh") != std::string::npos)
			return "SubMesh";
		if (typeName.find("Mesh") != std::string::npos)
			return "Mesh";
		if (typeName.find("PBRMaterial") != std::string::npos)
			return "PBRMaterial";
		if (typeName.find("Material") != std::string::npos)
			return "Material";
		if (typeName.find("Texture") != std::string::npos)
			return "Texture";
		if (typeName.find("Image") != std::string::npos)
			return "Image";
		if (typeName.find("AABB") != std::string::npos)
			return "AABB";
		return "";
	};

	std::vector<std::type_index> componentTypes = {
	    typeid(Transform),
	    typeid(PerspectiveCamera),
	    typeid(OrthoCamera),
	    typeid(DirectionalLight),
	    typeid(PointLight),
	    typeid(SpotLight),
	    typeid(Mesh),
	    typeid(SubMesh),
	    typeid(PBRMaterial),
	    typeid(Material),
	    typeid(Texture),
	    typeid(Image),
	    typeid(AABB)};

	bool hasAnyComponents = false;

	for (const auto& type : componentTypes) {
		if (scene.hasComponent(type)) {
			hasAnyComponents = true;
			const auto& components = scene.getComponents(type);
			std::string typeName = getComponentTypeName(type);

			std::println("├── {} ({})", typeName, components.size());

			for (size_t i = 0; i < components.size(); ++i) {
				const auto& component = components[i];
				bool        isLast = (i == components.size() - 1);
				std::string itemIcon = isLast ? "│   └── " : "│   ├── ";

				std::println("{}[{}] {} (UID: {})", itemIcon,
				             getComponentTypeName(component->getType()),
				             component->getName(),
				             component->getUid());

				if (auto* camera = dynamic_cast<PerspectiveCamera*>(component.get())) {
					std::println("{}    FOV: {:.1f}°, Aspect: {:.2f}, Near: {:.2f}, Far: {:.2f}",
					             isLast ? "        " : "│       ",
					             glm::degrees(camera->getFov()), camera->getAspectRatio(),
					             camera->getNearPlane(), camera->getFarPlane());
				} else if (auto* light = dynamic_cast<DirectionalLight*>(component.get())) {
					const auto& color = light->getColor();
					const auto& dir = light->getDirection();
					std::println("{}    Color: ({:.2f}, {:.2f}, {:.2f}), Intensity: {:.2f}",
					             isLast ? "        " : "│       ",
					             color.r, color.g, color.b, light->getIntensity());
					std::println("{}    Direction: ({:.2f}, {:.2f}, {:.2f})",
					             isLast ? "        " : "│       ",
					             dir.x, dir.y, dir.z);
				} else if (auto* mesh = dynamic_cast<Mesh*>(component.get())) {
					std::println("{}    Submeshes: {}",
					             isLast ? "        " : "│       ",
					             mesh->getSubmeshes().size());
				} else if (auto* submesh = dynamic_cast<SubMesh*>(component.get())) {
					std::println("{}    Vertices: {}, Indices: {}, Visible: {}",
					             isLast ? "        " : "│       ",
					             submesh->getVerticesCount(), submesh->getIndicesCount(),
					             submesh->isVisible() ? "Yes" : "No");
				} else if (auto* material = dynamic_cast<PBRMaterial*>(component.get())) {
					const auto& baseColor = material->getBaseColorFactor();
					std::println("{}    BaseColor: ({:.2f}, {:.2f}, {:.2f}, {:.2f})",
					             isLast ? "        " : "│       ",
					             baseColor.r, baseColor.g, baseColor.b, baseColor.a);
					std::println("{}    Metallic: {:.2f}, Roughness: {:.2f}",
					             isLast ? "        " : "│       ",
					             material->getMetallicFactor(), material->getRoughnessFactor());
				} else if (auto* image = dynamic_cast<Image*>(component.get())) {
					std::println("{}    Size: {}x{}, Format: {}, Data: {} bytes",
					             isLast ? "        " : "│       ",
					             image->getWidth(), image->getHeight(), image->getFormat(),
					             image->getData().size());
				}
			}
		}
	}

	if (!hasAnyComponents) {
		std::println("└── (No components found)");
	}

	std::println("=====================================================\n");
}

void printAssetManager()
{
	std::println("\n================== Asset Manager ==================");
	auto& asset_manager = AssetManager::instance();

	std::println("Loaded Meshes: {}", asset_manager.loaded_meshes.size());
	for (const auto& [name, mesh] : asset_manager.loaded_meshes)
		std::println("  - {} (UID: {})", name, mesh->getUid());

	std::println("Loaded SubMeshes: {}", asset_manager.loaded_submeshes.size());
	for (const auto& [name, submesh] : asset_manager.loaded_submeshes)
		std::println("  - {} (UID: {})", name, submesh->getUid());

	std::println("Loaded Images: {}", asset_manager.loaded_images.size());
	for (const auto& [name, image] : asset_manager.loaded_images)
		std::println("  - {} (UID: {})", name, image->getUid());

	std::println("Loaded Textures: {}", asset_manager.loaded_textures.size());
	for (const auto& [name, texture] : asset_manager.loaded_textures)
		std::println("  - {} (UID: {})", name, texture->getUid());

	std::println("Loaded Materials: {}", asset_manager.loaded_materials.size());
	for (const auto& [name, material] : asset_manager.loaded_materials)
		std::println("  - {} (UID: {})", name, material->getUid());

	std::println("=====================================================\n");
}

void printGraphicsManager()
{
	std::println("\n================== Graphics Manager ==================");
	auto& graphics_manager = GraphicsManager::instance();

	std::println("GL Meshes: {}", graphics_manager.gl_meshes.size());
	for (const auto& [mesh, glmeshes] : graphics_manager.gl_meshes) {
		std::println("  - Mesh UID: {} - GL Meshes: {}", mesh->getUid(), glmeshes.size());
		for (const auto& glmesh : glmeshes) {
			std::println("    - GLMesh Name: {}", glmesh.getSubMesh()->getName());
		}
	}

	std::println("GL Shaders: {}", graphics_manager.gl_shaders.size());
	for (const auto& [name, gl_shader] : graphics_manager.gl_shaders)
		std::println("  - {} (ID: {})", name, gl_shader->getId());

	std::println("GL Textures: {}", graphics_manager.gl_textures.size());
	for (const auto& [name, gl_texture] : graphics_manager.gl_textures)
		std::println("  - {} (ID: {})", name, gl_texture->getId());

	std::println("=====================================================\n");
}
