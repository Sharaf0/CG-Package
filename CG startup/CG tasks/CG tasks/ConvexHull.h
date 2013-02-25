#ifndef CONVEXHULL_H
#define CONVEXHULL_H
#include "Point.h"
#include <vector>
#include <algorithm>
using namespace std;
class ConvexHull
{
public:
	float crossProduct(const Point &O, const Point &A, const Point &B)
	{return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);}

	vector<Point> convexHullAlgorithm(vector<Point> points)
	{
		int n = points.size(), k = 0;
		vector<Point> H(2*n);
		// Sort points lexicographically
		sort(points.begin(), points.end());
		// Build lower hull
		for (int i = 0; i < n; i++)
		{
			while (k >= 2 && crossProduct(H[k-2], H[k-1], points[i]) <= 0)
				k--;
			H[k++] = points[i];
		}
		// Build upper hull
		for (int i = n-2, t = k+1; i >= 0; i--)
		{
			while (k >= t && crossProduct(H[k-2], H[k-1], points[i]) <= 0) k--;
			H[k++] = points[i];
		}
		H.resize(k);
		return H;
	}
};
#endif // CONVEXHULL_H