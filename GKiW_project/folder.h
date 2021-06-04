#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <iostream>

class folder
{
private:
	std::vector<std::pair<int, glm::vec3>> folder_pos;
	std::vector<std::pair<bool, glm::vec3>> places;
	int choose;
	int parent;

public:
	folder(int);
	void add_folder();
	void refresh_pos();
	void setChoose(int);
	int getChoose();
	int getParent();
	std::vector<std::pair<bool, glm::vec3>> getPlaces();
	std::vector<std::pair<int, glm::vec3>> getFolderPos();
};

