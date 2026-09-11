#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 rotation;

out vec3 Normal;

void main() 
{
	gl_Position = rotation * vec4(aPos, 1.0);
	Normal = mat3(rotation) * aNormal;
}