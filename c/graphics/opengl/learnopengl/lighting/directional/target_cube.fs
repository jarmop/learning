#version 330 core

struct Light {
    vec3 color;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// According to LearnOpenGL, the normal should be normalized in this shader, but
// we know it's already normalized, so normalizing again seems overly cautious
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Light light;

void main() {
    // ambient
    vec3 ambient = light.ambient * light.color;

    // diffuse
    vec3 lightDirection = normalize(-light.direction);
    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * light.color;

    // specular
    int shininess = 32;
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float angularDistance = max(dot(viewDirection, reflectDirection), 0.0);
    vec3 specular = light.specular * pow(angularDistance, shininess) * light.color;

    fragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}