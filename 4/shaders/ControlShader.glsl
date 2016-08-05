#version 400

layout(vertices = 4) out;

uniform vec4 cameraPos;

void main() {
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	float dmin = abs(distance(gl_in[0].gl_Position, cameraPos));
	float dmax = dmin;
	for (int i = 1; i < gl_in.length(); i++) {
		float d = abs(distance(gl_in[i].gl_Position, cameraPos));
		dmin = min(dmin, d);
		dmax = max(dmax, d);
	}

	float d = abs(distance(gl_in[gl_InvocationID].gl_Position, cameraPos));

	int Lhigh = 10;
	int Llow = 1;

	float L = ((d - dmin) / (dmax - dmin)) * (Llow - Lhigh) + Lhigh;

	gl_TessLevelOuter[0] = L;
	gl_TessLevelOuter[1] = L;
	gl_TessLevelOuter[2] = L;
	gl_TessLevelOuter[3] = L;
	gl_TessLevelInner[0] = L / 4;
	gl_TessLevelInner[1] = L / 4;
}