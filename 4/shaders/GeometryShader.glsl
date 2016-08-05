#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform vec4 lightPos;
uniform mat4 mvpMatrix;
uniform int heightMapWidth;
uniform int heightMapHeight;
uniform float heightMapHeightScale;

out float diffuseTerm;
out vec2 texCoords;
out vec4 texWeights;

void main() {
	vec3 p1 = gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[1].gl_Position.xyz;
	vec3 p3 = gl_in[2].gl_Position.xyz;

	vec3 norMatrix = normalize(cross(p2 - p1, p3 - p1));
	float div = heightMapHeightScale / 4.0f;
	float div3 = div / 3.0f;

	for (int i = 0; i < gl_in.length(); i++) {
		vec4 posn = gl_in[i].gl_Position;

		vec3 lightVec = normalize(lightPos.xyz - posn.xyz);
		diffuseTerm = max(dot(lightVec, norMatrix), 0.0f);

		texCoords = vec2(posn.x / heightMapWidth, posn.z / heightMapHeight);
		if (posn.y < div) {
			texWeights = vec4(1.0, 0.0, 0.0, 0.0);
		}
		else if (posn.y < div * 2.0f) {
			texWeights = vec4(0.0, 1.0, 0.0, 0.0);
		}
		else if (posn.y < div * 2.0f + div3) {
			float change = (posn.y - div * 2.0f) / div3;
			texWeights = vec4(0.0, change, 1.0 - change, 0.0);
		}
		else if (posn.y < div * 3.0f) {
			texWeights = vec4(0.0, 0.0, 1.0, 0.0);
		}
		else if (posn.y < div * 3.0f + div3) {
			float change = (posn.y - div * 3.0f) / div3;
			texWeights = vec4(0.0, 0.0, change, 1.0 - change);
		}
		else {
			texWeights = vec4(0.0, 0.0, 0.0, 1.0);
		}

		gl_Position = mvpMatrix * posn;
		EmitVertex();
	}
}