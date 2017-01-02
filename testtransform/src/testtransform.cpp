//============================================================================
// Name        : testtransform.cpp
// Author      : jccho@fine-tec.com
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <float.h>

#define POINT_INVALID FLT_MAX

using namespace std;

#define FLOAT_EPSILON 1E-5f

typedef unsigned int uint32_t;

template <typename T>
using Vector = std::vector<T>;

inline bool floatEquals(float lhs, float rhs)
{
  if((lhs - rhs > FLOAT_EPSILON) || (rhs - lhs > FLOAT_EPSILON))
    return false;
  else
    return true;
}

/* Point Class */
class Point
{
public:
  float x, y, z; // in meters

  Point(): x(0), y(0), z(0) {}
  Point(float x, float y): x(x), y(y), z(0) {}
  Point(float x, float y, float z): x(x), y(y), z(z) {}

  inline float angle(const Point &other) const;
  inline float norm() const;

  inline float dot(const Point &other) const;
  inline Point cross(const Point &other) const;

  inline Point &operator -();
  inline Point operator +(const Point &other) const;
  inline Point operator *(const Point &other) const;
  inline Point operator *(const float &other) const;
};

float Point::dot(const Point &other) const
{
  return (x*other.x + y*other.y + z*other.z);
}

Point Point::cross(const Point &other) const
{
  Point p;
  p.x = y*other.z - z*other.y;
  p.y = z*other.x - x*other.z;
  p.z = x*other.y - y*other.x;

  return p;
}


float Point::angle(const Point &other) const
{
  float n1 = norm(), n2 = other.norm();

  if(floatEquals(n1, 0) || floatEquals(n2, 0))
    return 0;

  return acos(dot(other)/n1/n2);
}

float Point::norm() const
{
  return sqrt(x*x + y*y + z*z);
}

inline Point &Point::operator -()
{
  x = -x;
  y = -y;
  z = -z;
  return *this;
}

Point Point::operator*(const Point &other) const
{
  return Point(x*other.x, y*other.y, z*other.z);
}

Point Point::operator*(const float &other) const
{
  return Point(x*other, y*other, z*other);
}

Point Point::operator +(const Point &other) const
{
  return Point(x + other.x, y + other.y, z + other.z);
}



class IntensityPoint: public Point
{
public:
  float i; // normalized 0-1

  static IntensityPoint *typeCast(Point *ptr)
  {
    return (IntensityPoint *)(ptr);
  }
};

class RGBPoint: public Point
{
public:
  float r, g, b; // normalized 0-1

  static RGBPoint *typeCast(Point *ptr)
  {
    return (RGBPoint *)(ptr);
  }
};

class Orientation // in radial co-ordinates
{
public:
  float theta, phi; // in radians
};


class PointCloudTransform
{
public:
	  uint32_t left, top;              // Image top-left corner
	  uint32_t width, height;            // sensor width and height
	  uint32_t rowsToMerge, columnsToMerge; // Binning
	  float fx, fy;             // x- and y- focal distance
	  float cx, cy;             // image center
	  float k1, k2, k3;           // radial distortion parameters
	  float p1, p2;             // tangential distortion parameters

	  Vector<Point> directions;

	  Point &getDirection(int row, int col);
public:
	  PointCloudTransform(uint32_t left, uint32_t top, uint32_t width, uint32_t height,
			  	  	  	  uint32_t rowsToMerge, uint32_t columnsToMerge,
						  float fx, float fy, float cx, float cy,
						  float k1, float k2, float k3, float p1, float p2);
protected:
	  Point _screenToNormalizedScreen(const Point &screen, bool verify);
	  Point _normalizedScreenToUnitWorld(const Point &normalizedScreen);
	  void _init();
};
PointCloudTransform::PointCloudTransform(uint32_t left, uint32_t top, uint32_t width, uint32_t height,
		  	  	  	  	  	  	  	  	 uint32_t rowsToMerge, uint32_t columnsToMerge,
										  float fx, float fy, float cx, float cy,
										  float k1, float k2, float k3, float p1, float p2)
{
	  this->left = left;
	  this->top = top;
	  this->width = width;
	  this->height = height;
	  this->rowsToMerge = rowsToMerge;
	  this->columnsToMerge = columnsToMerge;
	  this->fx = fx;
	  this->fy = fy;
	  this->cx = cx;
	  this->cy = cy;
	  this->k1 = k1;
	  this->k2 = k2;
	  this->k3 = k3;
	  this->p1 = p1;
	  this->p2 = p2;
	  directions.clear();
	  _init();
}

Point &PointCloudTransform::getDirection(int row, int col)
{
	return directions[col*width + row];
}

void PointCloudTransform::_init()
{
	directions.clear();

	for (uint32_t v = top; v < top + height; v++)
	{
		for (uint32_t u =  left; u < left + width; u++)
		{
			Point normalizedScreen = _screenToNormalizedScreen(Point(u, v), true);
			Point dir = _normalizedScreenToUnitWorld(normalizedScreen);
			directions.push_back(dir);
		}
	}
}

Point PointCloudTransform::_screenToNormalizedScreen(const Point &screen, bool verify)
{
  int iters = 100;
  float xs, ys;
  float yss = ys = (screen.y - cy) / fy;
  float xss = xs = (screen.x - cx) / fx;

  for(int j = 0; j < iters; j++)
  {
    float r2 = xs * xs + ys * ys;
    float icdist = 1.0f / (1 + ((k3 * r2 + k2) * r2 + k1) * r2);
    float deltaX = 2 * p1 * xs * ys + p2 * (r2 + 2 * xs * xs);
    float deltaY = p1 * (r2 + 2 * ys * ys) + 2 * p2 * xs * ys;
    xs = (xss - deltaX)*icdist;
    ys = (yss - deltaY)*icdist;
  }

  if(verify)
  {
    float x_ = xs;
    float y_ = ys;
    float r2 = x_ * x_ + y_ * y_;
    float r4 = r2 * r2;
    float r6 = r2 * r4;

    float x__ = x_ * (1.0f + k1 * r2 + k2 * r4 + k3 * r6) + 2.0f * p1 * x_ * y_ + p2 * (r2 + 2.0f * x_ * x_);
    float y__ = y_ * (1.0f + k1 * r2 + k2 * r4 + k3 * r6) + p1 * (r2 + 2.0f * y_ * y_) + 2.0f * p2 * x_ * y_;

    if(fabs(x__ - xss) > FLOAT_EPSILON || fabs(y__ - yss) > FLOAT_EPSILON)
    {
      return Point(POINT_INVALID, 0);
    }
    else
    {
      return Point(xs, ys);
    }
  }
  else
  {
    return Point(xs, ys);
  }
}

Point PointCloudTransform::_normalizedScreenToUnitWorld(const Point &normalizedScreen)
{
	  float _norm = 1.0f / (float)sqrt(normalizedScreen.x * normalizedScreen.x
	                                   + normalizedScreen.y * normalizedScreen.y
	                                   + 1.0f);
	  return Point(normalizedScreen.x * _norm, normalizedScreen.y * _norm, _norm);
}

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	ios state(nullptr);

	PointCloudTransform pct(0, 0, 320, 240,
							1, 1,
							226.304001, 225.347000, 158.091995, 117.942001,
							-0.173509002, 0.0715638027, 0.0268313009, 9.89991968e-05, 0.000356030010);

	ofstream outfile("result.txt");
	state.copyfmt(outfile);
	for (int row = 0; row < 240; row++)
	{
		for (int col = 0; col < 320; col++)
		{
			Point p;

			p = pct.getDirection(col, row);
			outfile << "("
					<< setw(3) << col
					<< ","
					<< setw(3) << row
					<< ")= "
					<< setw(12) << setprecision(5) << p.x
					<< ","
					<< setw(12) << setprecision(5) << p.y
					<< ","
					<< setw(12) << setprecision(5) << p.z
					<< endl;
		}
	}

	outfile.close();

	return 0;
}
