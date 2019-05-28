//
// Created by hotpot on 2019/5/9.
//

#ifndef SLICEVISUAL_CAMERA_H
#define SLICEVISUAL_CAMERA_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera {
 public:
	enum three_view{
		FRONT, RIGHT, TOP
	};
	Camera(){
		reset(three_view::FRONT);
	}
	
	void reset(three_view view) {
		switch(view){
			case three_view::FRONT:
				eye = glm::vec3(2, 0, 0);
				up = glm::vec3(0, 1, 0);
				break;
			case three_view::RIGHT:
				eye = glm::vec3(0, 2, 0);
				up = glm::vec3(0, 0, 1);
				break;
			case three_view::TOP:
				eye = glm::vec3(0, 0, 2);
				up = glm::vec3(1, 0, 0);
				break;
		}
	}
	
	glm::mat4 p(){
//		return  glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
		return  glm::perspective(45.0f, 1.0f, 0.01f, 1000.0f);
	}
	glm::mat4 v(){
		return glm::lookAt(eye, center, up);
	}
	
	bool onKeys(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_MINUS && (action == GLFW_PRESS | action == GLFW_REPEAT)) {
			eye -= glm::normalize(center - eye) * 0.02f;
		}else if (key == GLFW_KEY_EQUAL && (action == GLFW_PRESS | action == GLFW_REPEAT) ){
			eye += glm::normalize(center - eye) * 0.02f;
		}else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS | action == GLFW_REPEAT)) {
			eye = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, up)) * (eye-center) + center;
		}else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS | action == GLFW_REPEAT) ){
			eye = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, -up)) * (eye-center) + center;
		}else if (key == GLFW_KEY_UP && (action == GLFW_PRESS | action == GLFW_REPEAT)) {
			glm::vec3 axis = glm::cross(glm::normalize(eye-center), -up);
			eye = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, axis)) * (eye-center) + center;
			up = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, axis)) * up;
		}else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS | action == GLFW_REPEAT) ){
			glm::vec3 axis = glm::cross(glm::normalize(eye-center), up);
			eye = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, axis)) * (eye-center) + center;
			up = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, axis)) * up;
		}
	}
	
 public:
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
};

#endif SLICEVISUAL_CAMERA_H
