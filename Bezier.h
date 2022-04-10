#pragma once
#include "Mesh.h"

class Bezier
{
public:
	Mesh * bezierMesh;
   
	vector<glm::vec3> controlPoints;
    int nbPoints;
	
	//int degree; //nb_controlPoints-1
    //int number_of_curve_points;
        
    glm::vec3 getBezierPoints(float t, std::vector<glm::vec3>* controlPoint, int start, int stop);
    Bezier(vector <glm::vec3> controlPoints, int nbPoints);
    void computeBezierCurve();
	
    void computeBezierSurface();
    glm::vec3 getBezierPointSurface(float u, float v);
};

