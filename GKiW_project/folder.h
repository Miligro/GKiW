#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <iostream>

class folder
{
private:
	std::vector<std::pair<int, glm::vec3>> folder_pos;
	std::vector<std::pair<bool, glm::vec3>> places;
	std::vector<folder> folders;
	int choose;
	int cho;

public:
	folder();
	void add_folder();
	void refresh_pos();
	void setChoose(int);
	int getChoose();
	int getCho();
	void setCho();
	void restoreCho();
	std::vector<std::pair<bool, glm::vec3>> getPlaces();
	std::vector<std::pair<int, glm::vec3>> getFolderPos();
};

