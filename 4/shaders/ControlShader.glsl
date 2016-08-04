#version 400

layout(vertices = 4) out;

uniform vec4 cameraPos;

void main() {
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	float dmin = 5.0;
	float dmax = 200.0;
	float d = max(min(abs(distance(gl_in[gl_InvocationID].gl_Position, cameraPos)), dmax), dmin);

	int Lhigh = 6;
	int Llow = 10;

	float L = ((d - dmin) / (dmax - dmin)) * (Llow - Lhigh) + Lhigh;

	gl_TessLevelOuter[0] = L;
	gl_TessLevelOuter[1] = L;
	gl_TessLevelOuter[2] = L;
	gl_TessLevelOuter[3] = L;
	gl_TessLevelInner[0] = L;
	gl_TessLevelInner[1] = L;
}