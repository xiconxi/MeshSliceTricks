//
// Created by hotpot on 2019/5/9.
//

#ifndef SLICEVISUAL_SLICEVISUALUI_H
#define SLICEVISUAL_SLICEVISUALUI_H

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <numeric>
#include <algorithm>
#include "Camera.h"

struct SliceParams{
	void update_from_n() {
		nslices = std::max(1, nslices);
		width = (intervals[1]- intervals[0])/nslices;
	}
	
	void update_from_w() {
		width = std::max(0.01f, std::min((intervals[1]- intervals[0]), width));
		nslices = (intervals[1]- intervals[0])/width;
	}
	
	void update_partial_from_n() {
		partial[1] = nslices;
		partial[0] = std::min(partial[0], nslices);
	}
	
	void updateInterval(std::vector<glm::vec3>& mesh_v) {
		std::vector<float> dot_v(mesh_v.size());
		for(int i = 0 ; i < mesh_v.size(); i++)
			dot_v[i] = glm::dot(axis, mesh_v[i]);

		
		auto minmax = std::minmax_element(dot_v.begin(), dot_v.end());
		intervals = glm::vec2(*minmax.first, *minmax.second);
		
		update_from_n();
		update_partial_from_n();
	}
	
	int nslices = 15;
	float width;
	glm::vec2 intervals;
	
	glm::vec3 axis{.0f, 0.0f, 1.0f};
	
	glm::ivec2 partial{1,1};
};

class imgui_glfw_handle{
 public:
	imgui_glfw_handle(GLFWwindow* glfWwindow){
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
		
		// Setup Dear ImGui style
//		ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();
		
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(glfWwindow, true);
		ImGui_ImplOpenGL3_Init("#version 410 core");
	}
	
	void ui(SliceParams* m_params, Camera& camera) {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("Control");
		
		
		if(m_params) {
			if(ImGui::InputInt("nslices", &m_params->nslices)){
				m_params->update_from_n();
				m_params->update_partial_from_n();
			}
			if(ImGui::InputFloat("slices width", &m_params->width, 0.02f, m_params->intervals[1]-m_params->intervals[0], "%0.4f")) {
				m_params->update_from_w();
				m_params->update_partial_from_n();
			}
			
			ImGui::Text("slices axis  %0.3f, %0.3f,  %0.3f", m_params->axis[0], m_params->axis[1], m_params->axis[2]);
			
			ImGui::Text("slices partial: [%d, %d)", m_params->partial[0], m_params->partial[0]+m_params->partial[1]);
			ImGui::SameLine(0.0f, 3.0f);
			if (ImGui::ArrowButtonEx("slices.partial.l:L", ImGuiDir_Left, ImVec2(40, 19), ImGuiButtonFlags_Repeat)) {
				if(m_params->partial[0]  > 1)
					m_params->partial[0] --;
			}
			ImGui::SameLine(0.0f, 3.0f);
			if (ImGui::ArrowButtonEx("slices.partial.l:R", ImGuiDir_Right, ImVec2(40, 19), ImGuiButtonFlags_Repeat)) {
				if(m_params->partial[0] < m_params->nslices ) {
					m_params->partial[0]++;
					m_params->partial[1] = min(m_params->nslices-m_params->partial[0]+1, m_params->partial[1]);
				}
			}
			ImGui::SameLine(0.0f, 10.0f);
			if (ImGui::ArrowButtonEx("slices.partial.r:L", ImGuiDir_Left, ImVec2(40, 19), ImGuiButtonFlags_Repeat)) {
				if(m_params->partial[1] > 1)
					m_params->partial[1] --;
//				else
//					m_params->partial[1] = m_params->nslices - m_params->partial[0]+1;
			}
			ImGui::SameLine(0.0f, 3.0f);
			if (ImGui::ArrowButtonEx("slices.partial.r:R", ImGuiDir_Right, ImVec2(40, 19), ImGuiButtonFlags_Repeat)) {
				if(m_params->partial[0]+m_params->partial[1] <= m_params->nslices )
					m_params->partial[1] ++;
			}
			
			if(ImGui::Button("Front View", ImVec2(80, 19))) {
				camera.reset(Camera::three_view::FRONT);
			}ImGui::SameLine(0.0f, 3.0f);
			if(ImGui::Button("Right View", ImVec2(80, 19))) {
				camera.reset(Camera::three_view::RIGHT);
			}ImGui::SameLine(0.0f, 3.0f);
			if(ImGui::Button("Top View", ImVec2(80, 19))) {
				camera.reset(Camera::three_view::TOP);
			}
		}
		
		ImGui::End();
	}
	
	void render() {
		// Rendering
		ImGui::Render();
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

};

#endif //SLICEVISUAL_MESHIO_H
