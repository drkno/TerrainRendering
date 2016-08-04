#version 330

uniform sampler2D heightMapSampler;
in float diffuseTerm;

void main() 
{
	if (diffuseTerm > 1) {
		gl_FragColor = vec4(1, 0, 0, 1.0);
	}
	else {
		gl_FragColor = vec4(diffuseTerm, diffuseTerm, diffuseTerm, 1.0);
	}
}