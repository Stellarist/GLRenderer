#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "helper.hpp"

const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

float  yaw = -90.0f;
float  pitch = 0.0f;
float  last_x = SCR_WIDTH / 2.0f;
float  last_y = SCR_HEIGHT / 2.0f;
float  fov = 45.0f;
double delta_time = 0.0f;
double last_frame = 0.0f;
bool   first_mouse = true;

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_right = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

auto initWindow() -> GLFWwindow*;
void frameSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

int main(int argc, char const* argv[])
{
	auto* window = initWindow();

	std::println("Scene Initializing.");

	auto* scene = initDefaultScene();
	auto* camera = initDefaultCamera(*scene);
	auto* light = initDefaultLight(*scene);
	auto& asset_manager = AssetManager::instance();
	auto& graphics_manager = GraphicsManager::instance();

	auto* mesh = addMesh(*scene, SOURCE_DIR "/assets/nanosuit/nanosuit.obj", "Nanosuit");
	auto* mesh2 = addMesh(*scene, SOURCE_DIR "/assets/backpack/backpack.obj", "Backpack");
	auto* mesh3 = addMesh(*scene, SOURCE_DIR "/assets/backpack/backpack.obj", "Backpack");
	auto* shader = graphics_manager.uploadGLShader("ModelShader", SHADER_DIR "/default.vert", SHADER_DIR "/default.frag");

	auto& camera_transform = camera->getNode()->getTransform();
	auto& light_transform = light->getNode()->getTransform();
	auto& mesh_transform = mesh->getNode()->getTransform();
	auto& mesh2_transform = mesh2->getNode()->getTransform();
	auto& mesh3_transform = mesh3->getNode()->getTransform();

	mesh_transform.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	mesh_transform.setTranslation(glm::vec3(0.0f, -0.5f, 0.0f));
	mesh2_transform.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	mesh2_transform.setTranslation(glm::vec3(1.0f, -0.5f, 1.0f));
	mesh3_transform.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	mesh3_transform.setTranslation(glm::vec3(-1.0f, -0.5f, -1.0f));
	light_transform.setTranslation(glm::vec3(0.0f, 5.0f, 0.0f));

	std::println("Scene initialized.");

	glEnable(GL_DEPTH_TEST);

	bool first = true;
	while (!glfwWindowShouldClose(window)) {
		double current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera_transform.setTranslation(camera_pos);

		shader->use();
		shader->setMat4("model", mesh_transform.getWorldMatrix());
		shader->setMat4("view", camera->getView());
		shader->setMat4("projection", camera->getProjection());
		graphics_manager.drawMesh(*mesh, *shader);

		shader->use();
		shader->setMat4("model", mesh2_transform.getWorldMatrix());
		shader->setMat4("view", camera->getView());
		shader->setMat4("projection", camera->getProjection());
		graphics_manager.drawMesh(*mesh2, *shader);

		shader->use();
		shader->setMat4("model", mesh3_transform.getWorldMatrix());
		shader->setMat4("view", camera->getView());
		shader->setMat4("projection", camera->getProjection());
		graphics_manager.drawMesh(*mesh3, *shader);

		if (first) {
			printSceneNodes(*scene);
			printSceneComponents(*scene);
			printAssetManager();
			printGraphicsManager();
			first = false;
		}

		std::println("FPS: {:.2f}", 1.0f / delta_time);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

GLFWwindow* initWindow()
{
	if (!glfwInit()) {
		std::println("Failed to initialize GLFW.");
		throw std::runtime_error("Failed to initialize GLFW.");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLRenderer", nullptr, nullptr);
	if (window == nullptr) {
		std::println("Failed to create GLFW window.");
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window.");
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::println("Failed to initialize GLAD.");
		throw std::runtime_error("Failed to initialize GLAD.");
	}

	return window;
}

void frameSizeCallback(GLFWwindow* window, int width, int height)
{
	const_cast<int&>(SCR_WIDTH) = width;
	const_cast<int&>(SCR_HEIGHT) = height;
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos_in, double ypos_in)
{
	auto xpos = static_cast<float>(xpos_in);
	auto ypos = static_cast<float>(ypos_in);

	if (first_mouse) {
		last_x = xpos;
		last_y = ypos;
		first_mouse = false;
	}

	float xoffset = xpos - last_x;
	float yoffset = last_y - ypos;
	last_x = xpos;
	last_y = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front{
	    cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
	    sin(glm::radians(pitch)),
	    sin(glm::radians(yaw)) * cos(glm::radians(pitch))};
	camera_front = glm::normalize(front);
	camera_right = glm::normalize(glm::cross(camera_front, camera_up));
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= static_cast<float>(yoffset);
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float camera_speed = 2.5f * static_cast<float>(delta_time);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= camera_right * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += camera_right * camera_speed;
}
