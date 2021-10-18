#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 mat;


void main()
{
    vec3 pos = vec3(aPos.x, aPos.y, aPos.z);
    gl_Position = mat * vec4(pos, 1.0f);

}