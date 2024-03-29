﻿/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.
Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.
Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "other_files/constants.h"
#include "other_files/lodepng.h"
#include "shader_program/shaderprogram.h"
#include "other_files/myCube.h"
#include "other_files/myTeapot.h"
#include "readModel.h"
#include "folder.h"
#include <iostream>
#include <vector>
#include <cmath>

float lamp1 = 0;
float lamp2 = 0;
float aspectRatio = 1;
float walk_speed = 0;
float speed_x1 = 0;
float speed_y1 = 0;
int lampSwitch1 = 0;
int lampSwitch2 = 0;
float lightRatio = 1;

glm::vec3 pos = glm::vec3(0.0f, -0.5f, 0.0f);

glm::vec3 calcDir(float kat_x, float kat_y) {
	glm::vec4 dir = glm::vec4(0, 0, 1, 0);
	glm::mat4 M = glm::rotate(glm::mat4(1.0f), kat_y, glm::vec3(0, 1, 0)); //Right and left
	M = glm::rotate(M, kat_x, glm::vec3(1, 0, 0)); //Top and bottom
	dir = M * dir;
	return glm::vec3(dir);
}

ShaderProgram* sp;
GLuint tex;
GLuint tex1;
GLuint tex2;
GLuint tex3;
GLuint tex4;
readModel wall("wall.obj");
readModel folder1("container.obj");
readModel lampa("reflector.obj");
folder desktop;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);


	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			desktop.setChoose(3);
		}
		if (key == GLFW_KEY_RIGHT) {
			desktop.setChoose(-3);
		}
		if (key == GLFW_KEY_UP) {
			desktop.setChoose(-1);
		}
		if (key == GLFW_KEY_DOWN) {
			desktop.setChoose(1);
		}
		if (key == GLFW_KEY_ENTER) {
			desktop.setCho();	//Enter the folder
		}
		if (key == GLFW_KEY_ESCAPE) {
			desktop.restoreCho();	//Go to parent folder
		}
		if (key == 'L') desktop.remove_folder();
		if (key == 'A') speed_y1 = 1;
		if (key == 'D') speed_y1 = -1;
		if (key == 'Z') lamp1 = 1;
		if (key == 'X') lamp1 = -1;
		if (key == 'C') lamp2 = 1;
		if (key == 'V') lamp2 = -1;
		if (key == 'R') speed_x1 = 1;
		if (key == 'F') speed_x1 = -1;
		if (key == 'W') walk_speed = 2;
		if (key == 'S') walk_speed = -2;
		if (key == 'J') lampSwitch1 == 1 ? lampSwitch1 = 0 : lampSwitch1 = 1;
		if (key == 'K') lampSwitch2 == 1 ? lampSwitch2 = 0 : lampSwitch2 = 1;
		if (key == '-') { lightRatio -= 0.1f; printf("LightRatio: %f\n", lightRatio); }
		if (key == '=') { lightRatio += 0.1f; printf("LightRatio: %f\n", lightRatio); }
		if (key == 'O') desktop.add_folder();
	}
	if (action == GLFW_RELEASE) {
		if (key == 'Z') lamp1 = 0;
		if (key == 'X') lamp1 = 0;
		if (key == 'C') lamp2 = 0;
		if (key == 'V') lamp2 = 0;
		if (key == 'A') speed_y1 = 0;
		if (key == 'D') speed_y1 = 0;
		if (key == 'R') speed_x1 = 0;
		if (key == 'F') speed_x1 = 0;
		if (key == 'W') walk_speed = 0;
		if (key == 'S') walk_speed = 0;
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	tex = readTexture("coding.png");
	tex1 = readTexture("water.png");
	tex2 = readTexture("white.png");
	tex3 = readTexture("metal.png");
	tex4 = readTexture("darkmetal.png");
	sp = new ShaderProgram("shader_program/v_simplest.glsl", NULL, "shader_program/f_simplest.glsl");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	delete sp;
	glDeleteTextures(1, &tex);
	glDeleteTextures(1, &tex1);
	glDeleteTextures(1, &tex2);
	glDeleteTextures(1, &tex3);
	glDeleteTextures(1, &tex4);
}

void drawRoom(GLFWwindow* window, glm::mat4 V, glm::mat4 P, glm::vec4 lp1, glm::vec4 lp2) {

	glm::mat4 M = glm::mat4(1.0f);

	glUniform1i(sp->u("textureMap0"), 0);

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, wall.gettexCoords());

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, wall.getVertices());

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, wall.getNormals());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	for (int i = 0; i < 4; i++) {
		M = glm::mat4(1.0f);
		if (i == 0) {
			M = glm::rotate(M, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)); //Rotate around Y
			M = glm::translate(M, glm::vec3(0.0f, 0.0f, 4.0f)); //Move wall
		}
		else if (i == 1) {
			M = glm::translate(M, glm::vec3(0.0f, 0.0f, 4.0f));
		}
		else if (i == 2) {
			M = glm::translate(M, glm::vec3(0.0f, 0.0f, -4.0f));
		}
		else {
			M = glm::rotate(M, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)); //Same rotate as i==0
			M = glm::translate(M, glm::vec3(0.0f, 0.0f, -4.0f));
		}

		M = glm::scale(M, glm::vec3(4.0f, 2.0f, 0.2f)); //width, height, thickness
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

		glDrawArrays(GL_TRIANGLES, 0, wall.getVertexNumber());
	}

	//Create floor
	M = glm::mat4(1.0f);
	M = glm::rotate(M, 90.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::translate(M, glm::vec3(-2.2f, 0.0f, 0.0f));
	M = glm::scale(M, glm::vec3(0.2f, 4.0f, 4.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glUniform1i(sp->u("textureMap0"), 1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glDrawArrays(GL_TRIANGLES, 0, wall.getVertexNumber());

	//Create ceiling
	M = glm::mat4(1.0f);
	M = glm::rotate(M, 90.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::translate(M, glm::vec3(2.2f, 0.0f, 0.0f));
	M = glm::scale(M, glm::vec3(0.2f, 4.0f, 4.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glUniform1i(sp->u("textureMap0"), 2);


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glDrawArrays(GL_TRIANGLES, 0, wall.getVertexNumber());

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void drawReflector(GLFWwindow* window, float angle_1, float angle_2, glm::mat4 V, glm::mat4 P, glm::vec4 lp1, glm::vec4 lp2) {

	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(3.6f, 2.0f, 3.6f));
	M = glm::rotate(M, angle_1, glm::vec3(0.0f, 1.0f, 0.0f));


	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glUniform1i(sp->u("textureMap0"), 4);

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, lampa.gettexCoords());

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, lampa.getVertices());

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, lampa.getNormals());

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, tex4);
	glDrawArrays(GL_TRIANGLES, 0, lampa.getVertexNumber());


	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(-3.6f, 2.0f, 3.6f));
	M = glm::rotate(M, -angle_2, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glDrawArrays(GL_TRIANGLES, 0, lampa.getVertexNumber());


	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void drawFolders(GLFWwindow* window, glm::mat4 V, glm::mat4 P, glm::vec4 lp1, glm::vec4 lp2) {

	glm::mat4 M = glm::mat4(1.0f);

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, folder1.gettexCoords());

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, folder1.getVertices());

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, folder1.getNormals());

	glUniform1i(sp->u("textureMap0"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, tex3);

	for (int i = 0; i < desktop.getFolderPos().size(); i++) {
		if (i != desktop.getChoose()) {
			M = glm::mat4(1.0f);
			M = glm::rotate(M, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			M = glm::translate(M, desktop.getFolderPos().at(i).second);
			glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
			glDrawArrays(GL_TRIANGLES, 0, folder1.getVertexNumber());
		}
	}

	if (desktop.getChoose() != -1) {
		glUniform1i(sp->u("textureMap0"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tex2);
		M = glm::mat4(1.0f);
		M = glm::rotate(M, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		M = glm::translate(M, desktop.getFolderPos().at(desktop.getChoose()).second);
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		glDrawArrays(GL_TRIANGLES, 0, folder1.getVertexNumber());
	}
 
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_1, float angle_2, float kat_x, float kat_y) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(pos, pos + calcDir(kat_x, kat_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 0.1f, 50.0f); //Wylicz macierz rzutowania

	glm::mat3 R1 = glm::mat3(glm::vec3(cos(angle_1), 0, sin(angle_1)), glm::vec3(0, 1, 0), glm::vec3(-sin(angle_1), 0, cos(angle_1)));
	glm::vec3 N1 = glm::vec3(0.0f, -0.26f, -0.11f) * R1;
	glm::vec3 lp1 = N1 + glm::vec3(3.6f, 2.00f, 3.6f); //Light 1 ( Left )

	glm::mat3 R2 = glm::mat3(glm::vec3(cos(-angle_2), 0, sin(-angle_2)), glm::vec3(0, 1, 0), glm::vec3(-sin(-angle_2), 0, cos(-angle_2)));
	glm::vec3 N2 = glm::vec3(0.0f, -0.26f, -0.11f) * R2;
	glm::vec3 lp2 = N2 + glm::vec3(-3.6f, 2.00f, 3.6f); //Light 2 ( Right )

	sp->use();

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniform1i(sp->u("lpSw1"), lampSwitch1);
	glUniform1i(sp->u("lpSw2"), lampSwitch2);
	glUniform1f(sp->u("lightRatio"), lightRatio);

	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(glm::vec4(lp1, 1)));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(glm::vec4(lp2, 1)));


	drawRoom(window, V, P, glm::vec4(lp1, 1), glm::vec4(lp2, 1));
	drawReflector(window, angle_1, angle_2, V, P, glm::vec4(lp1,1), glm::vec4(lp2, 1));
	drawFolders(window, V, P, glm::vec4(lp1, 1), glm::vec4(lp2, 1));
	glfwSwapBuffers(window);
}

void start() {
	wall.read();
	folder1.read();
	lampa.read();
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno
	start();
	
	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1080, 1080, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);

	
	float angle_1 = 0; //Angles for lamps
	float angle_2 = 0; 
	float angle = 0;
	float kat_x = 0; //Angles for view
	float kat_y = 0;

	glfwSetTime(0);
	while (!glfwWindowShouldClose(window))
	{
		kat_x += speed_x1 * glfwGetTime();
		kat_y += speed_y1 * glfwGetTime();
		pos += (float)(walk_speed * glfwGetTime()) * calcDir(kat_x, kat_y);
		if (angle_1 >1.57 && lamp1 == -1 || angle_1 < 0.0 && lamp1 == 1 || angle_1 <= 1.57 && angle_1 >= 0.0) {
			angle_1 += lamp1 * glfwGetTime();
		}
		if (angle_2 > 1.57 && lamp2 == -1 || angle_2 < 0.0 && lamp2 == 1 || angle_2 <= 1.57 && angle_2 >= 0.0) {
			angle_2 += lamp2 * glfwGetTime();
		}
		glfwSetTime(0); //Zeruj timer
		drawScene(window, angle_1, angle_2, kat_x, kat_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
		desktop.refresh_pos();
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}