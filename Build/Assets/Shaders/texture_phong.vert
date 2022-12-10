#version 430 core 

in layout(location = 0) vec3 vPosition;
in layout(location = 1) vec2 vCoords;
in layout(location = 2) vec3 vNormal;

out vec3 position;
out vec3 normal;
out vec2 coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// create mvp matrix
	mat4 model_view = view * model;
	// pass data through
	normal = mat3(model_view) * vNormal;
	position = vec3(model_view * vec4(vPosition, 1));
	coords = vCoords;

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vPosition, 1.0);;
}
