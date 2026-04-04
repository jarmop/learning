#version 330 core

// According to LearnOpenGL, the normal should be normalized in this shader, but
// we know it's already normalized, so normalizing again seems overly cautious
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDirection = normalize(lightPos - fragPos);
    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    // specular
    float specularStrength = 0.5;
    int shininess = 32;
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float angularDistance = max(dot(viewDirection, reflectDirection), 0.0);
    vec3 specular = specularStrength * pow(angularDistance, shininess) * lightColor;

    fragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}