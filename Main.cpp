
#include "Mesh.h"
#include "Shader.h"
#include "Main.h"
#include "Bezier.h"
#include "Camera.h"
//#include "BezierSurface.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInputCamera(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{	
	//option wireframe
	std::cout << "Press 1 if you want to visualize wireframe. Else press 0" << std::endl;

	int x{ }; // define variable x to hold user input (and zero-initialize it)
	std::cin >> x; // get number from keyboard and store it in variable x

	std::cout << "You entered " << x << '\n';
	bool wireframe = x;

	//nombres de points voulus sur la courbe de bézier
	std::cout << "How many points do you want to draw the Bezier curve?" << std::endl;
	int nbPoints{ }; // define variable x to hold user input (and zero-initialize it)
	std::cin >> nbPoints; // get number from keyboard and store it in variable x

	bool option_curve;
	std::cout << "Press 1 if you want to vizualise a curve. If you want to vizualise a surface, press 0." << std::endl;
	std::cin >> option_curve; // get number from keyboard and store it in variable x

	std::cout << "You entered " << option_curve << '\n';
	


	std::cout << "aaaaaaaaah" << std::endl;


	//init glfw et glad
	GLFWwindow* window = init_utils();

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_TRUE);

	glEnable(GL_DEPTH_TEST);
	//-----------------------create points of bezier curve -------------------------

	struct Vertex v1;
	v1.Position = glm::vec3(-0.5f, 0.5f, 0.0f);
	v1.Normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v1.TexCoords = glm::vec2(0.0f, 0.0f);

	struct Vertex v2;
	v2.Position = glm::vec3(0.f, 0.25f, 0.0f);
	v2.Normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v2.TexCoords = glm::vec2(0.0f, 0.0f);


	struct Vertex v4;
	v4.Position = glm::vec3(0.5f, 0.5f, 0.0f);
	v4.Normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v4.TexCoords = glm::vec2(0.0f, 0.0f);


	vector <glm::vec3> controlPoints = readCSV("controlPointsCurve.csv", wireframe);
	vector<Vertex> vertices= initVertices(controlPoints);
	vector<unsigned int> indices = initIndices(vertices);
	
	Mesh* mesh = new Mesh(vertices, indices, wireframe);
	mesh->setupMesh();

	Bezier* bezier = new Bezier(controlPoints, nbPoints);
	bezier->computeBezierCurve();

	//one color shader
	Shader* shader;
	shader= new Shader("camera.vs", "camera.fs");

	//light shader
	Shader *lightShader;
	lightShader= new Shader("camera.vs", "light.fs");

	//another color shader
	Shader* shader2;
	shader2 = new Shader("camera.vs", "camera2.fs");	
	//shader = new Shader("vertex.glsl", "fragment.glsl");
	bezier->bezierMesh->setupMesh();
	
	//---------------------ATTEMPT TO DRAW BEZIER SURFACE-------------------------


	vector<glm::vec3> ScontrolPoints = readCSV("controlPointsSurface.csv", wireframe);
	
	vector<Vertex> Svertices = initVertices(ScontrolPoints);
	vector<unsigned int> Sindices = initIndices(Svertices);
	
	Bezier* Sbezier = new Bezier(ScontrolPoints, nbPoints);
	Sbezier->computeBezierSurface();


	Mesh* Smesh = new Mesh(Svertices, Sindices, wireframe);
	Sbezier->bezierMesh->generateMeshFromPoints(nbPoints);
	Sbezier->bezierMesh->setupMesh();
	Smesh->setupMesh();

	//------------------TRY TO HANDLE TRIANGLES------------------
	
	vector<Vertex> Tvertices;
	
	vector<unsigned int> Tindices;
	for (int i= 0 ; i < 3 ; ++i)
		Tindices.push_back(i);
	
	vector<Triangle> triangles = Sbezier->bezierMesh->getTrianglesVector();
	Tvertices.push_back(triangles[43].a);
	Tvertices.push_back(triangles[43].b);
	Tvertices.push_back(triangles[43].c);
	
	Sbezier->bezierMesh->computeAllTriangleNormal();
	
	
	vector<Vertex> nVertices= Sbezier->bezierMesh->getAllTriangleCentroid(Sbezier->bezierMesh->trianglesVec[43]);

	//------------try to display adjacent triangles and normals of trianglesVec[43]----------------------------

	vector<Triangle> trianglesAdj = Sbezier->bezierMesh->getTrianglesOfVertex(Sbezier->bezierMesh->vertices[63]);
	std::cout << trianglesAdj.size() << std::endl;
	
	vector<Vertex> triangleAdjVertices;
	
	for (int i = 0; i < trianglesAdj.size(); ++i)
	{
		triangleAdjVertices.push_back(trianglesAdj[i].a);
		triangleAdjVertices.push_back(trianglesAdj[i].b);
		triangleAdjVertices.push_back(trianglesAdj[i].c);

	}

	vector<Vertex> triangleAdjNormal0 = (Sbezier->bezierMesh->getAllTriangleCentroid(trianglesAdj[0]));
	vector<Vertex> triangleAdjNormal1 = (Sbezier->bezierMesh->getAllTriangleCentroid(trianglesAdj[1]));
	vector<Vertex> triangleAdjNormal2 = (Sbezier->bezierMesh->getAllTriangleCentroid(trianglesAdj[2]));
	vector<Vertex> triangleAdjNormal3 = (Sbezier->bezierMesh->getAllTriangleCentroid(trianglesAdj[3]));
	vector<Vertex> triangleAdjNormal4 = (Sbezier->bezierMesh->getAllTriangleCentroid(trianglesAdj[4]));
	vector<Vertex> triangleAdjNormal5 = (Sbezier->bezierMesh->getAllTriangleCentroid(trianglesAdj[5]));


	vector<unsigned int> nIndices2;
	for (int i = 0; i < 2; ++i)
		nIndices2.push_back(i);

	Mesh* n1AdjMesh = new Mesh(triangleAdjNormal0, nIndices2, wireframe);
	Mesh* n2AdjMesh = new Mesh(triangleAdjNormal1, nIndices2, wireframe);
	Mesh* n3AdjMesh = new Mesh(triangleAdjNormal2, nIndices2, wireframe);
	Mesh* n4AdjMesh = new Mesh(triangleAdjNormal3, nIndices2, wireframe);
	Mesh* n5AdjMesh = new Mesh(triangleAdjNormal4, nIndices2, wireframe);
	Mesh* n6AdjMesh = new Mesh(triangleAdjNormal5, nIndices2, wireframe);
	

	vector<unsigned int> triangleAdjIndices;
	for (int i = 0; i < triangleAdjVertices.size(); ++i)
		triangleAdjIndices.push_back(i);

	Mesh* ptitTriangleAdj = new Mesh(triangleAdjVertices, triangleAdjIndices, wireframe);
	ptitTriangleAdj->setupMesh();

	

	//--------------------------------------------------------------------------------------------

	vector<unsigned int> nIndices;
	for (int i = 0; i < 2; ++i)
		nIndices.push_back(i);
	Mesh* nMesh = new Mesh(nVertices, nIndices, wireframe);
	
	
	Mesh * ptitTriangle = new Mesh(Tvertices, Tindices, wireframe);
	ptitTriangle->setupMesh();
	
	Shader* shader3;
	shader3 = new Shader("camera.vs", "camera3.fs");
	
	Shader* shader4;
	shader4 = new Shader("camera.vs", "camera4.fs");

	//---------------------------TRY TO GET VERTEX NORMAL----------------------------
	
	Sbezier->bezierMesh->computeVertexNormal();
	vector<Vertex> vertexNormal = Sbezier->bezierMesh->getVertexNormal();
	
	vector<unsigned int> Vindices;
	for (int i = 0; i < 2; ++i)
		Vindices.push_back(i);
	
	Mesh * vertexNormalMesh = new Mesh(vertexNormal, Vindices, wireframe);



	//*************************  RENDER LOOP ************************************
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInputCamera(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//draw courbe de bezier
		if (option_curve)
		{
			bezier->bezierMesh->Draw(shader, camera, 1);
			mesh->Draw(shader2, camera, 0);
		}
		
		else
		{
			//draw surface de bezier 
			Sbezier->bezierMesh->Draw(lightShader, camera, 2);

			//draw points de controle 
			Smesh->Draw(shader4, camera, 0);

			ptitTriangle->Draw(shader3, camera, 0);

			//draw normale du triangle trianglesVec[43]
			nMesh->Draw(shader4, camera, 1);

			//draw des triangles adj du vertex vertices[63]
			ptitTriangleAdj->Draw(shader3, camera, 0);

			//draw de la normale du vertex vertices[53]
			vertexNormalMesh->Draw(shader, camera, 1);

			//draw des normales des triangles
			n1AdjMesh->Draw(shader3, camera, 1);
			n2AdjMesh->Draw(shader3, camera, 1);
			n3AdjMesh->Draw(shader3, camera, 1);
			n4AdjMesh->Draw(shader3, camera, 1);
			n5AdjMesh->Draw(shader3, camera, 1);
			n6AdjMesh->Draw(shader3, camera, 1);
		}
		
		
	

		glfwSwapBuffers(window);
		glfwPollEvents();

	}




	std::cout << "Coucou: ";
	return 0; 

}


//---------------functions to handle camera and mouse-------------------------------------------------

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInputCamera(GLFWwindow* window)
{
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
