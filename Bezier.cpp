#include "Bezier.h"

//updated version

glm::vec3 Bezier::getBezierPoints(float t, std::vector<glm::vec3>* controlPoint, int start, int stop)
{
	if (start == stop) return controlPoint->at(start);
	glm::vec3 left = getBezierPoints(t, controlPoint, start, stop - 1);
	glm::vec3 right = getBezierPoints(t, controlPoint, start + 1, stop);
	return ((1 - t) * left) + (t * right);
}

Bezier::Bezier(vector < glm::vec3> controlPoints, int nbPoints)
{
	this->controlPoints = controlPoints;
	this->nbPoints = nbPoints;

}

//fonction pour remplir la classe mesh avec les points sur la courbe de bezier
void Bezier::computeBezierCurve()
{
	//tableau indices
	vector<unsigned int> indices;
	for (int i = 0; i < nbPoints; i++)
	{
		indices.push_back(i);
	}

	vector<Vertex> bezierVertices;

	vector <glm::vec3> bezierPoints;
	float step = 1.0 / (float)this->nbPoints;
	//std::cout << "STEP= " << step << std::endl;
	for (float t = 0; t < 1.0f; t += step)
	{
		std::cout << t << std::endl;
		struct Vertex v;
		v.Position = getBezierPoints(t, &(this->controlPoints), 0, controlPoints.size() - 1);
		v.Normal = glm::vec3(1.0f, 1.0f, 1.0f);
		v.TexCoords = glm::vec2(0.0f, 0.0f);
		bezierVertices.push_back(v);
	}

	//DISPLAY RESULTS
	/*for (int i = 0; i < bezierVertices.size(); i++)
	{
		std::cout << "Point" << i << ":" << bezierVertices.at(i).Position.x << "," << bezierVertices.at(i).Position.y << "," << bezierVertices.at(i).Position.z << std::endl;
	}*/

	//create mesh with Bezier points created
	this->bezierMesh = new Mesh(bezierVertices, indices, 1);

}

//-------------------BEZIER SURFACE-------------------
//fonction pour créer une classe mesh avec les points sur la surface de bezier
void Bezier::computeBezierSurface()
{
	//tableau indices
	vector<unsigned int> indices;
	for (int i = 0; i < nbPoints  * nbPoints; i++)
	{
		indices.push_back(i);
	}

	vector<Vertex> bezierVertices;

	vector <glm::vec3> bezierPoints;
	float step = 1.0 / (float)this->nbPoints;
	//std::cout << "STEP= " << step << std::endl;
	for (float u = 0; u <= 1.0f; u += step)
	{
		for (float v = 0; v <= 1.0f; v += step)
		{
			struct Vertex vertex;
			vertex.Position = getBezierPointSurface(u, v);
			vertex.Normal = glm::vec3(1.0f, 1.0f, 1.0f);
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			bezierVertices.push_back(vertex);
		}
	}
		

	//DISPLAY RESULTS
	/*for (int i = 0; i < bezierVertices.size(); i++)
	{
		std::cout << "Pointdingz" << i << ":" << bezierVertices.at(i).Position.x << "," << bezierVertices.at(i).Position.y << "," << bezierVertices.at(i).Position.z << std::endl;
	}*/

	//create mesh with Bezier points created

	std::cout << "aaaaaaaaaaah" << bezierVertices.size() << std::endl;
	this->bezierMesh = new Mesh(bezierVertices, indices, 1);

}

glm::vec3 Bezier::getBezierPointSurface(float u, float v)
{
	/*for (int i = 0; i < this->controlPoints.size(); ++i)
		{
			std::cout << this->controlPoints.at(i).x << this->controlPoints.at(i).y << this->controlPoints.at(i).z << std::endl;
		}*/
	vector<glm::vec3> allControlPoints = this->controlPoints;
	vector<glm::vec3> subControlPoints;
	for (int i = 0; i < 3; ++i) {
		subControlPoints.push_back(getBezierPoints(u, &allControlPoints, i * 3, (i + 1) * 3 - 1));
		//std::cout << "control points" << i << std::endl;
		/*for (int i = 0; i < subControlPoints.size(); ++i)
		{
			std::cout << subControlPoints.at(i).x << subControlPoints.at(i).y << subControlPoints.at(i).z << std::endl;
		}*/
	}
	glm::vec3 res = getBezierPoints(v, &subControlPoints, 0, subControlPoints.size() - 1);
	this->controlPoints = allControlPoints;
	return res;
}


