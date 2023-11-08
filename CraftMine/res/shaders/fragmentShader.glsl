#version 330 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 fNormal;
in vec2 texCoor;

uniform vec3 fogColor;
uniform vec2 fogStrength;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;

uniform float farPlane;
uniform float nearPlane;

uniform sampler2D texture1;


float A = 0.0;
float C = 1.0;


float linearZ(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return ((2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane))) / farPlane;
}


// Cette fonction est une courbe de Bézier quadratique dont fogStrength est le point de controle
float fogValue(float x) 
{
    float t = (fogStrength.x + sqrt(fogStrength.x * fogStrength.x - 2 * x * fogStrength.x + x)) / 2 * fogStrength.x - 1;
    return 2 * C * (1 - t) * t + t * t;
}


void main()
{
    vec3 lightDir = normalize(lightPos - fragPos); 
    float colorStrength = max(dot(fNormal, lightDir), 0.0);

    float c = ambientStrength + ((1 - ambientStrength) * colorStrength);

    // gl_FragCoord.z
    FragColor = mix(texture(texture1, texCoor) * vec4(lightColor, 1.0) * c, vec4(fogColor, 1.0), 0);
} 
