#version 330 core
  
in vec2 TexCoord;
in vec4 Color;

uniform sampler2D uTexture;
uniform float uAlpha = -1.0f;

void main()
{
    ivec2 dims = textureSize(uTexture, 0);
    if (dims.x == 1 && dims.y == 1) {
        gl_FragColor = Color;
    } else {
        gl_FragColor = texture(uTexture, TexCoord) * Color; 
    }
    if (uAlpha != -1.0f) {
        gl_FragColor.a = uAlpha;
    }
}