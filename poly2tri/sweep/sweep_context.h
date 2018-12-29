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

#ifndef SWEEP_CONTEXT_H
#define SWEEP_CONTEXT_H

#include <cstddef>
#include <list>
#include <vector>

namespace p3t {

  // Inital triangle factor, seed triangle will extend 30% of
  // PointSet width to both left and right.
  const double kAlpha = 0.3;

  struct Point3;
  class Triangle3;
  struct Node;
  struct Edge3;
  class AdvancingFront;

  class SweepContext {
    public:

      /// Constructor
      SweepContext(std::vector<Point3*> polyline);
      /// Destructor
      ~SweepContext();

      void set_head(Point3* p1);

      Point3* head();

      void set_tail(Point3* p1);

      Point3* tail();

      int point_count();

      Node& LocateNode(Point3& point);

      void RemoveNode(Node* node);

      void CreateAdvancingFront(std::vector<Node*> nodes);

      /// Try to map a node to all sides of this triangle that don't have a neighbor
      void MapTriangleToNodes(Triangle3& t);

      void AddToMap(Triangle3* triangle);

      Point3* GetPoint(const int& index);

      Point3* GetPoints();

      void RemoveFromMap(Triangle3* triangle);

      void AddHole(std::vector<Point3*> polyline);

      void AddPoint(Point3* point);

      AdvancingFront* front();

      void MeshClean(Triangle3& triangle);

      std::vector<Triangle3*> GetTriangles();
      std::list<Triangle3*> GetMap();

      std::vector<Edge3*> edge_list;

      struct Basin {
        Node* left_node;
        Node* bottom_node;
        Node* right_node;
        double width;
        bool left_highest;

        Basin() : left_node(NULL), bottom_node(NULL), right_node(NULL), width(0.0), left_highest(false)
        {
        }

        void Clear()
        {
          left_node = NULL;
          bottom_node = NULL;
          right_node = NULL;
          width = 0.0;
          left_highest = false;
        }
      };

      struct EdgeEvent {
        Edge3* constrained_edge;
        bool right;

        EdgeEvent() : constrained_edge(NULL), right(false)
        {
        }
      };

      Basin basin;
      EdgeEvent edge_event;

    private:

      friend class Sweep;

      std::vector<Triangle3*> triangles_;
      std::list<Triangle3*> map_;
      std::vector<Point3*> points_;

      // Advancing front
      AdvancingFront* front_;
      // head point used with advancing front
      Point3* head_;
      // tail point used with advancing front
      Point3* tail_;

      Node *af_head_, *af_middle_, *af_tail_;

      void InitTriangulation();
      void InitEdges(std::vector<Point3*> polyline);

  };

  inline AdvancingFront* SweepContext::front()
  {
    return front_;
  }

  inline int SweepContext::point_count()
  {
    return points_.size();
  }

  inline void SweepContext::set_head(Point3* p1)
  {
    head_ = p1;
  }

  inline Point3* SweepContext::head()
  {
    return head_;
  }

  inline void SweepContext::set_tail(Point3* p1)
  {
    tail_ = p1;
  }

  inline Point3* SweepContext::tail()
  {
    return tail_;
  }

}

#endif
