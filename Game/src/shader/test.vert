#version 150 core
in vec4 position;

uniform vec2 size;
uniform vec2 location;
uniform float scale;

void main()
{
    gl_Position = position * vec4( scale / size, 1.0, 1.0 ) + vec4( location, 0.0, 0.0);
}