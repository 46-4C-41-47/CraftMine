#version 330 core
layout (location = 0) in vec3 cubePos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 pos;
layout (location = 4) in int texOffset;

out vec2 texCoor;
out vec3 fNormal;
out vec3 fragPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * (vec4(pos + cubePos, 1.0));
   fragPos = pos;
   texCoor = tex;
   fNormal = vNormal;
}
