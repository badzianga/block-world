#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in uint a_face;
layout(location = 2) in uint a_textureIndex;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 normal;
out vec2 uv;

const vec3 normals[6] = vec3[6](
        vec3(-1.0,  0.0,  0.0),  // left
        vec3( 1.0,  0.0,  0.0),  // right
        vec3( 0.0, -1.0,  0.0),  // bottom
        vec3( 0.0,  1.0,  0.0),  // top
        vec3( 0.0,  0.0,  1.0),  // front
        vec3( 0.0,  0.0, -1.0)   // back
);

const vec2 uvCoords[4] = vec2[4](
        vec2(0.0, 1.0),  // bottom-left
        vec2(1.0, 1.0),  // bottom-right
        vec2(1.0, 0.0),  // top-right
        vec2(0.0, 0.0)   // top-left
);

const int uvIndices[8] = int[8](
    0, 1, 2, 3,  // even face
    1, 2, 3, 0   // odd face
);

void main() {
    const uint flip = a_face % 2;
    const uint uvIndex = gl_VertexID % 4 + flip * 4;
    const float stride = 1.0 / 16.0;

    normal = normalize(mat3(transpose(inverse(u_model))) * normals[a_face]);

    uv = uvCoords[uvIndices[uvIndex]];
    uv.x = (uv.x + a_textureIndex % 16) * stride;
    uv.y = (uv.y + a_textureIndex / 16) * stride;

    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
