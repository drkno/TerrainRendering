#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
out float diffuseTerm;

uniform vec4 lightPos;
uniform mat4 mvpMatrix;

void main() {
	vec3 p1 = gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[1].gl_Position.xyz;
	vec3 p3 = gl_in[2].gl_Position.xyz;

	vec3 norMatrix = normalize(cross(p2 - p1, p3 - p1));

	for (int i = 0; i < gl_in.length(); i++) {
		vec3 lightVec = normalize(lightPos.xyz - gl_in[i].gl_Position.xyz);
		diffuseTerm = max(dot(lightVec, norMatrix), 0.0f);

		gl_Position = mvpMatrix * gl_in[i].gl_Position;
		EmitVertex();
	}
}