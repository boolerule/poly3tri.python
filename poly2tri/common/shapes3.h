/* 
 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
 * http://code.google.com/p/poly2tri/
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * * Neither the name of Poly2Tri nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Include guard
#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include <cstddef>
#include <assert.h>
#include <cmath>
#include <exception>

namespace p3t {

struct Node;
struct Edge3;


struct Point3 {

  double x, y, z;

  /// 默认构造函数不做任何事情(为了性能)。
  Point3()
  {
    x = 0.0;
    y = 0.0;
    z = 0.0;
  }

  /// 这一点构成了一个上端点
  std::vector<Edge3*> edge_list;

  /// 使用坐标构造。
  Point3(double x, double y, double z) : x(x), y(y), z(z) {}

  /// 将这个点设置为所有的0
  void set_zero()
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }

  /// 将这个点设置为某个指定的坐标。
  void set(double x_, double y_, double z_)
  {
    x = x_;
    y = y_;
    z = z_;
  }

  /// 否定这一点。
  Point3 operator -() const
  {
    Point3 v;
    v.set(-x, -y, -z);
    return v;
  }

  /// 在这一点上加一点。
  void operator +=(const Point3& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
  }

  /// 这个点减去1。
  void operator -=(const Point3& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
  }

  /// 用标量乘以这个点
  void operator *=(double a)
  {
    x *= a;
    y *= a;
    z *= a;
  }

  /// 得到这个点的长度(范数)。
  double Length() const
  {
    return sqrt(x * x + y * y + z * z);
  }

  /// 把这个点转换成一个单位点。返回长度
  double Normalize()
  {
    double len = Length();
    x /= len;
    y /= len;
    z /= len;
    return len;
  }

};
//重复点异常
class RepeatedPointException : public std::exception {
	virtual const char * what() const throw() {
		return "Repeated Point3";
	}
};
// Represents a simple polygon's edge
//表示一个简单多边形的边缘
//TODO:虽然标了一个3但是其实还是二维运行，只是会带着一个z的坐标值，因为我们的是等高线，所以需要这个
struct Edge3 {

  Point3* p, *q;

  /// Constructor
  Edge3(Point3& p1, Point3& p2) : p(&p1), q(&p2)
  {
    if (p1.y > p2.y) {
      q = &p1;
      p = &p2;
    } else if (p1.y == p2.y) {
      if (p1.x > p2.x) {
        q = &p1;
        p = &p2;
      } else if (p1.x == p2.x) {
        // Repeat points
	throw new RepeatedPointException();
        assert(false);
      }
    }

    q->edge_list.push_back(this);
  }
};
// Triangle3-based data structures are know to have better performance than quad-edge structures
// See: J. Shewchuk, "Triangle3: Engineering a 2D Quality Mesh Generator and Delaunay Triangulator"
//      "Triangulations in CGAL"
class Triangle3 {
public:

/// Constructor
Triangle3(Point3& a, Point3& b, Point3& c);

/// Flags to determine if an edge is a Constrained edge
bool constrained_edge[3];
/// Flags to determine if an edge is a Delauney edge
bool delaunay_edge[3];

Point3* GetPoint(const int& index);
Point3* PointCW(Point3& point);
Point3* PointCCW(Point3& point);
Point3* OppositePoint(Triangle3& t, Point3& p);

Triangle3* GetNeighbor(const int& index);
void MarkNeighbor(Point3* p1, Point3* p2, Triangle3* t);
void MarkNeighbor(Triangle3& t);

void MarkConstrainedEdge(const int index);
void MarkConstrainedEdge(Edge3& edge);
void MarkConstrainedEdge(Point3* p, Point3* q);

int Index(const Point3* p);
int EdgeIndex(const Point3* p1, const Point3* p2);

Triangle3* NeighborCW(Point3& point);
Triangle3* NeighborCCW(Point3& point);
bool GetConstrainedEdgeCCW(Point3& p);
bool GetConstrainedEdgeCW(Point3& p);
void SetConstrainedEdgeCCW(Point3& p, bool ce);
void SetConstrainedEdgeCW(Point3& p, bool ce);
bool GetDelunayEdgeCCW(Point3& p);
bool GetDelunayEdgeCW(Point3& p);
void SetDelunayEdgeCCW(Point3& p, bool e);
void SetDelunayEdgeCW(Point3& p, bool e);

bool Contains(Point3* p);
bool Contains(const Edge3& e);
bool Contains(Point3* p, Point3* q);
void Legalize(Point3& point);
void Legalize(Point3& opoint, Point3& npoint);
void ClearNeighbors();
void ClearDelunayEdges();

inline bool IsInterior();
inline void IsInterior(bool b);

Triangle3& NeighborAcross(Point3& opoint);

void DebugPrint();

private:

/// Triangle3 points
Point3* points_[3];
/// Neighbor list
Triangle3* neighbors_[3];

/// Has this triangle been marked as an interior triangle?
bool interior_;
};

inline bool cmp(const Point3* a, const Point3* b)
{
  if (a->y < b->y) {
    return true;
  } else if (a->y == b->y) {
    // Make sure q is point with greater x value
    if (a->x < b->x) {
      return true;
    }
  }
  return false;
}

/// Add two points_ component-wise.
inline Point3 operator +(const Point3& a, const Point3& b)
{
  return Point3(a.x + b.x, a.y + b.y, a.z + b.z);
}

/// Subtract two points_ component-wise.
inline Point3 operator -(const Point3& a, const Point3& b)
{
  return Point3(a.x - b.x, a.y - b.y, a.z - b.z);
}

/// Multiply point by scalar
inline Point3 operator *(double s, const Point3& a)
{
  return Point3(s * a.x, s * a.y, s*a.z);
}

inline bool operator ==(const Point3& a, const Point3& b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator !=(const Point3& a, const Point3& b)
{
  return a.x != b.x && a.y != b.y && a.z != b.z;
}

///在两个向量上做点积。
inline double Dot(const Point3& a, const Point3& b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// 对两个向量做叉乘。在二维中这产生一个标量。 在三维中这产生一个向量点
//(ax,ay,az)x(bx,by,bz)=(aybz-azby,azbx-axbz,axby-aybx)
inline Point3 Cross(const Point3& a, const Point3& b)
{
  return Point3(a.y * b.z - a.z * b.y, a.z*b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

/// 对点和标量做叉乘。在二维空间中
/// a point.
inline Point3 Cross(const Point3& a, double s)
{
  return Point3(s * a.y, -s * a.x,a.z);
}

/// Perform the cross product on a scalar and a point. In 2D this produces
/// a point.
//对一个标量和一个点做叉乘。在2D中，这会产生一个点。
inline Point3 Cross(const double s, const Point3& a)
{
  return Point3(-s * a.y, s * a.x,a.z);
}

inline Point3* Triangle3::GetPoint(const int& index)
{
  return points_[index];
}

inline Triangle3* Triangle3::GetNeighbor(const int& index)
{
  return neighbors_[index];
}

inline bool Triangle3::Contains(Point3* p)
{
  return p == points_[0] || p == points_[1] || p == points_[2];
}

inline bool Triangle3::Contains(const Edge3& e)
{
  return Contains(e.p) && Contains(e.q);
}

inline bool Triangle3::Contains(Point3* p, Point3* q)
{
  return Contains(p) && Contains(q);
}

inline bool Triangle3::IsInterior()
{
  return interior_;
}

inline void Triangle3::IsInterior(bool b)
{
  interior_ = b;
}

}

#endif


