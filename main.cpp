#include "raycast.hpp"

#define WINDOW_SIZE 512


// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";



int main()
{
	
	glfwInit();															// Initialize GLFW

	
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);						// Tell GLFW what version of OpenGL we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);						// In this case we are using OpenGL 4.1
	

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// Tell GLFW we are using the CORE profile So that means we only have the modern functions

	
	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "RC", NULL, NULL);	// Create a GLFWwindow object of window sizw, naming it "RC"
	
	if (window == NULL)													// Error check if the window fails to create
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);                                     // Introduce the window into the current context

	gladLoadGL();                                                       //Load GLAD so it configures OpenGL
	glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);                         // Specify the viewport of OpenGL in the Window
	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);             // Create Vertex Shader Object and get its reference
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);         // Attach Vertex Shader source to the Vertex Shader Object
	glCompileShader(vertexShader);                                      // Compile the Vertex Shader into machine code

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);         // Create Fragment Shader Object and get its reference
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);     // Attach Fragment Shader source to the Fragment Shader Object
	glCompileShader(fragmentShader);                                    // Compile the Vertex Shader into machine code

	
	GLuint shaderProgram = glCreateProgram();                           // Create Shader Program Object and get its reference
	glAttachShader(shaderProgram, vertexShader);                        // Attach the Vertex and *next line*
	glAttachShader(shaderProgram, fragmentShader);                      // Fragment Shaders to the Shader Program
	
	glLinkProgram(shaderProgram);                                       // Wrap-up/Link all the shaders together into the Shader Program

	glDeleteShader(vertexShader);                                       // Delete the now useless Vertex and *next line*
	glDeleteShader(fragmentShader);                                     // Fragment Shader objects


	// Vertices coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 						// Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 						// Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 						// Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 					// Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 						// Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f 							// Inner down
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 3, 5, 														// Lower left triangle
		3, 2, 4, 														// Upper triangle
		5, 4, 1 														// Lower right triangle
	};
    
    // Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAO;                                                         //stores the state configuration for vertex attribute pointers. 
                                                                        //It acts as a container for VBOs and how their data should be interpreted
    GLuint VBO;                                                         //stores vertex data (positions, colors, texture coordinates, normals) in the GPU's memory. 
	GLuint EBO;															// An EBO is a buffer object that stores indices to the vertices within one or more VBOs.                                                    
    
	// Generate the VAO and VBO and EBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);                                             // Make the VAO the current Vertex Array Object by binding it

	glBindBuffer(GL_ARRAY_BUFFER, VBO);                                 // Bind the VBO specifying it's a GL_ARRAY_BUFFER
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// Introduce the vertices into the VBO static_draw can bt dynamic


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);							// Bind the EBO specifying it's a GL_ELEMENT_ARRAY_BUFFER
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);// Introduce the indices into the EBO

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glEnableVertexAttribArray(0);										// Enable the Vertex Attribute so that OpenGL knows to use it

	glBindBuffer(GL_ARRAY_BUFFER, 0);									// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindVertexArray(0);



	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);						// Specify the color of the background
		glClear(GL_COLOR_BUFFER_BIT);									// Clean the back buffer and assign the new color to it
		glUseProgram(shaderProgram);									// Tell OpenGL which Shader Program we want to use
		glBindVertexArray(VAO);											// Bind the VAO so OpenGL knows to use it
		//glDrawArrays(GL_TRIANGLES, 0, 9);								// Draw the triangle using the GL_TRIANGLES primitive

		
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);			// Draw primitives, number of indices, datatype of indices, index of indices

		glfwSwapBuffers(window);										// Swap the back buffer with the front buffer
		glfwPollEvents();												// Take care of all GLFW events like pressing keys or closing window
	}



	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	
	glfwDestroyWindow(window);											// Delete window before ending the program
	glfwTerminate();													// Terminate GLFW before ending the program
	return 0;
}