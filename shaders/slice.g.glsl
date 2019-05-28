#version 410 core

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 mvp;

out vec3 face_norm;
out vec3 pos;

void main()
{
    face_norm = normalize( cross((gl_in[2].gl_Position-gl_in[0].gl_Position).xyz, (gl_in[1].gl_Position-gl_in[0].gl_Position).xyz) );
    for(int i = 0; i< 3; i++) {
        pos = gl_in[i].gl_Position.xyz;
        gl_Position = mvp * gl_in[i].gl_Position;
        EmitVertex();
    }
    gl_PrimitiveID  =  gl_PrimitiveIDIn;
    EndPrimitive();
}
