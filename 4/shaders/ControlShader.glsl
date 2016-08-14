#version 400

layout(vertices = 4) out;

uniform vec4 cameraPos;
uniform int heightMapWidth;
uniform int heightMapHeight;
uniform float heightMapHeightScale;
uniform sampler2D heightMapSampler;

float getHeight(float x, float z) {
	vec4 heightVec = texture(heightMapSampler, vec2(x, z));
	return (heightVec.r + heightVec.g + heightVec.b) * heightMapHeightScale;
}

float getMax(float n, float curr) {
	float h = abs(n - curr);
	return h > curr ? h : curr;
}

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
	float d = abs(distance(gl_in[gl_InvocationID].gl_Position, cameraPos));
	float x = max(gl_in[gl_InvocationID].gl_Position.x / heightMapWidth, 0.0f);
	float z = max(abs(gl_in[gl_InvocationID].gl_Position.z / heightMapHeight), 0.0f);
	int Lhigh = 3;
	int Llow = 1;
	int dmin = 0;
	int dmax = heightMapHeight / 4;

	float L = ((d - dmin) / (dmax - dmin)) * (Llow - Lhigh) + Lhigh;
	float change = 3.0;
	float lookDist = 4.0 / heightMapHeight;
	if (x > lookDist && x < 1 - lookDist && z > lookDist && z < 1 - lookDist) {
		float h = getHeight(x, z);
		float d = getMax(getHeight(x + lookDist, z), 0.0f);
		d = getMax(getHeight(x, z + lookDist), d);
		d = getMax(getHeight(x - lookDist, z), d);
		d = getMax(getHeight(x, z - lookDist), d);

		float g = abs((d - h) / lookDist);

		if (g < 1) {
			change = 1.0;
		}
		else if (g < 2) {
			change = 1.5;
		}
		else if (g < 4) {
			change = 2.0;
		}
		else if (g < 8) {
			change = 2.5;
		}
	}
	L *= change;

	L = max(L, 1);

	gl_TessLevelOuter[0] = L;
	gl_TessLevelOuter[1] = L;
	gl_TessLevelOuter[2] = L;
	gl_TessLevelOuter[3] = L;
	gl_TessLevelInner[0] = L;
	gl_TessLevelInner[1] = L;
}
