#version 330

uniform sampler2D heightMapSampler;
uniform sampler2D waterSampler;
uniform sampler2D grassSampler;
uniform sampler2D rockSampler;
uniform sampler2D snowSampler;
uniform int isWireframe;

in float diffuseTerm;
in vec2 texCoords;
in vec4 texWeights;

void main() 
{
	if (isWireframe == 1) {
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else {
		vec4 water = texture(waterSampler, texCoords);
		vec4 grass = texture(grassSampler, texCoords);
		vec4 rock = texture(rockSampler, texCoords);
		vec4 snow = texture(snowSampler, texCoords);

		vec4 texColour = texWeights[0] * water + texWeights[1] * grass + texWeights[2] * rock + texWeights[3] * snow;
		//vec4 ambColour = vec4(0.0, 1.0, 0.0, 1.0);
		vec4 diffColour = vec4(diffuseTerm, diffuseTerm, diffuseTerm, 1.0);

		gl_FragColor = diffColour * texColour;
	}
}