#version 400

layout(quads, equal_spacing, ccw) in;
uniform mat4 mvpMatrix;
uniform int heightMapWidth;
uniform int heightMapHeight;
uniform float heightMapHeightScale;
uniform sampler2D heightMapSampler;

void main() {
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	vec4 posn = (1-u) * (1-v) * gl_in[0].gl_Position
	          +    u  * (1-v) * gl_in[1].gl_Position
	          +    u  *    v  * gl_in[2].gl_Position
	          + (1-u) *    v  * gl_in[3].gl_Position;

	vec4 heightVec = texture(heightMapSampler, vec2(posn.x / heightMapWidth, posn.z / heightMapHeight));
	posn.y = (heightVec.r + heightVec.g + heightVec.b) * heightMapHeightScale;

	gl_Position = mvpMatrix * posn;
}