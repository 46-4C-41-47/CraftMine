#include "../include/Noise.h"


Noise* Noise::instance = nullptr;


const int Noise::permutation[256] = {
		151,160,137, 91, 90, 15,131, 13,201, 95, 96, 53,194,233,  7,225,
		140, 36,103, 30, 69,142,  8, 99, 37,240, 21, 10, 23,190,  6,148,
		247,120,234, 75,  0, 26,197, 62, 94,252,219,203,117, 35, 11, 32,
		 57,177, 33, 88,237,149, 56, 87,174, 20,125,136,171,168, 68,175,
		 74,165, 71,134,139, 48, 27,166, 77,146,158,231, 83,111,229,122,
		 60,211,133,230,220,105, 92, 41, 55, 46,245, 40,244,102,143, 54,
		 65, 25, 63,161,  1,216, 80, 73,209, 76,132,187,208, 89, 18,169,
		200,196,135,130,116,188,159, 86,164,100,109,198,173,186,  3, 64,
		 52,217,226,250,124,123,  5,202, 38,147,118,126,255, 82, 85,212,
		207,206, 59,227, 47, 16, 58, 17,182,189, 28, 42,223,183,170,213,
		119,248,152,  2, 44,154,163, 70,221,153,101,155,167, 43,172,  9,
		129, 22, 39,253, 19, 98,108,110, 79,113,224,232,178,185,112,104,
		218,246, 97,228,251, 34,242,193,238,210,144, 12,191,179,162,241,
		 81, 51,145,235,249, 14,239,107, 49,192,214, 31,181,199,106,157,
		184, 84,204,176,115,121, 50, 45,127,  4,150,254,138,236,205, 93,
		222,114, 67, 29, 24, 72,243,141,128,195, 78, 66,215, 61,156,180
};


Noise* Noise::getInstance()
{
	if (instance == nullptr)
		instance = new Noise(params::noise::SEED);

	return instance;
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

	cornerValue[0] = smoothNoise(x    , y    , 0);
	cornerValue[1] = smoothNoise(x + 1, y    , 0);
	cornerValue[2] = smoothNoise(x    , y + 1, 0);
	cornerValue[3] = smoothNoise(x + 1, y + 1, 0);

	double u, v, w, offset = 1.0 / resolution;

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


double* Noise::detailed3DNoise(double x, double y, double z, int resolution, int height)
{
	double* res = new double[resolution * resolution * height];
	double cornerValue[8];

	// haut
	cornerValue[0] = smoothNoise(x    , y    , z + 1);
	cornerValue[1] = smoothNoise(x + 1, y    , z + 1);
	cornerValue[2] = smoothNoise(x    , y + 1, z + 1);
	cornerValue[3] = smoothNoise(x + 1, y + 1, z + 1);

	// bas
	cornerValue[4] = smoothNoise(x    , y    , z);
	cornerValue[5] = smoothNoise(x + 1, y    , z);
	cornerValue[6] = smoothNoise(x    , y + 1, z);
	cornerValue[7] = smoothNoise(x + 1, y + 1, z);

	double u, v, offset = 1.0 / resolution;

	for (int X = 0; X < resolution; X++)
	{
		for (int Y = 0; Y < resolution; Y++)
		{
			for (int Z = 0; Z < height; Z++)
			{
				u = fade(X * offset);
				v = fade(Y * offset);

				res[X + Y * resolution] = lerp(u,
					lerp(v, cornerValue[0], cornerValue[2]),
					lerp(v, cornerValue[1], cornerValue[3])
				);
			}
		}
	}

	return res;
}
