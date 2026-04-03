#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDirection = normalize(lightPos - fragPos);
    // According to LearnOpenGL, the normal should be normalized here, but we 
    // know it's already normalized, so normalizing here seems overly cautious
    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    fragColor = vec4((ambient + diffuse) * objectColor, 1.0);
}