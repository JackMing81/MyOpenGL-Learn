#include "Shader.h"

// Golbal Varibles
const float screenWidth  = 800.0f;
const float screenHeight = 600.0f;


Camera ourCamera(0.0f, 0.0f, 3.0f, 0.0f, 1.0f, 1.0f, -90.0f, 0.0f);


// delta
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// view
float lastmouseX = 400.0f, lastmouseY = 300.0f;
static bool firstMouse = true;

// texture mix
float mixlevel = 0.0f;

// Functions
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


	Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

	// Vertices data
	//float vertices[] = {
	//	//         Position             Color         texture coords
	//		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Right Up
	//		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Right Down
	//		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Left Down
	//		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Left Up
	//};

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//unsigned int indices[] = {
	//	0, 1, 3,	// first
	//	1, 2, 3		// second
	//};

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

	// Vertex Buffer Objects (VBO)
	// Vertex Array Object (VAO)
	// Element Buffer Object (EBO)
	unsigned int VAO = 0, VBO = 0;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);


	// Set vertex attrib pointer
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// Image*********************
	int width = 0, height = 0, nrChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Sources/Images/image2.jpg", &width, &height, &nrChannels, 0);
	unsigned int texture1, texture2;

	// Create 1 texture and bind
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

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
		std::cout << nrChannels << "\n";
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

	// Texture2*************
	// Create 1 texture and bind
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Use mirror-put mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Use Nearest filter when smaller
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Use Linear filter when bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Sources/Images/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		std::cout << nrChannels << "\n";
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

	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	ourShader.setFloat("mixlevel", mixlevel);

	// MVP Matirx
	// Create a view-matrix
	glm::mat4 view(1.0f);

	// Create a projection-matrix
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);


	// Set
	view = glm::lookAt(ourCamera.Position, ourCamera.Position+ourCamera.Front, ourCamera.Up);

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Update and Render
		// delta
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ourShader.use();
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);
		ourShader.setFloat("mixLevel", mixlevel);

		view = glm::lookAt(ourCamera.Position, ourCamera.Position + ourCamera.Front, ourCamera.Up);
		projection = glm::perspective(glm::radians(ourCamera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f;
			model = glm::rotate(model, angle * i, glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

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
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Dir
	float cameraSpeed = 2.0f * deltaTime;
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

	// mix level
	float mixSpeed = 0.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		mixlevel +=mixSpeed;
		if (mixlevel > 1.0f) mixlevel = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
	{
		mixlevel -= mixSpeed;
		if (mixlevel < 0.0f) mixlevel = 0.0f;
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