#version 330 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 fNormal;
in vec2 texCoor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D texture1;
uniform float ambientStrength;

void main()
{
    vec3 lightDir = normalize(lightPos - fragPos); 
    float colorStrength = max(dot(fNormal, lightDir), 0.0);

    float c = ambientStrength + ((1 - ambientStrength) * colorStrength);

    FragColor = texture(texture1, texCoor) * vec4(lightColor, 1.0) * c;
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
} 
