#version 410 core
layout (location = 0) in vec3 vert;

void main()
{
    gl_Position = vec4(vert,1.0);//vec4(0.01*gl_VertexID, 0.01*gl_VertexID, 0.0, 0.0); //
}
