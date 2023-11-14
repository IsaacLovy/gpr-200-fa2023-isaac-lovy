#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

struct Light
{
	ew::Vec3 position;
	ew::Vec3 color;
};

struct Material
{
	float ambientK;
	float diffuseK;
	float specular;
	float shininess;
};

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg",GL_REPEAT,GL_LINEAR);

	ew::Shader lightShader("assets/unlit.vert", "assets/unlit.frag");

	//Create cube
	ew::Mesh cubeMesh(ew::createCube(1.0f));
	ew::Mesh planeMesh(ew::createPlane(5.0f, 5.0f, 10));
	ew::Mesh sphereMesh(ew::createSphere(0.5f, 64));
	ew::Mesh cylinderMesh(ew::createCylinder(0.5f, 1.0f, 32));

	//Initialize transforms
	ew::Transform cubeTransform;
	ew::Transform planeTransform;
	ew::Transform sphereTransform;
	ew::Transform cylinderTransform;
	planeTransform.position = ew::Vec3(0, -1.0, 0);
	sphereTransform.position = ew::Vec3(-1.5f, 0.0f, 0.0f);
	cylinderTransform.position = ew::Vec3(1.5f, 0.0f, 0.0f);

	Material brickMaterial;
	brickMaterial.ambientK = 0.05f;
	brickMaterial.diffuseK = 0.75f;
	brickMaterial.specular = 0.2f;
	brickMaterial.shininess = 16;

	Light lights[4];
	ew::Transform lightTransforms[4];

	int numLights = 4;

	lights[0].color = ew::Vec3(1, 1, 1);
	lights[0].position = ew::Vec3(-5, 5, -5);
	lightTransforms[0].position = ew::Vec3(-5, 5, -5);
	lightTransforms[0].scale = ew::Vec3(0.3f, 0.3f, 0.3f);

	lights[1].color = ew::Vec3(1, 1, 1);
	lights[1].position = ew::Vec3(-5, 5, 5);
	lightTransforms[1].position = ew::Vec3(-5, 5, 5);
	lightTransforms[1].scale = ew::Vec3(0.3f, 0.3f, 0.3f);

	lights[2].color = ew::Vec3(1, 1, 1);
	lights[2].position = ew::Vec3(5, 5, 5);
	lightTransforms[2].position = ew::Vec3(5, 5, 5);
	lightTransforms[2].scale = ew::Vec3(0.3f, 0.3f, 0.3f);

	lights[3].color = ew::Vec3(1, 1, 1);
	lights[3].position = ew::Vec3(5, 5, -5);
	lightTransforms[3].position = ew::Vec3(5, 5, -5);
	lightTransforms[3].scale = ew::Vec3(0.3f, 0.3f, 0.3f);

	resetCamera(camera,cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		glClearColor(bgColor.x, bgColor.y,bgColor.z,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		shader.setVec3("_CameraPos", camera.position);

		for (int i = 0; i < 4; i++)
		{
			shader.setVec3("_Lights[" + std::to_string(i) + "].position", lights[i].position);

			if (i < numLights) shader.setVec3("_Lights[" + std::to_string(i) + "].color", lights[i].color);
			else shader.setVec3("_Lights[" + std::to_string(i) + "].color", ew::Vec3(0,0,0));
		}

		shader.setFloat("_Shininess", brickMaterial.shininess);
		shader.setFloat("_DiffuseK", brickMaterial.diffuseK);
		shader.setFloat("_SpecularK", brickMaterial.specular);
		shader.setFloat("_AmbientK", brickMaterial.ambientK);

		//Draw shapes
		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();

		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();

		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();

		//TODO: Render point lights
		lightShader.use();
		lightShader.setMat4("_ViewProjection", camera.ProjectionMatrix()* camera.ViewMatrix());

		for (int i = 0; i < numLights; i++)
		{
			lightShader.setVec3("_Color", lights[i].color);
			lightShader.setMat4("_Model", lightTransforms[i].getModelMatrix());
			sphereMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::ColorEdit3("BG color", &bgColor.x);
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}

			if (ImGui::CollapsingHeader("Material"))
			{
				ImGui::SliderFloat("AmbientK", &brickMaterial.ambientK, 0, 1);
				ImGui::SliderFloat("DiffuseK", &brickMaterial.diffuseK, 0, 1);
				ImGui::SliderFloat("SpecularK", &brickMaterial.specular, 0, 1);
				ImGui::SliderFloat("Shininess", &brickMaterial.shininess, 2, 1000);
			}

			if (ImGui::CollapsingHeader("Lights"))
			{
				ImGui::SliderInt("Number Lights", &numLights, 1, 4);

				for (int i = 0; i < numLights; i++)
				{
					ImGui::PushID(i);
					if (ImGui::CollapsingHeader(("Light " + std::to_string(i)).c_str()))
					{
						ImGui::DragFloat3("Position", &lights[i].position.x, 0.05f);
						ImGui::ColorPicker3("Color", &lights[i].color.x, 0.01f);
					}
					ImGui::PopID();
				}
			}

			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}


