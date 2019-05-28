//
// Created by hotpot on 2019/5/9.
//

#ifndef SLICEVISUAL_MESHIO_H
#define SLICEVISUAL_MESHIO_H

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <fstream>

bool readOFF(const std::string file_path, std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f) ;

bool readSTL(const std::string file_path, std::vector<glm::vec3> &V, std::vector<glm::ivec3>  &F );

#endif //SLICEVISUAL_MESHIO_H
