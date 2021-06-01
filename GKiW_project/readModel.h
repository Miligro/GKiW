#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>

class readModel
{
private:
	std::vector <float> vertices;
	std::vector <float> texCoords;
	std::vector <float> normals;
	std::vector <unsigned int> vertexIndexes;
	std::vector <unsigned int> textureIndexes;
	std::vector <unsigned int> normalIndexes;
	std::vector<float> verticesN;
	std::vector<float> normalsN;
	std::vector<float> texCoordsN;
	int indexNumber;
	int vertexNumber;
	const char* filename;

public:
	readModel(const char* file) {
		filename = file;
	}
	void read();
	float* getVertices();
	float* gettexCoords();
	float* getNormals();
	unsigned int* getIndexesVertex();
	unsigned int* getIndexesNormal();
	unsigned int* getIndexesTexCoord();
	int getIndexNumber();
	int getVertexNumber();
	void przeteguj();
};