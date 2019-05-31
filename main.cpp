#include "GLMesh.h"
#include "MeshIO.h"
#include "GLSLShader.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SliceVisualUI.h"
#include "Camera.h"


Camera cam;
std::vector<glm::vec3> v;
std::vector<glm::ivec3> f;
SliceParams params;
GLMesh mesh;

float centralization(std::vector<glm::vec3>& v) {
	auto mean = std::accumulate(v.begin(), v.end(), glm::vec3(0))/float(v.size());
	
	auto radius = abs(std::accumulate(v.begin(), v.end(), 0.0f, [&](float v, glm::vec3& p){
		return glm::length(p-mean) + v;
	} )/v.size());
	
	for_each(v.begin(), v.end(), [&](glm::vec3& e){ e = (e-mean)/radius;});
	
	return radius;
}

int main(int argc, char** argv) {
	GLFWwindow *window;
	
	{
		/* Initialize the library */
		if (!glfwInit())
			return -1;
		
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		
		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(680, 680, "SliceVisual", NULL, NULL);
		if (!window) {
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		
		if (gl3wInit()) {
			fprintf(stderr, "failed to initialize OpenGL\n");
			return -1;
		}
		if (!gl3wIsSupported(3, 2)) {
			fprintf(stderr, "OpenGL 3.2 not supported\n");
			return -1;
		}
		printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		       glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	
	imgui_glfw_handle ui(window);
	
	GLSLShader shader;
	shader.LoadFromFile(GL_VERTEX_SHADER, "../shaders/slice.v.glsl");
	shader.LoadFromFile(GL_GEOMETRY_SHADER, "../shaders/slice.g.glsl");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "../shaders/slice.f.glsl");
	shader.CreateAndLinkProgram();
	
	shader.Use();
	shader.AddUniform("mvp");
	shader.AddUniform("enable_discard");
	shader.AddUniform("slice_axis_d");
	shader.AddUniform("slice_partial");
	shader.UnUse();
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods)->void {
		if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
			glfwSetWindowShouldClose(window, GL_TRUE);
		if(mods == GLFW_MOD_SHIFT) {
			if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS | action == GLFW_REPEAT)) {
				params.axis = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, glm::vec3(1,0,0))) * params.axis;
			}else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS | action == GLFW_REPEAT) ){
				params.axis = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, -glm::vec3(1,0,0))) * params.axis;
			}else if (key == GLFW_KEY_UP && (action == GLFW_PRESS | action == GLFW_REPEAT)) {
				params.axis = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, glm::vec3(0,1,0))) * params.axis;
			}else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS | action == GLFW_REPEAT) ){
				params.axis = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::pi<float>()*0.02f, -glm::vec3(0,1,0))) * params.axis;
			}
		}else
			cam.onKeys(key, scancode, action, mods);
	});
	
	glfwSetDropCallback(window, [](GLFWwindow* window, int count, const char** paths) -> void {
		v.clear(); f.clear();
		
		if(std::string(paths[0]).find(".STL") != std::string::npos || std::string(paths[0]).find(".stl") != std::string::npos) {
			readSTL(paths[0], v, f);
		}else if (std::string(paths[0]).find(".off") != std::string::npos || std::string(paths[0]).find(".OFF") != std::string::npos){
			readOFF(paths[0], v, f);
		}
		assert(v.size() && v.size());
		centralization(v);
		params.updateInterval(v);
		mesh.gl.reset(new GLMesh::GLData());
		mesh.gl->update(v, f);
	});
	
	
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Poll for and process events */
		glfwPollEvents();
		
		ui.ui(&params, cam);
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		shader.Use();
		glUniform1i(shader("enable_discard"), params.enable);
		glUniform4fv(shader("slice_axis_d"), 1, glm::value_ptr( glm::vec4(params.axis , params.intervals[0])/ (params.width) ));
		glUniformMatrix4fv(shader("mvp"), 1, GL_FALSE, glm::value_ptr(cam.p()*cam.v()));
		glUniform2i(shader("slice_partial"), params.partial[0], params.partial[1]);
		if(mesh.gl != nullptr)
			mesh.gl->draw();
		shader.UnUse();

		ui.render();
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}