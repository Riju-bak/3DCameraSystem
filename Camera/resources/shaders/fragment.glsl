#version 330 core

out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//Sample the texture "texture1" at the texture coordinate "tex_coord", which is a vector that specifies the location in the texture to sample. 
	// The result of this operation is a color value that represents the texel (i.e. a pixel in the texture) at the specified coordinate.
	FragColor = mix(texture(texture1, tex_coord), texture(texture2, tex_coord), 0.2);  //Sample the texture
}