#version 410 core
out vec4 color;

in vec3 face_norm;
in vec3 pos;
uniform vec4 slice_axis_d;
uniform ivec2 slice_partial;
uniform bool enable_discard = false;

void main()
{
    float v_dis = dot(pos, slice_axis_d.xyz)-slice_axis_d.w;
    if(enable_discard) if(cos(2*3.1415926*fract(v_dis) ) > -.6 || v_dis >= slice_partial[1] + slice_partial[0] || v_dis < slice_partial[0] - 1) discard;
    color = vec4(vec3(enable_discard? fract(v_dis): 0.9)*(0.4f+0.6*abs(dot(vec3(-1,0,0),face_norm))), 1);
}
