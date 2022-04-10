#pragma once
#include "Shader.h"
#include <iostream>
#include <fstream> 
#include <string> 
#include <sstream>  
#include <vector>
#include <glad/glad.h>

#include <windows.h>
#include "Camera.h"
using namespace std;


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

//struct with each triangle vertex
struct Triangle
{
    Vertex a;
    Vertex b;
    Vertex c;
    glm::vec3 triangleNormale;
    glm::vec3 centroid;
    //vector<Triangle> neighbors;
};


class Mesh
{
public:
	// lighting
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    bool wireframe;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, bool wireframe);
    vector< glm::vec3> coords;
    vector< glm::vec3> normal;
    vector< glm::vec2> texCoords;
    vector <int> faceIndex;
    vector<int> textureIndex;
    unsigned int VAO, VBO, NBO, EBO;

    vector<Vertex> vertices;
    vector<unsigned int> indices;

    void loadObjModel(const char * filename);
    void setupMesh();
    void cpyIntoStructure();
    void Draw(Shader * shader, Camera camera, int option);

    void generateMeshFromPoints(int N);
    int IX(int i, int j, int N);
	

    vector<Triangle> trianglesVec;
    glm::vec3 computeNormalofTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    vector<Triangle> getTrianglesVector();
	Vertex getVertexAt(int i);
    void computeAllTriangleNormal();
    vector<Vertex> getTriangleCentroid(Triangle t);
    vector<Vertex> getAllTriangleCentroid(Triangle t);
    vector<Triangle> getTrianglesOfVertex(Vertex v);
    
	//functions to get normal of vertices
    void computeVertexNormal();
    vector<Vertex> getVertexNormal();

    void setVertex(vector<Vertex> v);
	
};


