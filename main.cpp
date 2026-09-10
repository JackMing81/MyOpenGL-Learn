#include "Shader.h"

// Golbal Varibles
const float screenWidth = 800.0f;
const float screenHeight = 600.0f;

Camera ourCamera(0.0f, 0.0f, 7.0f, 0.0f, 1.0f, 1.0f, -90.0f, 0.0f);

// delta
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// view
float lastmouseX = 400.0f, lastmouseY = 300.0f;
static bool firstMouse = true;

static void processInput(GLFWwindow* window);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	// Initialize the GLFW
	if (!glfwInit())
	{
		std::cerr << "Faild to initializes the GLFW!" << "\n";
		return -1;
	}

	// Use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use core mode (Core-Profile)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		std::cerr << "Faild to create the window!" << "\n";
		glfwTerminate();
		return -1;
	}
	// VSync: Enable
	glfwSwapInterval(1);
	// No Cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Apply the context
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << "\n";
		glfwTerminate();
		return -1;
	}

	// set view port
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	// Call Back
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader lightingShader("Shaders/colors.vs", "Shaders/colors.fs");
	Shader lightCubeShader("Shaders/light_cube.vs", "Shaders/light_cube.fs");

	// Vertices data
	float vertices[] = {
		//        Position           Normal Vector		TexCoords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(0.2f, 0.2f, 0.6f),
		glm::vec3(0.3f, 0.3f, 0.7f),
		glm::vec3(0.0f, 0.0f, 0.3f),
		glm::vec3(0.4f, 0.4f, 0.4f)
	};

	// Vertex Buffer Objects (VBO)
	// Vertex Array Object (VAO)
	// Element Buffer Object (EBO)
	unsigned int cubeVAO = 0, VBO = 0;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	// Bind the Buffer
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Set vertex attrib pointer
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	unsigned int lightCubeVAO = 0;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Image*********************
	int width = 0, height = 0, nrChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Sources/Images/container2.png", &width, &height, &nrChannels, 0);
	unsigned int diffuseMap = 0, specularMap = 0;

	// Create 1 texture and bind
	glGenTextures(1, &diffuseMap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	// Use mirror-put mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// Use Nearest filter when smaller
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Use Linear filter when bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate
	if (data)
	{
		GLenum format = 0;
		if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format,
			width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Faild to load image!" << "\n";
		glfwTerminate();
		return -1;
	}

	// free
	stbi_image_free(data);

	// specularMap*************
	// Create 1 texture and bind
	glGenTextures(1, &specularMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	// Use mirror-put mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Use Nearest filter when smaller
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Use Linear filter when bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Sources/Images/container2_specular.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format = 0;
		if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format,
			width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Faild to load image!" << "\n";
		glfwTerminate();
		return -1;
	}
	// free
	stbi_image_free(data);

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update and Render
		// delta
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glm::mat4 projection = glm::perspective(glm::radians(ourCamera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);

		glm::mat4 view = ourCamera.GetViewMatrix();

		lightingShader.use();
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		// struct
		lightingShader.setInt("material.diffuse", 0);
		lightingShader.setInt("material.specular", 1);
		lightingShader.setFloat("material.shininess", 32.0f);

		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f); 
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.1f);
		lightingShader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("dirLight.specular", 0.1f, 0.1f, 0.1f);

		for (unsigned int i = 0; i < 4; i++)
		{
			std::string index = std::to_string(i);
			lightingShader.setVec3("pointLights[" + index + "].position", pointLightPositions[i]);
			//lightingShader.setVec3("pointLights[" + index + "].ambient", 0.05f, 0.05f, 0.05f);
			//lightingShader.setVec3("pointLights[" + index + "].diffuse", 0.5f, 0.5f, 0.5f);
			//lightingShader.setVec3("pointLights[" + index + "].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("pointLights[" + index + "].ambient", pointLightColors[i].x*0.1f
				, pointLightColors[i].y * 0.1f, pointLightColors[i].z * 0.1f);
			lightingShader.setVec3("pointLights[" + index + "].diffuse", pointLightColors[i].x
				, pointLightColors[i].y , pointLightColors[i].z );
			lightingShader.setVec3("pointLights[" + index + "].specular", pointLightColors[i].x
				, pointLightColors[i].y , pointLightColors[i].z );
			lightingShader.setFloat("pointLights[" + index + "].constant", 1.0f);
			lightingShader.setFloat("pointLights[" + index + "].linear", 0.09f);
			lightingShader.setFloat("pointLights[" + index + "].quadratic", 0.032f);
		}

		lightingShader.setVec3("spotLight.position", ourCamera.Position);
		lightingShader.setVec3("spotLight.direction", ourCamera.Front);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("spotLight.specular", 0.8f, 0.8f, 0.8f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.009f);
		lightingShader.setFloat("spotLight.quadratic", 0.0032f);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(27.5f)));

		
		lightingShader.setVec3("viewPos", ourCamera.Position.x,
			ourCamera.Position.y, ourCamera.Position.z);

		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glm::mat3 ourNormal = glm::mat3(glm::transpose(glm::inverse(model)));
			lightingShader.setMat3("ourNormal", ourNormal);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightCubeShader.use();
		glBindVertexArray(lightCubeVAO);

		lightCubeShader.setMat4("view", view);
		lightCubeShader.setMat4("projection", projection);
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightCubeShader.setMat4("model", model);
			lightCubeShader.setVec3("cubeColor", pointLightColors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Functions
static void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	// Dir
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastmouseX = static_cast<float>(xpos);
		lastmouseY = static_cast<float>(ypos);
		firstMouse = false;
	}
	float xoffset = static_cast<float>(xpos) - lastmouseX;
	float yoffset = lastmouseY - static_cast<float>(ypos);
	lastmouseX = static_cast<float>(xpos);
	lastmouseY = static_cast<float>(ypos);
	ourCamera.ProcessMouseMovement(xoffset, yoffset, true);

}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ourCamera.ProcessMouseScroll(static_cast<float>(yoffset));
}