#include "readModel.h"

void readModel::read() {

	std::ifstream file(filename);
	std::string tmp;
	int index;
	int counter = 0;

	if (file.is_open()) {
		while (!file.eof()) {
			char header[128];
			file >> header;
			if (strcmp(header, "v") == 0) {			//Vertices
				float x, y, z;
				file >> x;
				file >> y;
				file >> z;
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
				vertices.push_back(1);
			}
			else if (strcmp(header, "vt") == 0) {	//Texture coords
				float x, y;
				file >> x;
				file >> y;
				texCoords.push_back(x);
				texCoords.push_back(y);
			}
			else if (strcmp(header, "vn") == 0) {	//normal vectors
				float x, y, z;
				file >> x;
				file >> y;
				file >> z;
				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);
				normals.push_back(0);
			}
			else if (strcmp(header, "f") == 0) {	//Binding coords

				counter = 0;
				getline(file, tmp, '\n');
				std::stringstream ss(tmp);
				std::string word;
				while (ss >> word) {
					std::stringstream ss(word);
					std::string token;

					while (std::getline(ss, token, '/')) {
						index = atoi(token.c_str());
						if (counter % 3 == 0) {
							vertexIndexes.push_back(index - 1);
						}
						else if (counter % 3 == 1) {
							textureIndexes.push_back(index - 1);
						}
						else if (counter % 3 == 2) {
							normalIndexes.push_back(index - 1);
						}
						counter += 1;
					}
				}
			}
		}
	}

	indexNumber = vertexIndexes.size();
	vertexNumber = indexNumber / 3;
	convert();
}

float* readModel::getVertices() {
	return &verticesN[0];
}

float* readModel::getNormals() {
	return &normalsN[0];
}

float* readModel::gettexCoords() {
	return &texCoordsN[0];
}

unsigned int* readModel::getIndexesVertex() {
	return &vertexIndexes[0];
}

unsigned int* readModel::getIndexesNormal() {
	return &normalIndexes[0];
}

unsigned int* readModel::getIndexesTexCoord() {
	return &textureIndexes[0];
}

int readModel::getIndexNumber() {
	return indexNumber;
}

int readModel::getVertexNumber() {
	return vertexNumber;
}

void readModel::convert() {

	for (int i = 0; i < vertexIndexes.size(); i++) {
		verticesN.push_back(vertices[4 * vertexIndexes[i]]);
		verticesN.push_back(vertices[4 * vertexIndexes[i] + 1]);
		verticesN.push_back(vertices[4 * vertexIndexes[i] + 2]);
		verticesN.push_back(vertices[4 * vertexIndexes[i] + 3]);
		normalsN.push_back(normals[4 * normalIndexes[i]]);
		normalsN.push_back(normals[4 * normalIndexes[i] + 1]);
		normalsN.push_back(normals[4 * normalIndexes[i] + 2]);
		normalsN.push_back(normals[4 * normalIndexes[i] + 3]);
		texCoordsN.push_back(texCoords[2 * textureIndexes[i]]);
		texCoordsN.push_back(texCoords[2 * textureIndexes[i] + 1]);
	}
	vertexNumber = verticesN.size() / 4;
}