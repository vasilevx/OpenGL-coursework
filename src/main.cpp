
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include "model.h"
const unsigned int screen_width = 1600;
const unsigned int screen_height = 900;
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	GLenum format = GL_RGB;
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);
	return textureID;
}

unsigned int loadCubemap(std::vector <std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 4);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}

int main(int argc, char* argv[]) {
	FreeConsole();
	int width, height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "I967 Vasilev", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();
	glEnable(GL_DEPTH_TEST);

	Shader myShader("../src/vertexShader.vert", "../src/fragmentShader.frag");
	Shader skyboxShader("../src/skyBox.vert", "../src/skyBox.frag");
	Model blueCampfire("../assets/models/blue_campfire/blue_campfire.obj");
	Model firecamp("../assets/models/firecamp/firecamp.obj");
	Model redCampfire("../assets/models/red_campfire/red_campfire.obj");
	Model dwarf("../assets/models/dwarf/dwarf.obj");
	Model demonForm("../assets/models/demonForm/demonForm.obj");
	Model orb("../assets/models/orb/orb.obj");
	Model portal("../assets/models/portal/portal.obj");
	Model shooter("../assets/models/shooter/shooter.obj");
	Model mage("../assets/models/mage/mage.obj");
	Model frostbolt("../assets/models/frostbolt/frostbolt.obj");

	float vertices[] = {
		// вершины          // грани      // текстуры
		-10.5f, -0.1f, -10.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 10.5f, -0.1f, -10.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 10.5f,  0.0f, -10.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 10.5f,  0.0f, -10.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-10.5f,  0.0f, -10.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-10.5f, -0.1f, -10.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-10.5f, -0.1f,  10.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 10.5f, -0.1f,  10.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 10.5f,  0.0f,  10.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 10.5f,  0.0f,  10.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-10.5f,  0.0f,  10.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-10.5f, -0.1f,  10.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-10.5f,  0.0f,  10.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-10.5f,  0.0f, -10.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-10.5f, -0.1f, -10.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-10.5f, -0.1f, -10.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-10.5f, -0.1f,  10.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-10.5f,  0.0f,  10.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 10.5f,  0.0f,  10.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 10.5f,  0.0f, -10.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 10.5f, -0.1f, -10.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 10.5f, -0.1f, -10.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 10.5f, -0.1f,  10.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 10.5f,  0.0f,  10.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-10.5f, -0.1f, -10.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 10.5f, -0.1f, -10.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 10.5f, -0.1f,  10.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 10.5f, -0.1f,  10.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-10.5f, -0.1f,  10.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-10.5f, -0.1f, -10.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-10.5f,  0.0f, -10.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 10.5f,  0.0f, -10.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 10.5f,  0.0f,  10.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 10.5f,  0.0f,  10.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-10.5f,  0.0f,  10.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-10.5f,  0.0f, -10.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint texture1 = loadTexture("../assets/textures/ground.jpg");
	std::vector <std::string> faces {
		"../assets/textures/mercury_ft.jpg",
		"../assets/textures/mercury_bk.jpg",
		"../assets/textures/mercury_up.jpg",
		"../assets/textures/mercury_dn.jpg",
		"../assets/textures/mercury_rt.jpg",
		"../assets/textures/mercury_lf.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	myShader.use();
	myShader.setInt("material.diffuse", 0);
	myShader.setInt("material.specular", 0);	

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		myShader.use();

		myShader.setVec3("viewPos", camera.Position);
		myShader.setFloat("material.shininess", 32.0f);
		myShader.setVec3("dirLight.direction", 0.0f, 1.0f, 0.0f);
		myShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
		myShader.setVec3("dirLight.diffuse", 1.0f, 0.42f, 0.0f);
		myShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

		glm::vec3 pointLightPositions[] = {
			glm::vec3(
			    10 * cos(glfwGetTime() * 0.5),
				3.0,
				10 * sin(glfwGetTime()* 0.5)
			),
			glm::vec3(
				9.5f,
				0.0f,
				-9.5f
			),
			glm::vec3(
				9.5f,
				0.0f,
				9.5f
			),
			glm::vec3(
				-9.5f,
				0.0f,
				-9.5f
			)
		};
		myShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		myShader.setVec3("pointLights[0].ambient", 255.0f / 100.0f, 246.0f/ 100.0f, 91.0f / 100.0f);
		myShader.setVec3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
		myShader.setVec3("pointLights[0].specular", 255.0f / 100.0f, 246.0f / 100.0f, 91.0f / 100.0f);
		myShader.setFloat("pointLights[0].constant", 1.0f);
		myShader.setFloat("pointLights[0].linear", 0.22);
		myShader.setFloat("pointLights[0].quadratic", 0.20);

		myShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		myShader.setVec3("pointLights[1].ambient", 0.0f, 1.0f, 0.0f);
		myShader.setVec3("pointLights[1].diffuse", 0.3f, 0.3f, 0.3f);
		myShader.setVec3("pointLights[1].specular", 0.0f, 1.0f, 0.0f);
		myShader.setFloat("pointLights[1].constant", 1.0f);
		myShader.setFloat("pointLights[1].linear", 0.07);
		myShader.setFloat("pointLights[1].quadratic", 0.017);

		myShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		myShader.setVec3("pointLights[2].ambient", 1.0f, 0.0f, 0.0f);
		myShader.setVec3("pointLights[2].diffuse", 0.3f, 0.3f, 0.3f);
		myShader.setVec3("pointLights[2].specular", 1.0f, 0.0f, 0.0f);
		myShader.setFloat("pointLights[2].constant", 1.0f);
		myShader.setFloat("pointLights[2].linear", 0.07);
		myShader.setFloat("pointLights[2].quadratic", 0.017);

		myShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		myShader.setVec3("pointLights[3].ambient", 0.0f, 0.0f, 1.0f);
		myShader.setVec3("pointLights[3].diffuse", 0.3f, 0.3f, 0.3f);
		myShader.setVec3("pointLights[3].specular", 0.0f, 0.0f, 1.0f);
		myShader.setFloat("pointLights[3].constant", 1.0f);
		myShader.setFloat("pointLights[3].linear", 0.07);
		myShader.setFloat("pointLights[3].quadratic", 0.017);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
		myShader.setMat4("model", model);
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);

		model = glm::translate(model, pointLightPositions[1]);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		myShader.setMat4("model", model);
		firecamp.Draw(myShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[2]);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		myShader.setMat4("model", model);
		redCampfire.Draw(myShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::rotate(model, float(glfwGetTime() * 2.0), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		myShader.setMat4("model", model);
		orb.Draw(myShader);

        model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[3]);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		myShader.setMat4("model", model);
		blueCampfire.Draw(myShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.5f, -2.0f, -5.0f));
		model = glm::rotate(model, float(glm::radians(135.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		myShader.setMat4("model", model);
		demonForm.Draw(myShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 5.0f));
		model = glm::rotate(model, float(glm::radians(163.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", model);
		shooter.Draw(myShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -4.0f));
		model = glm::rotate(model, float(glm::radians(95.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", model);
		mage.Draw(myShader);
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 9.0f));
		myShader.setMat4("model", model);
		frostbolt.Draw(myShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.0f, 0.0f, -7.0f));
		model = glm::rotate(model, float(glm::radians(-45.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", model);
		dwarf.Draw(myShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.8f, 0.0f, 4.8f));
		model = glm::rotate(model, float(glm::radians(135.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		myShader.setMat4("model", model);
		portal.Draw(myShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		myShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LEQUAL); 
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); 
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &skyboxVAO);
	glfwTerminate();
	return 0;
}