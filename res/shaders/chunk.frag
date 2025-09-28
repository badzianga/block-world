#version 460 core

in vec3 normal;
in vec2 uv;

uniform sampler2D u_texture;

out vec4 fragColor;

const vec3 lightDir = -normalize(vec3(0.2, -1.0, -0.5));

const float ambient = 0.6;

void main() {
    float diffuse = max(dot(normal, lightDir), 0.0);

    vec3 textureColor = texture(u_texture, uv).rgb;

    vec3 result = min(ambient + diffuse, 1.0) * textureColor;
    fragColor = vec4(result, 1.0);
}
