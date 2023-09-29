#include "../include/Noise.h"


Noise::Noise()
{
	this->seed = 0;
	init();
}


Noise::Noise(double seed)
{
	this->seed = seed;
	init();
}


Noise::~Noise() 
{
	delete[] p;
}


void Noise::init() 
{
	p = new int[512];

	for (int i = 0; i < 256; i++)
	{
		p[256 + i] = p[i] = permutation[i];
	}
}


inline double Noise::lerp(double t, double a, double b) 
{ 
	return a + t * (b - a); 
}


inline double Noise::fade(double t) 
{ 
	return t * t * t * (t * (t * 6 - 15) + 10); 
}


inline double Noise::dot(Vector2 a, Vector2 b)
{
	return a.x * b.x + a.y * b.y;
}


Vector2 Noise::getConstantVector(int value)
{
	int res = value & 3;

	if (res == 0)
		return Vector2{ 1.0, 1.0 };
	
	if (res == 1)
		return Vector2{ -1.0, 1.0 };
	
	if (res == 2)
		return Vector2{ -1.0, -1.0 };

	return Vector2{ 1.0, -1.0 };
}


double Noise::smoothNoise(double x, double y) 
{
	int X = ((int)x) & 255;
	int Y = ((int)y) & 255;

	double xf = x - (int)x;
	double yf = y - (int)y;

	Vector2    topRight = Vector2{ xf - 1.0, yf - 1.0 };
	Vector2     topLeft = Vector2{ xf, yf - 1.0 };
	Vector2 bottomRight = Vector2{ xf - 1.0, yf };
	Vector2  bottomLeft = Vector2{ xf, yf };

	int    valueTopRight = p[p[X + 1] + Y + 1];
	int     valueTopLeft = p[p[X]     + Y + 1];
	int valueBottomRight = p[p[X + 1] + Y    ];
	int  valueBottomLeft = p[p[X]     + Y    ];

	double    dotTopRight = dot(   topRight, getConstantVector(valueTopRight)   );
	double     dotTopLeft = dot(    topLeft, getConstantVector(valueTopLeft)    );
	double dotBottomRight = dot(bottomRight, getConstantVector(valueBottomRight));
	double  dotBottomLeft = dot( bottomLeft, getConstantVector(valueBottomLeft) );

	double u = fade(xf);
	double v = fade(yf);

	double res = lerp(u,
		lerp(v,  dotBottomLeft, dotTopLeft ),
		lerp(v, dotBottomRight, dotTopRight)
	);

	return (res + 1) * 0.5;
}


double Noise::noise2D(double x, double y, int nbOfOctaves)
{
	double result = 0.0;
	double amplitude = 1.0;
	double frequency = 0.005;

	for (int octave = 0; octave < nbOfOctaves; octave++) {
		double n = amplitude * smoothNoise(x * frequency, y * frequency);
		result += n;

		amplitude *= 0.5;
		frequency *= 2.0;
	}

	double finalRes = ((result + 1) * 0.5);
	return 1 < finalRes ? 1.0 : finalRes;
}
