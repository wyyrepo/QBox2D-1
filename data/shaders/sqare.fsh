#version 120

uniform vec4 color;
varying vec4 fragColor;
void main(void)
{
gl_FragColor = color;
}

