#version 330 core
in vec2 texCoord;
out vec4 fragColor;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
void main() {
    // fragColor = texture(texSampler1, texCoord);
    fragColor = mix(texture(texSampler1, texCoord), texture(texSampler2, texCoord), 0.2);

    // FragColor = texture(texSampler, texCoord) * vertexColor;
}