#version 460 core

in vec3 normal;

out vec4 fragColor;

void main() {
    const vec3 lightColor = vec3(1.f, 1.f, 1.f);
    const vec3 objectColor = vec3(0.8f, 0.6f, 0.2f);
    const vec3 lightDir = -normalize(vec3(0.2f, -1.f, -0.5f));

    float diff = max(dot(normal, lightDir), 0.f);

    vec3 ambient = 0.15f * lightColor;
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.f);
}
