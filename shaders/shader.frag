#version 460

in vec3 Normal;

out vec4 FragColor;

void main()
{
	vec3 outColor = normalize(Normal) * 0.5 + 0.5;
	FragColor = vec4(outColor, 1.0f);
}