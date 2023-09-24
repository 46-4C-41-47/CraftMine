#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 texture;

out vec2 texCoor;
out vec3 fNormal;
out vec3 fragPos;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(pos, 1.0);
   fragPos = vec3(model * vec4(pos, 1.0));
   texCoor = texture;
   fNormal = vNormal;
}
