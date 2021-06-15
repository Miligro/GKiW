#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <iostream>

class folder
{
private:
	std::vector<std::pair<int, glm::vec3>> folder_pos;	//Positions of folders
	std::vector<std::pair<bool, glm::vec3>> places;		
	std::vector<folder> folders;	//Subfolders of current folder
	int choose;		//Highlighted folder inside current folder
	int cho;		//Folder in which the viewer is in.

public:
	folder();
	void add_folder();
	void refresh_pos();
	void setChoose(int);
	int getChoose();
	int getCho();
	void setCho();
	void restoreCho();
	void remove_folder();
	std::vector<std::pair<bool, glm::vec3>> getPlaces();
	std::vector<std::pair<int, glm::vec3>> getFolderPos();
};

