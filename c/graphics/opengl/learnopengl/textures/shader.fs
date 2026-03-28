#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D texSampler;
void main() {
    FragColor = texture(texSampler, texCoord);
    // FragColor = texture(texSampler, texCoord) * vertexColor;
}