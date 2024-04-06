#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 Color;
  
uniform mat4 uProjection;
uniform vec4 uColor;

void main()
{
    gl_Position = uProjection * vec4(aPos, 1.0f);
    Color = uColor; 
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
} 