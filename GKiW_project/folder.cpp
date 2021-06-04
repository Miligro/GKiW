#include "folder.h"

folder::folder(int p) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			places.push_back(std::make_pair(false, glm::vec3(-3.5f + 1.0f * i, 0.0f - 0.8f * j, 0.0f)));
		}
	}
	choose = -1;
	parent = p;
}

void folder::add_folder() {
	for (int i = 0; i < places.size(); i++) {
		if (places.at(i).first == false) {
			places.at(i).first = true;
			folder_pos.push_back(std::make_pair(0, places.at(i).second));
			break;
		}
		else if (i == places.size() - 1) {
			std::cout << "Brak miejsca na folder" << std::endl;
		}
	}
}

void folder::refresh_pos() {
	for (int i = 0; i < folder_pos.size(); i++) {
		if (abs(abs(folder_pos.at(i).second.z) - 3.6) < 0.01) {
			folder_pos.at(i).first = (folder_pos.at(i).first + 1) % 2;
		}
		if (folder_pos.at(i).first == 0) {
			folder_pos.at(i).second.z -= 0.01;
		}
		else {
			folder_pos.at(i).second.z += 0.01;
		}
	}
}

void folder::setChoose(int x) {
	if (x == 3) {
		if (choose < 12 && folder_pos.size() > choose + 3)
			choose += 3;
	}
	else if (x == -3) {
		if (choose > 2)
			choose -= 3;
	}
	else if (x == 1) {
		if (choose < 14 && folder_pos.size() > choose + 1)
			choose += 1;
	}
	else if (x == -1) {
		if (choose > 0)
			choose -= 1;
	}
}

int folder::getChoose() {
	return choose;
}

int folder::getParent() {
	return parent;
}

std::vector<std::pair<bool, glm::vec3>> folder::getPlaces() {
	return places;
}

std::vector<std::pair<int, glm::vec3>> folder::getFolderPos() {
	return folder_pos;
}