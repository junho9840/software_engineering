#include "glSetUp.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace glm;

#include <iostream>
using namespace std;

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

void init();
void quit();
void render(GLFWwindow * window);
void keyboard(GLFWwindow * window, int key, int code, int action, int mods);

//Camera configuration
vec3 eye(10, 10, 10);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

//Light configuration
vec4 light(5.0, 5.0, 5.0, 1);

//Global coordinate frame
float AXIS_LENGTH = 10;
float AXIS_LINE_WIDTH = 2;

//Colors
GLfloat bgColor[4] = { 1, 1, 1, 1 };

//selected example
int selection = 1;

//play configuration
bool pause = false;

//법선벡터 키고 끄는 토글
bool ntoggle = false;

//점을 저장하는 배열
vec3 point[18][36];

//Min, MAX
int yMAX = 36;
int yMIN = 1;

int zMAX = 18;
int zMIN = 1;


int main(int argc, char * argv[]) {
	//Initialize the OpenGl system
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	//Callbacks
	glfwSetKeyCallback(window, keyboard);

	//Depth test
	glEnable(GL_DEPTH_TEST);

	//Normal vectors are normalized after transformation
	glEnable(GL_NORMALIZE);

	//viewpert and perspective setting
	reshape(window, windowW, windowH);

	//Initialization - Main loop - Finalization
	init();

	//main loop
	float previous = glfwGetTime();
	float elapsed = 0;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		render(window);
		glfwSwapBuffers(window);
	}

	//finalization
	quit();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}


void init() {
	cout << "keyboard Input : 1 for draw 36 x 18 data points using 'GL_POINTS'" << endl;
	cout << "keyboard Input : 2 for draw the wireframe only" << endl;
	cout << "keyboard Input : 3 for draw the quads only" << endl;
	cout << "keyboard Input : 4 for draw the quads and the wireframe" << endl;
	cout << "keyboard Input : 5 for draw the normal vectors of the polygons" << endl;
	cout << "keyboard Input : 6 for Two-Sided constant shading with the wireframe" << endl;
	cout << "keyboard Input : a for increase the sweep angle by 10 around the y-axis" << endl;
	cout << "keyboard Input : s for decrease the sweep angle by -10 around the y-axis" << endl;
	cout << "keyboard Input : j for increase the sweep angle by 20 around the z-axis" << endl;
	cout << "keyboard Input : k for decrease the sweep angle by -20 around the z-axis" << endl;
}

void quit() {
	return;
}



void drawPoint() {
	int numc = 18;
	int numt = 36;
	   
	double TwoPI = 2 * M_PI;
	glColor3f(0,0,1);//색은 파란색
	glPointSize(2.0);//크기는 2
	glBegin(GL_POINTS);
	for (int i = 0; i < numc; i++) {
		for (int j = 0; j < numt; j++) {
			double s = (i + 1) % numc + 0.5;
			double t = j % numt;

			//1 - inner radius
			//0.3 - outer radius
			float x = (1 + 0.3 * cos(s * TwoPI / numc)) * cos(t * TwoPI / numt);
			float y = (0.3 * sin(s * TwoPI / numc));
			float z = (1 + 0.3 * cos(s * TwoPI / numc)) * sin(t * TwoPI / numt);

			point[17-i][35-j] = vec3(3 * x, 2 + 3 * y, 3 * z);
			//점들을 저장
			
		}
	}

	for (int i = 0; i < zMAX - 1; i++) {
		for (int j = 0; j < yMAX - 1; j++) {

			glVertex3f(point[i][j].x, point[i][j].y, point[i][j].z);
			glVertex3f(point[i + 1][j].x, point[i + 1][j].y, point[i + 1][j].z);
			glVertex3f(point[i + 1][j + 1].x, point[i + 1][j + 1].y, point[i + 1][j + 1].z);
			glVertex3f(point[i][j + 1].x, point[i][j + 1].y, point[i][j + 1].z);
			//끝점 이어주기
			if (j == 34) {
				glVertex3f(point[i][35].x, point[i][35].y, point[i][35].z);
				glVertex3f(point[i + 1][35].x, point[i + 1][35].y, point[i + 1][35].z);
				glVertex3f(point[i + 1][0].x, point[i + 1][0].y, point[i + 1][0].z);
				glVertex3f(point[i][0].x, point[i][0].y, point[i][0].z);
			}

			if (i == 16) {
				glVertex3f(point[17][j].x, point[17][j].y, point[17][j].z);
				glVertex3f(point[0][j].x, point[0][j].y, point[0][j].z);
				glVertex3f(point[0][j + 1].x, point[0][j + 1].y, point[0][j + 1].z);
				glVertex3f(point[17][j + 1].x, point[17][j + 1].y, point[17][j + 1].z);
			}

		}
	}
	//마지막 점과 끝점이 만나는 구간 칠해주기
	if (yMAX == 36 && zMAX == 18) {
		glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
		glVertex3f(point[17][0].x, point[17][0].y, point[17][0].z);
		glVertex3f(point[17][35].x, point[17][35].y, point[17][35].z);
		glVertex3f(point[0][35].x, point[0][35].y, point[0][35].z);
	}
	glEnd();
}


void drawWireframeOnly() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Turn on wireframe mode
	glColor3f(0, 0, 0);//검은색
	glBegin(GL_QUADS);

	for (int i = 0; i < zMAX - 1; i++) {
		for (int j = 0; j < yMAX - 1; j++) {

			glVertex3f(point[i][j].x, point[i][j].y, point[i][j].z);
			glVertex3f(point[i + 1][j].x, point[i + 1][j].y, point[i + 1][j].z);
			glVertex3f(point[i + 1][j + 1].x, point[i + 1][j + 1].y, point[i + 1][j + 1].z);
			glVertex3f(point[i][j + 1].x, point[i][j + 1].y, point[i][j + 1].z);
			//끝점 이어주기
			if (j == 34) {
				glVertex3f(point[i][35].x, point[i][35].y, point[i][35].z);
				glVertex3f(point[i + 1][35].x, point[i + 1][35].y, point[i + 1][35].z);
				glVertex3f(point[i + 1][0].x, point[i + 1][0].y, point[i + 1][0].z);
				glVertex3f(point[i][0].x, point[i][0].y, point[i][0].z);
			}

			if (i == 16) {
				glVertex3f(point[17][j].x, point[17][j].y, point[17][j].z);
				glVertex3f(point[0][j].x, point[0][j].y, point[0][j].z);
				glVertex3f(point[0][j + 1].x, point[0][j + 1].y, point[0][j + 1].z);
				glVertex3f(point[17][j + 1].x, point[17][j + 1].y, point[17][j + 1].z);
			}

		}
	}
	//마지막 점과 끝점이 만나는 구간 칠해주기
	if (yMAX == 36 && zMAX == 18) {
		glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
		glVertex3f(point[17][0].x, point[17][0].y, point[17][0].z);
		glVertex3f(point[17][35].x, point[17][35].y, point[17][35].z);
		glVertex3f(point[0][35].x, point[0][35].y, point[0][35].z);
	}

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
}

void drawQuadsOnly() {
	glColor3f(0, 0, 1);//파란색
	glBegin(GL_QUADS);

	for (int i = 0; i < zMAX - 1; i++) {
		for (int j = 0; j < yMAX - 1; j++) {

			glVertex3f(point[i][j].x, point[i][j].y, point[i][j].z);
			glVertex3f(point[i + 1][j].x, point[i + 1][j].y, point[i + 1][j].z);
			glVertex3f(point[i + 1][j + 1].x, point[i + 1][j + 1].y, point[i + 1][j + 1].z);
			glVertex3f(point[i][j + 1].x, point[i][j + 1].y, point[i][j + 1].z);
			//끝점 이어주기
			if (j == 34) {
				glVertex3f(point[i][35].x, point[i][35].y, point[i][35].z);
				glVertex3f(point[i + 1][35].x, point[i + 1][35].y, point[i + 1][35].z);
				glVertex3f(point[i + 1][0].x, point[i + 1][0].y, point[i + 1][0].z);
				glVertex3f(point[i][0].x, point[i][0].y, point[i][0].z);
			}

			if (i == 16) {
				glVertex3f(point[17][j].x, point[17][j].y, point[17][j].z);
				glVertex3f(point[0][j].x, point[0][j].y, point[0][j].z);
				glVertex3f(point[0][j + 1].x, point[0][j + 1].y, point[0][j + 1].z);
				glVertex3f(point[17][j + 1].x, point[17][j + 1].y, point[17][j + 1].z);
			}

		}
	}
	//마지막 점과 끝점이 만나는 구간 칠해주기
	if (yMAX == 36 && zMAX == 18) {
		glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
		glVertex3f(point[17][0].x, point[17][0].y, point[17][0].z);
		glVertex3f(point[17][35].x, point[17][35].y, point[17][35].z);
		glVertex3f(point[0][35].x, point[0][35].y, point[0][35].z);
	}

	glEnd();
}

void drawQuadsAndWireframe() {
	drawQuadsOnly();
	glPolygonOffset(1.1f, 1.1f); //이 거리만큼 떨어트려 줌
	drawWireframeOnly();
}


void drawNormalVector() {
	if (ntoggle == true) {
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		vec3 l(0, 0, 0);
		vec3 v1(0, 0, 0);
		vec3 v2(0, 0, 0);
		vec3 n(0, 0, 0); //법선벡터
		vec3 avep(0, 0, 0); //중점
		vec3 plusn(0, 0, 0); //중점 + 법선벡터
		for (int i = 0; i < zMAX - 1; i++) {
			for (int j = 0; j < yMAX - 1; j++) {
				//카메라까지의 벡터 계산
				l.x = eye[0] - point[i][j].x;
				l.y = eye[1] - point[i][j].y;
				l.z = eye[2] - point[i][j].z;

				//크기 구하기
				float lookSize = sqrt(l.x*l.x + l.y*l.y + l.z*l.z);

				//normalize
				l.x = l.x / lookSize;
				l.y = l.y / lookSize;
				l.z = l.z / lookSize;

				//면의 백터 구하기
				v1.x = point[i + 1][j].x - point[i][j].x;
				v1.y = point[i + 1][j].y - point[i][j].y;
				v1.z = point[i + 1][j].z - point[i][j].z;

				v2.x = point[i][j + 1].x - point[i][j].x;
				v2.y = point[i][j + 1].y - point[i][j].y;
				v2.z = point[i][j + 1].z - point[i][j].z;

				//법선벡터 계산
				n = cross(v1, v2);

				//normalize
				float normalSize = sqrt(n.x*n.x + n.y*n.y + n.z*n.z) * 2;
				//*2는 normal백터의 길이를 조절하기 위해 곱한 임의의 수

				n.x = n.x / normalSize;
				n.y = n.y / normalSize;
				n.z = n.z / normalSize;


				//평면의 4점의 중심점 구하기
				avep.x = (point[i][j].x + point[i + 1][j].x + point[i + 1][j + 1].x + point[i][j + 1].x) / 4;
				avep.y = (point[i][j].y + point[i + 1][j].y + point[i + 1][j + 1].y + point[i][j + 1].y) / 4;
				avep.z = (point[i][j].z + point[i + 1][j].z + point[i + 1][j + 1].z + point[i][j + 1].z) / 4;

				//중심점 + 법선백터 길이
				plusn.x = n.x + avep.x;
				plusn.y = n.y + avep.y;
				plusn.z = n.z + avep.z;

				glVertex3f(avep.x, avep.y, avep.z);
				glVertex3f(plusn.x, plusn.y, plusn.z);

				//예외처리
				if (j == 34) {
					avep.x = (point[i][35].x + point[i + 1][35].x + point[i + 1][0].x + point[i][0].x) / 4;
					avep.y = (point[i][35].y + point[i + 1][35].y + point[i + 1][0].y + point[i][0].y) / 4;
					avep.z = (point[i][35].z + point[i + 1][35].z + point[i + 1][0].z + point[i][0].z) / 4;

					plusn.x = n.x + avep.x;
					plusn.y = n.y + avep.y;
					plusn.z = n.z + avep.z;

					glVertex3f(avep.x, avep.y, avep.z);
					glVertex3f(plusn.x, plusn.y, plusn.z);
				}

				if (i == 16) {
					avep.x = (point[17][j].x + point[0][j].x + point[0][j + 1].x + point[17][j + 1].x) / 4;
					avep.y = (point[17][j].y + point[0][j].y + point[0][j + 1].y + point[17][j + 1].y) / 4;
					avep.z = (point[17][j].z + point[0][j].z + point[0][j + 1].z + point[17][j + 1].z) / 4;

					plusn.x = n.x + avep.x;
					plusn.y = n.y + avep.y;
					plusn.z = n.z + avep.z;

					glVertex3f(avep.x, avep.y, avep.z);
					glVertex3f(plusn.x, plusn.y, plusn.z);
				}


			}
		}
		//마지막 점과 끝점이 만나는 구간 계산
		if (yMAX == 36 && zMAX == 18) {
			avep.x = (point[0][0].x + point[17][0].x + point[17][35].x + point[0][35].x) / 4;
			avep.y = (point[0][0].y + point[17][0].y + point[17][35].y + point[0][35].y) / 4;
			avep.z = (point[0][0].z + point[17][0].z + point[17][35].z + point[0][35].z) / 4;

			plusn.x = n.x + avep.x;
			plusn.y = n.y + avep.y;
			plusn.z = n.z + avep.z;

			glVertex3f(avep.x, avep.y, avep.z);
			glVertex3f(plusn.x, plusn.y, plusn.z);
		}
		glEnd();
	}
	
}

void shadingWithWireframe() {
	drawWireframeOnly();
	glBegin(GL_QUADS);
	vec3 l(0, 0, 0);
	vec3 v1(0, 0, 0);
	vec3 v2(0, 0, 0);
	vec3 n(0, 0, 0);
	for (int i = 0; i < zMAX - 1; i++) {
		for (int j = 0; j < yMAX - 1; j++) {
			//카메라까지의 벡터 계산
			l.x = eye[0] - point[i][j].x;
			l.y = eye[1] - point[i][j].y;
			l.z = eye[2] - point[i][j].z;

			//크기 구하기
			float lookSize = sqrt(l.x*l.x + l.y*l.y + l.z*l.z);

			//normalize
			l.x = l.x / lookSize;
			l.y = l.y / lookSize;
			l.z = l.z / lookSize;

			//면의 백터 구하기
			v1.x = point[i + 1][j].x - point[i][j].x;
			v1.y = point[i + 1][j].y - point[i][j].y;
			v1.z = point[i + 1][j].z - point[i][j].z;

			v2.x = point[i][j + 1].x - point[i][j].x;
			v2.y = point[i][j + 1].y - point[i][j].y;
			v2.z = point[i][j + 1].z - point[i][j].z;

			//법선벡터 계산
			n = cross(v1, v2);

			//normalize
			float normalSize = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);

			n.x = n.x / normalSize;
			n.y = n.y / normalSize;
			n.z = n.z / normalSize;

			//내적
			float result = dot(l, n);

			if (result > 0)
				glColor3f(0, 0, 1);
			else
				glColor3f(1, 0, 0);

			glVertex3f(point[i][j].x, point[i][j].y, point[i][j].z);
			glVertex3f(point[i + 1][j].x, point[i + 1][j].y, point[i + 1][j].z);
			glVertex3f(point[i + 1][j + 1].x, point[i + 1][j + 1].y, point[i + 1][j + 1].z);
			glVertex3f(point[i][j + 1].x, point[i][j + 1].y, point[i][j + 1].z);

			//끝점 이어주기
			if (j == 34) {
				glVertex3f(point[i][35].x, point[i][35].y, point[i][35].z);
				glVertex3f(point[i + 1][35].x, point[i + 1][35].y, point[i + 1][35].z);
				glVertex3f(point[i + 1][0].x, point[i + 1][0].y, point[i + 1][0].z);
				glVertex3f(point[i][0].x, point[i][0].y, point[i][0].z);
			}

			if (i == 16) {
				glVertex3f(point[17][j].x, point[17][j].y, point[17][j].z);
				glVertex3f(point[0][j].x, point[0][j].y, point[0][j].z);
				glVertex3f(point[0][j + 1].x, point[0][j + 1].y, point[0][j + 1].z);
				glVertex3f(point[17][j + 1].x, point[17][j + 1].y, point[17][j + 1].z);
			}
		}
	}
	//마지막 점과 끝점이 만나는 구간 칠해주기
	if (yMAX == 36 && zMAX == 18) {
		glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
		glVertex3f(point[17][0].x, point[17][0].y, point[17][0].z);
		glVertex3f(point[17][35].x, point[17][35].y, point[17][35].z);
		glVertex3f(point[0][35].x, point[0][35].y, point[0][35].z);
	}
	glEnd();
}





void render(GLFWwindow* window) {
	//background color
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	//Axes
	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH*dpiScaling);

	drawNormalVector();

	//Draw 
	switch (selection) {
	case 1: drawPoint();	break;
	case 2: drawWireframeOnly(); break;
	case 3: drawQuadsOnly(); break;
	case 4: drawQuadsAndWireframe(); break;
	//case 5: drawNormalVector(); break;
	case 6: shadingWithWireframe(); break;
	}
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key) {
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_1: selection = 1; break;
		case GLFW_KEY_2: selection = 2; break;
		case GLFW_KEY_3: selection = 3; break;
		case GLFW_KEY_4: selection = 4; break;
		case GLFW_KEY_5: ntoggle = !ntoggle; break;
		case GLFW_KEY_6: selection = 6; break;

		case GLFW_KEY_A: //y방향 10 증가
			if (yMAX == 36)
				break;
			else {
				yMAX += 1;
				break;
			}
		case GLFW_KEY_S : //y방향 10감소
			if (yMAX == yMIN)
				break;
			else {
				yMAX += -1;
				break;
			}
		case GLFW_KEY_J : //z방향 10증가
			if (zMAX == 18)
				break;
			else
				zMAX += 1;
			break;
		case GLFW_KEY_K: //z방향 10감소
			if (zMAX == zMIN)
				break;
			else
				zMAX += -1;
			break;
		}
	
	}
}