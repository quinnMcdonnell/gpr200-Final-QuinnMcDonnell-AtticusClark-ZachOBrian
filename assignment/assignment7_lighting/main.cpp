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
	ew::Vec3 position; //World space
	ew::Vec3 color; //RGB
};

struct Material 
{
	float ambientK; //Ambient coefficient (0-1)
	float diffuseK; //Diffuse coefficient (0-1)
	float specular; //Specular coefficient (0-1)
	float shininess; //Shininess
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
	ew::Shader unlitShader("assets/unlit.vert", "assets/unlit.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg",GL_REPEAT,GL_LINEAR);

	//Create cube
	ew::Mesh cubeMesh(ew::createCube(1.0f));
	ew::Mesh planeMesh(ew::createPlane(5.0f, 5.0f, 10));
	ew::Mesh sphereMesh(ew::createSphere(0.5f, 64));
	ew::Mesh cylinderMesh(ew::createCylinder(0.5f, 1.0f, 32));

	ew::Mesh unlitsphereMeshR(ew::createSphere(0.125f, 32));
	ew::Mesh unlitsphereMeshG(ew::createSphere(0.125f, 32));
	ew::Mesh unlitsphereMeshY(ew::createSphere(0.125f, 32));
	ew::Mesh unlitsphereMeshB(ew::createSphere(0.125f, 32));


	//Initialize transforms
	ew::Transform cubeTransform;
	ew::Transform planeTransform;
	ew::Transform sphereTransform;
	ew::Transform cylinderTransform;
	planeTransform.position = ew::Vec3(0, -1.0, 0);
	sphereTransform.position = ew::Vec3(-1.5f, 0.0f, 0.0f);
	cylinderTransform.position = ew::Vec3(1.5f, 0.0f, 0.0f);

	ew::Transform unlitRed;
	ew::Transform unlitGreen;
	ew::Transform unlitYellow;
	ew::Transform unlitBlue;


	//Light Array
	Light _lights[4];
	int lights = 4;
	_lights[0].color = ew::Vec3(1,0,0);
	_lights[0].position = ew::Vec3(5,1,0);
	unlitRed.position = _lights[0].position;

	_lights[1].color = ew::Vec3(0, 1, 0);
	_lights[1].position = ew::Vec3(5, 1, -5);
	unlitGreen.position = _lights[1].position;

	_lights[2].color = ew::Vec3(1, 1, 0);
	_lights[2].position = ew::Vec3(0, 1, 5);
	unlitYellow.position = _lights[2].position;

	_lights[3].color = ew::Vec3(0, 0, 1);
	_lights[3].position = ew::Vec3(-5, 1, 5);
	unlitBlue.position = _lights[3].position;

	Material _material;
	_material.ambientK = 0.2;
	_material.diffuseK = 0.5;
	_material.specular = 0.5;
	_material.shininess = 128;

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

		//TODO: Render point lights
		shader.setVec3("_Lights[0].position", _lights[0].position);
		shader.setVec3("_Lights[0].color", _lights[0].color);

		shader.setVec3("_Lights[1].position", _lights[1].position);
		shader.setVec3("_Lights[1].color", _lights[1].color);

		shader.setVec3("_Lights[2].position", _lights[2].position);
		shader.setVec3("_Lights[2].color", _lights[2].color);

		shader.setVec3("_Lights[3].position", _lights[3].position);
		shader.setVec3("_Lights[3].color", _lights[3].color);

		shader.setInt("numLights", lights);

		shader.setFloat("_Material.ambientK", _material.ambientK);
		shader.setFloat("_Material.diffuseK", _material.diffuseK);
		shader.setFloat("_Material.specular", _material.specular);
		shader.setFloat("_Material.shininess", _material.shininess);

		shader.setVec3("_CameraPosition", camera.position);

		//Draw shapes
		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();

		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();

		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();


		unlitShader.use();

		unlitShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		
		unlitShader.setMat4("_Model", unlitRed.getModelMatrix());
		unlitShader.setVec3("_Color",_lights[0].color);
		unlitsphereMeshR.draw();

		unlitShader.setMat4("_Model", unlitGreen.getModelMatrix());
		unlitShader.setVec3("_Color", _lights[1].color);
		unlitsphereMeshG.draw();

		unlitShader.setMat4("_Model", unlitYellow.getModelMatrix());
		unlitShader.setVec3("_Color", _lights[2].color);
		unlitsphereMeshY.draw();

		unlitShader.setMat4("_Model", unlitBlue.getModelMatrix());
		unlitShader.setVec3("_Color", _lights[3].color);
		unlitsphereMeshB.draw();

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
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

			ImGui::ColorEdit3("BG color", &bgColor.x);

			if (ImGui::CollapsingHeader("Material"))
			{
				ImGui::DragFloat("Amibence",&_material.ambientK,0.01f,0.0f,1.0f);
				ImGui::DragFloat("Diffuse", &_material.diffuseK, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Specular", &_material.specular, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Shininess", &_material.shininess, 1.0f, 2.0f, 500.0f);
			}
			ImGui::DragInt("Number of Lights", &lights, 1, 0, 4);
			
			if (ImGui::CollapsingHeader("Light"))
			{
				ImGui::DragFloat3("Color", &_lights[0].color.x, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat3("Position", &_lights[0].position.x, 0.5f);
				unlitRed.position = _lights[0].position;
			}

			if (ImGui::CollapsingHeader("Light 2"))
			{
				ImGui::DragFloat3("Color", &_lights[1].color.x, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat3("Position", &_lights[1].position.x, 0.5f);
				unlitGreen.position = _lights[1].position;
			}

			if (ImGui::CollapsingHeader("Light 3"))
			{
				ImGui::DragFloat3("Color", &_lights[2].color.x, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat3("Position", &_lights[2].position.x, 0.5f);
				unlitYellow.position = _lights[2].position;
			}

			if (ImGui::CollapsingHeader("Light 4"))
			{
				ImGui::DragFloat3("Color", &_lights[3].color.x, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat3("Position", &_lights[3].position.x, 0.5f);
				unlitBlue.position = _lights[3].position;
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


