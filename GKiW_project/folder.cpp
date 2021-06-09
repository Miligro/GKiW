#include "folder.h"

folder::folder() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			places.push_back(std::make_pair(false, glm::vec3(-3.5f + 1.0f * i, 0.0f - 0.8f * j, 0.0f)));
		}
	}
	choose = -1;
	cho = -1;
}

void folder::add_folder() {
	if (cho == -1) {
		for (int i = 0; i < places.size(); i++) {
			if (places.at(i).first == false) {
				places.at(i).first = true;
				std::vector<std::pair<int, glm::vec3>>::iterator it_pos;
				it_pos = folder_pos.begin();
				folder_pos.insert(it_pos+i, std::make_pair(0, places.at(i).second));
				//folder_pos.push_back(std::make_pair(0, places.at(i).second));

				std::vector<folder>::iterator it_place;
				it_place = folders.begin();

				folder x;
				folders.insert(it_place + i, x);
				//folders.push_back(x);
				break;
			}
			else if (i == places.size() - 1) {
				std::cout << "Brak miejsca na folder" << std::endl;
			}
		}
	}
	else {
		folders.at(cho).add_folder();
	}
}


void folder::remove_folder() {
	if (cho == -1) {
		if (choose != -1) {
			folders.erase(folders.begin() + choose);
			folder_pos.erase(folder_pos.begin() + choose);
			places.at(choose).first = false;
			choose -= 1;
		}
	}
	else {
		folders.at(cho).remove_folder();
	}
}

void folder::refresh_pos() {
	if (cho == -1) {
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
	else {
		folders.at(cho).refresh_pos();
	}
}

void folder::setChoose(int x) {
	if (cho == -1) {
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
	else {
		folders.at(cho).setChoose(x);
	}
}

int folder::getChoose() {
	if (cho == -1) {
		return choose;
	}
	else {
		return folders.at(cho).getChoose();
	}
}

std::vector<std::pair<bool, glm::vec3>> folder::getPlaces() {
	if (cho == -1) {
		return places;
	}
	else {
		return folders.at(cho).getPlaces();
	}
}

std::vector<std::pair<int, glm::vec3>> folder::getFolderPos() {
	if (cho == -1) {
		return folder_pos;
	}
	else {
		return folders.at(cho).getFolderPos();
	}
}

int folder::getCho() {
	return cho;
}

void folder::setCho() {
	if (cho == -1 && choose != -1) {
		cho = choose;
	}
	else if (cho != -1){
		folders.at(cho).setCho();
	}
}


void folder::restoreCho() {
	if (cho != -1){
		if (folders.at(cho).getCho() == -1) {
			cho = -1;
		}
		else {
			folders.at(cho).restoreCho();
		}
	}
}