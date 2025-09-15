#version 460 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in int a_normal;
layout(location = 2) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 normal;
out vec2 uv;

const vec3 normals[6] = vec3[6](
    vec3(-1.0,  0.0,  0.0),  // left
    vec3( 1.0,  0.0,  0.0),  // right
    vec3( 0.0,  1.0,  0.0),  // top
    vec3( 0.0, -1.0,  0.0),  // bottom
    vec3( 0.0,  0.0, -1.0),  // front
    vec3( 0.0,  0.0,  1.0)   // back
);

void main() {
    normal = normalize(mat3(transpose(inverse(u_model))) * normals[a_normal]);
    uv = a_uv;
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
}
