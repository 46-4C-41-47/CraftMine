#include "../include/Noise.h"


Noise::Noise() : seed{ 0 }
{
	init();
}


Noise::Noise(double s) : seed{ s } 
{
	init();
}


void Noise::init()
{
	for (int i = 0; i < 256; i++) {
		p[256 + i] = p[i] = permutation[i];
	}
}


double Noise::grad(int hash, double x, double y, double z)
{
    int h = hash & 15; // CONVERT LO 4 BITS OF HASH CODE
    double u = h < 8 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
    v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}


MyVec2 Noise::GetConstantVector(int v)
{
	int h = v & 3;

	if (h == 0)
		return MyVec2{ 1.0, 1.0 };
	
	if (h == 1)
		return MyVec2{ -1.0, 1.0 };
	
	if (h == 2)
		return MyVec2{-1.0, -1.0};
	
	return MyVec2{ 1.0, -1.0 };
}


double Noise::smoothNoise(double x, double y, double z) 
{
    int X = (int)floor(x) & 255,                  // FIND UNIT CUBE THAT
        Y = (int)floor(y) & 255,                  // CONTAINS POINT.
        Z = (int)floor(z) & 255;

    x -= floor(x);                                // FIND RELATIVE X,Y,Z
    y -= floor(y);                                // OF POINT IN CUBE.
    z -= floor(z);

    double u = fade(x),                                // COMPUTE FADE CURVES
           v = fade(y),                                // FOR EACH OF X,Y,Z.
           w = fade(z);

    int A = p[X    ] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,      // HASH COORDINATES OF
        B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;      // THE 8 CUBE CORNERS,

    return lerp(w,  lerp(v, lerp(u, grad(p[AA    ], x    , y    , z   ),    // AND ADD
                                    grad(p[BA    ], x - 1, y    , z    )),  // BLENDED
                            lerp(u, grad(p[AB    ], x    , y - 1, z    ),   // RESULTS
                                    grad(p[BB    ], x - 1, y - 1, z    ))), // FROM  8
                    lerp(v, lerp(u, grad(p[AA + 1], x    , y    , z - 1),   // CORNERS
                                    grad(p[BA + 1], x - 1, y    , z - 1)),  // OF CUBE
                            lerp(u, grad(p[AB + 1], x    , y - 1, z - 1),
                                    grad(p[BB + 1], x - 1, y - 1, z - 1))));
}


double Noise::classicNoise(double x, double y)
{
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;

	double xf = x - floor(x);
	double yf = y - floor(y);

	MyVec2 topRight    = { xf - 1.0, yf - 1.0 };
	MyVec2 topLeft     = { xf      , yf - 1.0 };
	MyVec2 bottomRight = { xf - 1.0, yf       };
	MyVec2 bottomLeft  = { xf      , yf       };

	// Select a value from the permutation array for each of the 4 corners
	int valueTopRight    = p[p[X + 1] + Y + 1];
	int valueTopLeft     = p[p[X    ] + Y + 1];
	int valueBottomRight = p[p[X + 1] + Y    ];
	int valueBottomLeft  = p[p[X    ] + Y    ];

	double dotTopRight = dot(topRight, GetConstantVector(valueTopRight));
	double dotTopLeft = dot(topLeft, GetConstantVector(valueTopLeft));
	double dotBottomRight = dot(bottomRight, GetConstantVector(valueBottomRight));
	double dotBottomLeft = dot(bottomLeft, GetConstantVector(valueBottomLeft));

	double u = fade(xf);
	double v = fade(yf);

	return lerp(u,
		lerp(v, dotBottomLeft, dotTopLeft),
		lerp(v, dotBottomRight, dotTopRight)
	);
}


double* Noise::detailed2DNoise(double x, double y, int resolution)
{
	double* res = new double[resolution * resolution];
	double cornerValue[4];

	cornerValue[0] = classicNoise(x    , y    );
	cornerValue[1] = classicNoise(x + 1, y    );
	cornerValue[2] = classicNoise(x    , y + 1);
	cornerValue[3] = classicNoise(x + 1, y + 1);

	double u, v, offset = 1.0 / resolution;

	for (int x = 0; x < resolution; x++)
	{
		for (int y = 0; y < resolution; y++)
		{
			u = fade(x * offset);
			v = fade(y * offset);

			res[x + y * resolution] = lerp(u,
				lerp(v, cornerValue[0], cornerValue[2]),
				lerp(v, cornerValue[1], cornerValue[3])
			);
		}
	}

	return res;
}
