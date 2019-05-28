//
// Created by hotpot on 2019/5/9.
//

#ifndef SLICEVISUAL_GLMESH_H
#define SLICEVISUAL_GLMESH_H

#include <GL/gl3w.h>
#include <glm/vec3.hpp>
#include <vector>
#include <memory>

class GLMesh {
 public:
	class GLData {
	 public:
		GLData() ;
		
		void update(std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f) ;
		
		void draw() ;
		
		~GLData() ;
	 private:
		GLuint _vao, _vbo, _ibo;
		GLuint _draw_count;
	};
 
 public:
	glm::vec3 color{0.4f, 0.4f, 0.4f};
	std::shared_ptr<GLData> gl{nullptr};
};

#endif //SLICEVISUAL_GLMESH_H
