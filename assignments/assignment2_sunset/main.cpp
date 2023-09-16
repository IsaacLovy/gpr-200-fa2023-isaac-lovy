#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ILGL/shader.h>

unsigned int createShader(GLenum shaderType, const char* sourceCode);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO(unsigned int* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex
{
	float x, y, z;
	float u, v;
};

Vertex vertices[4] = {
	   //x   //y  //z  //u  //v
	{ -1.0, -1.0, 0.0, 0.0, 0.0 },
	{  1.0, -1.0, 0.0, 1.0, 0.0 },
	{  1.0,  1.0, 0.0, 1.0, 1.0 },
	{ -1.0,  1.0, 0.0, 0.0, 1.0 }
};

unsigned int indices[6] = {
	0, 1, 2, //tri 1
	2, 3, 0  //tri 2
};

float skyColor[3] = { 0.37, 0.81, 0.95 };
float groundColor[3] = { 0.87, 0.52, 0 };
float bgMixColor[3] = { 0.2,0,0.2 };
float sunColor[4] = { 1.0, .78, 0.06, 1.0 };
float skyLineColor[3] = { 0,0,0 };
float sunRadius = 0.35f;
float timeScale = 0.5;
bool showImGUIDemoWindow = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	ilgl::Shader shader ("assets/vertexShader.vert", "assets/fragmentShader.frag");
	unsigned int vao = createVAO((unsigned int*)vertices, 4, indices, 6);

	shader.use();
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		shader.setVec2("_WindowSize", SCREEN_WIDTH, SCREEN_HEIGHT);
		shader.setFloat("_Time", glfwGetTime());
		shader.setFloat("_TimeScale", timeScale);
		shader.setVec3("_SkyCol", skyColor[0], skyColor[1], skyColor[2]);
		shader.setVec3("_GroundCol", groundColor[0], groundColor[1], groundColor[2]);
		shader.setVec3("_BGMixCol", bgMixColor[0], bgMixColor[1], bgMixColor[2]);
		shader.setVec4("_SunCol", sunColor[0], sunColor[1], sunColor[2], sunColor[3]);
		shader.setVec3("_SkylineCol", skyLineColor[0], skyLineColor[1], skyLineColor[2]);
		shader.setFloat("_SunRadius", sunRadius);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Sky Color", skyColor);
			ImGui::ColorEdit3("Ground Color", groundColor);
			ImGui::ColorEdit4("Sun Color", sunColor);
			ImGui::ColorEdit3("skyline Color", skyLineColor);
			ImGui::SliderFloat("Sun Radius", &sunRadius, 0.0f, 1.0f);
			ImGui::DragFloat("Time Scale", &timeScale, 0.25f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(unsigned int* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	////Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 5, vertexData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,x));
	glEnableVertexAttribArray(0);

	//UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

