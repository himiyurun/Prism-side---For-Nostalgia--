#version 150 core
out vec4 fragment;
in vec2 tc;
uniform sampler2D tex;
void main()
{
    fragment = texture(tex, tc);
}