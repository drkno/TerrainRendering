#version 330

uniform sampler2D heightMapSampler;
in float diffuseTerm;

void main() 
{
	vec4 ambColour = vec4(0.0, 1.0, 0.0, 1.0);
	vec4 diffColour = vec4(diffuseTerm, diffuseTerm, diffuseTerm, 1.0);

	gl_FragColor = diffColour * ambColour;
}