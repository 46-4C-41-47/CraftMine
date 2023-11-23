#version 330 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 fNormal;
in vec2 texCoor;

uniform vec3 fogColor;
uniform float fogStrength;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;

uniform float farPlane;
uniform float nearPlane;

uniform float windowWidth;

uniform sampler2D texture1;


float A = 0.0;
float C = 1.0;


float linearZ() 
{
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - (gl_FragCoord.z * 2.0 - 1.0) * (farPlane - nearPlane));
}


// Cette fonction est une courbe de Bézier quadratique dont fogStrength est le point de controle
/*float fogValue(float x) 
{
    float t = (fogStrength.x + sqrt(fogStrength.x * fogStrength.x - 2 * x * fogStrength.x + x)) / 2 * fogStrength.x - 1;
    return 2 * C * (1 - t) * t + t * t;
}*/


float fogValue(float x)
{
    return 1 - sqrt(1 - pow(x, fogStrength));
}


float correctHorizontalFov()
{
    float correctFarPlane = sin(acos(abs((gl_FragCoord.x - (windowWidth * 0.5)) / (windowWidth * 0.5)))) * farPlane;
    return min(correctFarPlane, mix(0, farPlane, (linearZ() + nearPlane) / farPlane)) / correctFarPlane;
}


void main()
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float colorStrength = max(dot(fNormal, lightDir), 0.0);

    float lightStrength = ambientStrength + ((1 - ambientStrength) * colorStrength);

    FragColor = mix(texture(texture1, texCoor) * vec4(lightColor, 1.0) * lightStrength, vec4(fogColor, 1.0), fogValue(linearZ() / farPlane));
} 
