#version 400

layout(quads, equal_spacing, ccw) in;
uniform mat4 mvpMatrix;
uniform sampler2D heightMapSampler;

void main() {
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	vec4 posn = (1-u) * (1-v) * gl_in[0].gl_Position
	          +    u  * (1-v) * gl_in[1].gl_Position
	          +    u  *    v  * gl_in[2].gl_Position
	          + (1-u) *    v  * gl_in[3].gl_Position;

	float heightVec = texture(heightMapSampler, vec2(0,0)).r;



    //posn.y = heightVec;

	gl_Position = mvpMatrix * posn;
}