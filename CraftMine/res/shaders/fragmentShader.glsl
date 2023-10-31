#version 330 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 fNormal;
in vec2 texCoor;

uniform vec3 fogColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float farPlane;
uniform float nearPlane;
uniform float ambientStrength;
uniform sampler2D texture1;


float fogValue(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return ((2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane))) / farPlane;	
}


void main()
{
    vec3 lightDir = normalize(lightPos - fragPos); 
    float colorStrength = max(dot(fNormal, lightDir), 0.0);

    float c = ambientStrength + ((1 - ambientStrength) * colorStrength);

    FragColor = mix(texture(texture1, texCoor) * vec4(lightColor, 1.0) * c, vec4(fogColor, 1.0), fogValue(gl_FragCoord.z));
} 
