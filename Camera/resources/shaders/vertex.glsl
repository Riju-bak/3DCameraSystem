#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 'out vec4 gl_Position' is optional, but why? If it's not mentioned as input, how will the vertex shader pass this information onto the rest of the graphics pipeline?
// Answer: gl_Position is a built-in output variable in GLSL (OpenGL Shading Language) that represents the final position of a vertex in clip space.
out vec4 gl_Position;	

out vec2 tex_coord;

void main()
{
	mat4 transform = projection * view * model;
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	// or simply
	//gl_Position = transform * vec4(aPos, 1.0);

	tex_coord = aTexCoord;
}