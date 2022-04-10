#include "Mesh.h"

void Mesh::setVertex(vector<Vertex> v)
{
    this->vertices = v;
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, bool wireframe)
{
    this->vertices = vertices;
    this->indices = indices;
    setupMesh();
    this->wireframe = wireframe;
}

void Mesh::cpyIntoStructure()
{
    for (int i = 0; i < coords.size(); i++)
    {
        //recopier tout dans la structure vertices, c moche mé c oké
        this->vertices[i].Position = coords[i];
        this->vertices[i].TexCoords = texCoords[i];
        this->vertices[i].Normal = normal[i];
    }

    for (int i = 0; i < coords.size(); i++)
    {
        this->indices[i] = faceIndex[i];
    }
}

void Mesh::loadObjModel(const char* filename)
{
    std::ifstream in(filename, std::ios::in);
    if (!in)
    {
        std::cerr << "Fichier introuvable " << filename << std::endl;
        exit(1);

    }
    vector<string> lines;
    std::string line;
    getline(in, line);
    
    while (getline(in, line)) {
        if (line.substr(0, 2) == "v ")
        {
            std::istringstream v(line.substr(2));
            double x, y, z;
            v >> x; v >> y; v >> z;
            glm::vec3 coord = glm::vec3(x, y, z);
            this->coords.push_back(coord);

        }

        else if (line.substr(0, 2) == "vt")
        {
            std::istringstream v(line.substr(3));
            glm::vec2 tex;
            int U, V;
            v >> U; v >> V;
            tex = glm::vec2(U, V);
            this->texCoords.push_back(tex);
         }

     
        else if (line.substr(0, 2) == "f ") {
            int a, b, c; //to store mesh index
            int A, B, C; //to store texture index
         
            const char* chh = line.c_str();
            sscanf_s(chh, "f %i/%i %i/%i %i/%i", &a, &A, &b, &B, &c, &C); //here it read the line start with f and store the corresponding values in the variables

            a--; b--; c--;
            A--; B--; C--;
           
            this->faceIndex.push_back(a); this->textureIndex.push_back(A);
            this->faceIndex.push_back(b); this->textureIndex.push_back(B);
            this->faceIndex.push_back(c); this->textureIndex.push_back(C);
        }

        else if (line.substr(0, 2) == "Vn") {
            std::istringstream v(line.substr(2));
            double x, y, z;
            v >> x; v >> y; v >> z;
            glm::vec3 norm = glm::vec3(x, y, z);
            this->normal.push_back(norm);
            //vertex.Position = glm::vec3(x, y, z);
            //std::cout << "coords:" << coord.x << coord.y << coord.z << std::endl;
        }


    }

}

void Mesh::setupMesh()
{
    //cpyIntoStructure();

    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
   
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

}

//v= vertices
//vt= texture coordinates
//f= faces

//coordcartésiennes


void Mesh::Draw(Shader * shader, Camera camera, int option)
{
   if (!(this->wireframe))
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    shader->use();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    shader->setMat4("projection", projection);
    shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader->setVec3("lightPos", camera.Position);
    shader->setVec3("cameraPos", camera.Position);

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    shader->setMat4("view", view);

    glBindVertexArray(VAO);
	if (option == 1)
        glDrawElements(GL_LINES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    if (option == 0)
    {	
        glPointSize(8);
        glDrawElements(GL_POINTS, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_POINTS, 0, static_cast<unsigned int>(vertices.size()));
	}
    if (option == 2)
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);	
    glBindVertexArray(0);

} 


int Mesh::IX(int i, int j, int N)
{
	return i * N + j;
}

//--------------------------------------mailler les points de la surface de bézier----------------------
//N nombre de points sur une ligne
void Mesh::generateMeshFromPoints(int N)
{
    indices.clear();
    for (int i = 1; i < N; ++i)
    {
        for (int j = 0; j < N-1; ++j)
        {
            indices.push_back(IX(i, j, N));
            indices.push_back(IX(i - 1, j, N));
			indices.push_back(IX(i-1, j+1, N));

            indices.push_back(IX(i, j, N));
            indices.push_back(IX(i, j + 1, N));
            indices.push_back(IX(i - 1, j + 1, N));

        }
    }
}

//indices.push_back(IX(i, j + 1, N));
//indices.push_back(IX(i, j, N))



//------------------------------calculer les normales----------------------------------------------------

Vertex Mesh::getVertexAt(int i)
{
    return this->vertices.at(i);
}

//fonction qui construit trianglesVec
vector<Triangle> Mesh::getTrianglesVector()
{
    struct Triangle t;

    for (int i = 0; i < indices.size() / 3; ++i)
    {
        t.a = getVertexAt(indices.at(i * 3));
        t.b = getVertexAt(indices.at(i * 3 + 1));
        t.c = getVertexAt(indices.at(i * 3 + 2));
        this->trianglesVec.push_back(t);
    }
    return this->trianglesVec;
}

//fonction qui calcule la normale d'un triangle
glm::vec3 Mesh::computeNormalofTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    return glm::normalize(glm::cross(b - a, c - a));
}

//fonction qui calcule la normale de tout les triangles de trianglesVec
void Mesh::computeAllTriangleNormal()
{
    for (int i = 0; i < this->trianglesVec.size(); ++i)
    {
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
        normal = computeNormalofTriangle(this->trianglesVec[i].a.Position,
                                         this->trianglesVec[i].b.Position,
                                         this->trianglesVec[i].c.Position);
        //parce que une normale sur deux est vers l'interieur et que on les veut toute vers l'exterieur
        if (i % 2 == 0)
            normal = -(normal);
        this->trianglesVec[i].triangleNormale = normal;
    }
}

//-----------ATTEMPT TO DRAW EACH TRIANGLE NORMAL
// 
//fonction qui calcule le centroide d'un triangle
vector<Vertex> Mesh::getTriangleCentroid(Triangle t)
{
    t.centroid.x = (t.a.Position.x + t.b.Position.x + t.c.Position.x) / 3;
	t.centroid.y = (t.a.Position.y + t.b.Position.y + t.c.Position.y) / 3;
    t.centroid.z = (t.a.Position.z + t.b.Position.z + t.c.Position.z) / 3;
    
    vector<Vertex> vNormal;
    
    Vertex v;
    v.Position = glm::vec3(t.centroid.x, t.centroid.y, t.centroid.z);
    v.Normal = glm::vec3(0.0f, 0.0f, 0.f);
    v.TexCoords = glm::vec3(0.0f, 0.0f, 0.f);

    Vertex v2;
    v2.Position = glm::vec3(t.centroid.x + t.triangleNormale.x , t.centroid.y + t.triangleNormale.y, t.centroid.z + t.triangleNormale.z);
    v2.Normal = glm::vec3(0.0f, 0.0f, 0.f);
    v2.TexCoords = glm::vec3(0.0f, 0.0f, 0.f);
	
    vNormal.push_back(v);
    vNormal.push_back(v2);
    return vNormal;
}

vector<Vertex> Mesh::getAllTriangleCentroid(Triangle t)
{
   // struct Triangle t = this->trianglesVec[43];
    vector<Vertex> vertices = getTriangleCentroid(t);
    return vertices;
}


//------------------ATTEMPT TO COMPUTE EACH VERTEX NORMAL----------------------


//fonction qui calcule quels sont les triangles qui sont reliés à un vertex
vector<Triangle> Mesh::getTrianglesOfVertex(Vertex v)
{
	vector<Triangle> triangles;
	for (int i = 0; i < this->trianglesVec.size(); ++i)
	{
		if (this->trianglesVec[i].a.Position == v.Position)
		{
			triangles.push_back(this->trianglesVec[i]);
		}
		if (this->trianglesVec[i].b.Position == v.Position)
		{
			triangles.push_back(this->trianglesVec[i]);
		}
		if (this->trianglesVec[i].c.Position == v.Position)
		{
			triangles.push_back(this->trianglesVec[i]);
		}
	}
	return triangles;
}

void Mesh::computeVertexNormal()
{
    vector<Triangle> trianglesAdj;
    for (int i = 0; i < vertices.size(); ++i)
    {
		trianglesAdj = getTrianglesOfVertex(vertices[i]);
		glm::vec3 sumNormal = glm::vec3(0.0f, 0.0f, 0.0f);
        for (int j = 0; j < trianglesAdj.size(); ++j)
        {
			sumNormal += trianglesAdj[j].triangleNormale;
        }
        sumNormal.x = sumNormal.x / (float)trianglesAdj.size();
        sumNormal.y = sumNormal.y / (float)trianglesAdj.size();
        sumNormal.z = sumNormal.z / (float)trianglesAdj.size();
		vertices[i].Normal = glm::normalize(sumNormal);
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



//fonction qui affiche la normale d'un vertex
vector<Vertex> Mesh::getVertexNormal()
{
    Vertex v = this->vertices[63];
	
	
    Vertex v2;
    v2.Position = glm::vec3(v.Position.x + v.Normal.x,
                            v.Position.y + v.Normal.y,
                            v.Position.z + v.Normal.z);
    v2.Normal = glm::vec3(0.0f, 0.0f, 0.f);
    v2.TexCoords = glm::vec3(0.0f, 0.0f, 0.f);
	
    vector<Vertex> vNormal;
    vNormal.push_back(v);
    vNormal.push_back(v2);
    return vNormal;
}

