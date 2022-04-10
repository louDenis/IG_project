#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma once

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

GLFWwindow* init_utils()
{
    // glfw: initialize and configure
  // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
       
    }

    return window;
}



//-------------------function to read csv file---------------------------------


vector<glm::vec3> readCSV(string fileName, int wireframe)
{
    vector<vector<float>> data;
    ifstream file(fileName);
    string line;

    vector<glm::vec3> coords;
    vector<Vertex> vertices;
    vector<float> values;
	glm::vec3 temp;

    while (getline(file, line))
    {
        values.clear();
        stringstream lineStream(line);
        string cell;
        while (getline(lineStream, cell, ','))
        {
            values.push_back(stof(cell));
        }
        temp.x = values[0];
		temp.y = values[1];
		temp.z = values[2];
     
		coords.push_back(temp);
          
    }
    return coords;;
}

vector<unsigned int> initIndices(vector<Vertex> vertices)
{
	vector<unsigned int> indices;
	for (int i = 0; i < vertices.size(); i++)
	{
		indices.push_back(i);
	}
	return indices;
}

vector<Vertex> initVertices(vector<glm::vec3> coords)
{
	vector<Vertex> vertices;
    Vertex temp;
	for (int i = 0; i < coords.size(); i++)
	{
        temp.Position = coords[i];
		temp.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		temp.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(temp);
	}
	return vertices;
}
