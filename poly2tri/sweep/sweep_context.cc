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
#include "sweep_context.h"
#include <algorithm>
#include "advancing_front.h"

#define nullptr 0L

namespace p3t {

SweepContext::SweepContext(std::vector<Point3*> polyline) :
	front_(nullptr), head_(nullptr), tail_(nullptr), af_head_(nullptr), af_middle_(nullptr), af_tail_(nullptr)
{
  basin = Basin();
  edge_event = EdgeEvent();

  points_ = polyline;

  InitEdges(points_);
}

void SweepContext::AddHole(std::vector<Point3*> polyline)
{
  InitEdges(polyline);
  for(unsigned int i = 0; i < polyline.size(); i++) {
    points_.push_back(polyline[i]);
  }
}

void SweepContext::AddPoint(Point3* point) {
  points_.push_back(point);
}

std::vector<Triangle3*> SweepContext::GetTriangles()
{
  return triangles_;
}

std::list<Triangle3*> SweepContext::GetMap()
{
  return map_;
}

void SweepContext::InitTriangulation()
{
  double xmax(points_[0]->x), xmin(points_[0]->x);
  double ymax(points_[0]->y), ymin(points_[0]->y);
  double zmax(points_[0]->z), zmin(points_[0]->z);

  // Calculate bounds.
  for (unsigned int i = 0; i < points_.size(); i++) {
    Point3& p = *points_[i];
    if (p.x > xmax)
      xmax = p.x;
    if (p.x < xmin)
      xmin = p.x;
    if (p.y > ymax)
      ymax = p.y;
    if (p.y < ymin)
      ymin = p.y;
    if (p.z > zmax)
      zmax = p.y;
    if (p.z < zmin)
      zmin = p.y;
  }

  double dx = kAlpha * (xmax - xmin);
  double dy = kAlpha * (ymax - ymin);
  double zy = kAlpha * (zmax - zmin);
  head_ = new Point3(xmax + dx, ymin - dy, zmin - zy);
  tail_ = new Point3(xmin - dx, ymin - dy, zmin - zy);

  // Sort points along y-axis
  std::sort(points_.begin(), points_.end(), cmp);

}

void SweepContext::InitEdges(std::vector<Point3*> polyline)
{
  unsigned int num_points = polyline.size();
  for (unsigned int i = 0; i < num_points; i++) {
    int j = i < num_points - 1 ? i + 1 : 0;
    edge_list.push_back(new Edge3(*polyline[i], *polyline[j]));
  }
}

Point3* SweepContext::GetPoint(const int& index)
{
  return points_[index];
}

void SweepContext::AddToMap(Triangle3* triangle)
{
  map_.push_back(triangle);
}

Node& SweepContext::LocateNode(Point3& point)
{
  // TODO implement search tree
  return *front_->LocateNode(point.x);
}

void SweepContext::CreateAdvancingFront(std::vector<Node*> nodes)
{

  // Initial triangle
  Triangle3* triangle = new Triangle3(*points_[0], *tail_, *head_);

  map_.push_back(triangle);

  af_head_ = new Node(*triangle->GetPoint(1), *triangle);
  af_middle_ = new Node(*triangle->GetPoint(0), *triangle);
  af_tail_ = new Node(*triangle->GetPoint(2));
  front_ = new AdvancingFront(*af_head_, *af_tail_);

  // TODO: More intuitive if head is middles next and not previous?
  //       so swap head and tail
  af_head_->next = af_middle_;
  af_middle_->next = af_tail_;
  af_middle_->prev = af_head_;
  af_tail_->prev = af_middle_;
}

void SweepContext::RemoveNode(Node* node)
{
  delete node;
}

void SweepContext::MapTriangleToNodes(Triangle3& t)
{
  for (int i = 0; i < 3; i++) {
    if (!t.GetNeighbor(i)) {
      Node* n = front_->LocatePoint(t.PointCW(*t.GetPoint(i)));
      if (n)
        n->triangle = &t;
    }
  }
}

void SweepContext::RemoveFromMap(Triangle3* triangle)
{
  map_.remove(triangle);
}

void SweepContext::MeshClean(Triangle3& triangle)
{
  if (&triangle != NULL && !triangle.IsInterior()) {
    triangle.IsInterior(true);
    triangles_.push_back(&triangle);
    for (int i = 0; i < 3; i++) {
      if (!triangle.constrained_edge[i])
        MeshClean(*triangle.GetNeighbor(i));
    }
  }
}

SweepContext::~SweepContext()
{

    // Clean up memory

    delete head_;
    delete tail_;
    delete front_;
    delete af_head_;
    delete af_middle_;
    delete af_tail_;

    typedef std::list<Triangle3*> type_list;

    for(type_list::iterator iter = map_.begin(); iter != map_.end(); ++iter) {
        Triangle3* ptr = *iter;
        delete ptr;
    }

     for(unsigned int i = 0; i < edge_list.size(); i++) {
        delete edge_list[i];
    }

}

}
