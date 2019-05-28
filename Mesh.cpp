//
// Created by hotpot on 2019/5/9.
//


#include <iostream>
#include "MeshIO.h"
#include "GLMesh.h"


bool readOFF(const std::string file_path, std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f) {
	std::ifstream file(file_path);
	if(!file.is_open()) return false;
	std::string line_head;
	file >> line_head;
	if(line_head != "OFF") return false;
	int vs, fs, es;
	file >> vs >> fs >> es;
	v.resize(vs);
	f.resize(fs);
	for(auto& e: v) file >> e.r >> e.g >> e.b;
	for(auto& e: f) {
		file >> es >> e.r >> e.g >> e.b;
	}
	return true;
}

bool readSTL(const std::string file_path, std::vector<glm::vec3> &V, std::vector<glm::ivec3>  &F ){
	FILE* stl_file = fopen(file_path.c_str(), "rb");
	using namespace std;
	
	if(NULL==stl_file)
	{
		fprintf(stderr,"IOError: stl file could not be reopened as binary (1) ...\n");
		return false;
	}
	
	// Specifically 80 character header
	
	char header[80];
	char solid[80];
	bool is_ascii = true;
	if(fread(header,1,80,stl_file) != 80)
	{
		cerr<<"IOError: too short (1)."<<endl;
		goto close_false;
	}
	sscanf(header,"%s",solid);
	if(string("solid") != solid)
	{
		// definitely **not** ascii
		is_ascii = false;
	}else
	{
		// might still be binary
		char buf[4];
		if(fread(buf,1,4,stl_file) != 4)
		{
			cerr<<"IOError: too short (3)."<<endl;
			goto close_false;
		}
		size_t num_faces = *reinterpret_cast<unsigned int*>(buf);
		fseek(stl_file,0,SEEK_END);
		int file_size = ftell(stl_file);
		if(file_size == 80 + 4 + (4*12 + 2) * num_faces)
		{
			is_ascii = false;
		}else
		{
			is_ascii = true;
		}
	}
	
	if(is_ascii)
	{
		std::cerr << "Only Supported binary STL." << std::endl;
	}else
	{
		// Binary
		//stl_file = freopen(NULL,"rb",stl_file);
		fseek(stl_file, 0, SEEK_SET);
		// Read 80 header
		char header[80];
		if(fread(header,sizeof(char),80,stl_file)!=80)
		{
			cerr<<"IOError: bad format (6)."<<endl;
			goto close_false;
		}
		// Read number of triangles
		unsigned int num_tri;
		if(fread(&num_tri,sizeof(unsigned int),1,stl_file)!=1)
		{
			cerr<<"IOError: bad format (7)."<<endl;
			goto close_false;
		}
		V.resize(num_tri*3);
		F.resize(num_tri);
		for(int t = 0;t<(int)num_tri;t++)
		{
			// Read normal
			float n[3];
			if(fread(n,sizeof(float),3,stl_file)!=3)
			{
				cerr<<"IOError: bad format (8)."<<endl;
				goto close_false;
			}
			// Read each vertex
			for(int c = 0;c<3;c++)
			{
				F[t][c] = 3*t+c;
				float v[3];
				if(fread(v,sizeof(float),3,stl_file)!=3)
				{
					cerr<<"IOError: bad format (9)."<<endl;
					goto close_false;
				}
				V[3*t+c][0] = v[0];
				V[3*t+c][1] = v[1];
				V[3*t+c][2] = v[2];
			}
			// Read attribute size
			unsigned short att_count;
			if(fread(&att_count,sizeof(unsigned short),1,stl_file)!=1)
			{
				cerr<<"IOError: bad format (10)."<<endl;
				goto close_false;
			}
		}
		goto close_true;
	}
	assert(V.size() && F.size());
	close_false:
	fclose(stl_file);
	return false;
	close_true:
	fclose(stl_file);
	return true;
}



GLMesh::GLData::GLData() {
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);
}

void GLMesh::GLData::update(std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f) {
	glBindVertexArray(_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ARRAY_BUFFER, f.size() * sizeof(glm::ivec3), f.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(glm::vec3), v.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	
	_draw_count = f.size() * 3;
}

void GLMesh::GLData::draw() {
	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glDrawElements(GL_TRIANGLES, _draw_count, GL_UNSIGNED_INT, (const GLvoid*)0);
	glBindVertexArray(0);
}

GLMesh::GLData::~GLData() {
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ibo);
}
