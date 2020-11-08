#version 330 core

//                            local coord
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 texPos;

uniform vec2 posOffset;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexPos;
void main()
{
    TexPos = texPos;
    //gl_Position = projection * view * vec4(pos.x + posOffset.x, pos.y + posOffset.y, pos.z, 1.0);
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    //gl_Position = vec4(pos, 1.0f);
}
