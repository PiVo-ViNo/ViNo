#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D uText;
uniform vec3 uTextColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uText, TexCoords).r);
    FragColor = vec4(uTextColor, 1.0) * sampled;
}  