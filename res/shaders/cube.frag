#version 460 core

in vec3 normal;
in vec2 uv;

uniform sampler2D u_texture;

out vec4 fragColor;

void main() {
    const vec3 lightColor = vec3(1.0, 1.0, 1.0);
    const vec3 lightDir = -normalize(vec3(0.2, -1.0, -0.5));

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 ambient = 0.15 * lightColor;
    vec3 diffuse = diff * lightColor;

    vec3 textureColor = texture(u_texture, uv).rgb;

    vec3 result = (ambient + diffuse) * textureColor;
    fragColor = vec4(result, 1.0);
}
