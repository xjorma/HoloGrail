#include "demopch.h"

#include "./headers/geometries.h"

Geometry Icosahedron = Geometry
(
	{
		{0.0, 0.0, -1.0}, {0.0, 0.894427359104, -0.44721314311}, {0.850650846958, 0.276393681765, -0.447213202715}, {0.52573120594, -0.723606944084, -0.44721326232}, {-0.52573120594, -0.723606944084, -0.44721326232}, {-0.850650846958, 0.276393681765, -0.447213202715}, {-0.52573120594, 0.723606944084, 0.44721326232}, {-0.850650846958, -0.276393681765, 0.447213202715},
		{0.0, -0.894427359104, 0.44721314311}, {0.850650846958, -0.276393681765, 0.447213202715}, {0.52573120594, 0.723606944084, 0.44721326232}, {0.0, 0.0, 1.0}
	},
	{
		{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 5}, {1, 6}, {1, 10}, {2, 3}, {2, 9}, {2, 10}, {3, 4}, {3, 8}, {3, 9}, {4, 5}, {4, 7}, {4, 8}, {5, 6}, {5, 7}, {6, 7}, {6, 10}, {6, 11}, {7, 8}, {7, 11}, {8, 9}, {8, 11}, {9, 10}, {9, 11}, {10, 11}
	}
);
