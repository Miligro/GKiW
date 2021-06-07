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
				folder_pos.push_back(std::make_pair(0, places.at(i).second));
				folder x;
				folders.push_back(x);
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
	//if (cho == -1) {
		return cho;
	//}
	//else {
		//return folders.at(cho).getCho();
	//}
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