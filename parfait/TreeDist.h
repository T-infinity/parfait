#pragma once
#include <vector>
#include <parfait/Point.h>
#include <parfait/Extent.h>
#include <Facet.h>
#include <limits>
#include <Tracer.h>
#include <queue>
#include <stack>

namespace Parfait {

  inline Parfait::Point<double> getClosestPointInExtent(const Parfait::Extent<double> &e, Parfait::Point<double> p){
      for(int i = 0; i < 3; i++){
          if(p[i] > e.hi[i]) p[i] = e.hi[i];
          else if(p[i] < e.lo[i]) p[i] = e.lo[i];
      }
      return p;
  }

  class OctTree {
  public:
      class Node {
      public:
          enum {EMPTY = -11};
          inline Node(const Parfait::Extent<double>& e, int depth)
              : extent(e), depth(depth) {
          }
          const Parfait::Extent<double> extent;
          const int depth;
          std::array<int, 8> children {
              EMPTY, EMPTY, EMPTY, EMPTY,
              EMPTY, EMPTY, EMPTY, EMPTY
          };

          std::vector<int> inside_facets;

          inline bool isLeaf() const {
              for(auto c : children)
                  if(c != EMPTY)
                      return false;
              return true;
          }
          inline Parfait::Extent<double> childExtent(int child) {
              switch(child){
                  case 0: return getChild0Extent(extent);
                  case 1: return getChild1Extent(extent);
                  case 2: return getChild2Extent(extent);
                  case 3: return getChild3Extent(extent);
                  case 4: return getChild4Extent(extent);
                  case 5: return getChild5Extent(extent);
                  case 6: return getChild6Extent(extent);
                  case 7: return getChild7Extent(extent);
                  default: throw std::logic_error("Voxels only have 8 children");
              }
          }
          inline Parfait::Extent<double> getChild0Extent(const Parfait::Extent<double> &extent){
              Parfait::Extent<double> childExtent = extent;
              childExtent.hi[0] = 0.5*(childExtent.lo[0] + childExtent.hi[0]);
              childExtent.hi[1] = 0.5*(childExtent.lo[1] + childExtent.hi[1]);
              childExtent.hi[2] = 0.5*(childExtent.lo[2] + childExtent.hi[2]);
              return childExtent;
          }

          inline Parfait::Extent<double> getChild1Extent(const Parfait::Extent<double> &extent){
              Parfait::Extent<double> childExtent = extent;
              childExtent.lo[0] = 0.5*(childExtent.lo[0] + childExtent.hi[0]);
              childExtent.hi[1] = 0.5*(childExtent.lo[1] + childExtent.hi[1]);
              childExtent.hi[2] = 0.5*(childExtent.lo[2] + childExtent.hi[2]);
              return childExtent;
          }

          inline Parfait::Extent<double> getChild2Extent(const Parfait::Extent<double> &extent){
              Parfait::Extent<double> childExtent = extent;
              childExtent.lo[0] = 0.5*(childExtent.lo[0] + childExtent.hi[0]);
              childExtent.lo[1] = 0.5*(childExtent.lo[1] + childExtent.hi[1]);
              childExtent.hi[2] = 0.5*(childExtent.lo[2] + childExtent.hi[2]);
              return childExtent;
          }

          inline Parfait::Extent<double> getChild3Extent(const Parfait::Extent<double> &extent){
              Parfait::Extent<double> childExtent = extent;
              childExtent.lo[1] = 0.5*(childExtent.lo[1] + childExtent.hi[1]);
              childExtent.hi[0] = 0.5*(childExtent.lo[0] + childExtent.hi[0]);
              childExtent.hi[2] = 0.5*(childExtent.lo[2] + childExtent.hi[2]);
              return childExtent;
          }

          inline Parfait::Extent<double> getChild4Extent(const Parfait::Extent<double> &extent){
              Parfait::Extent<double> childExtent = extent;
              childExtent.hi[0] = 0.5*(childExtent.lo[0] + childExtent.hi[0]);
              childExtent.hi[1] = 0.5*(childExtent.lo[1] + childExtent.hi[1]);
              childExtent.lo[2] = 0.5*(childExtent.lo[2] + childExtent.hi[2]);
              return childExtent;
          }

          inline Parfait::Extent<double> getChild5Extent(const Parfait::Extent<double> &extent){
              Parfait::Extent<double> childExtent = extent;
              childExtent.lo[0] = 0.5*(childExtent.lo[0] + childExtent.hi[0]);
              childExtent.hi[1] = 0.5*(childExtent.lo[1] + childExtent.hi[1]);
              childExtent.lo[2] = 0.5*(childExtent.lo[2] + childExtent.hi[2]);
              return childExtent;
          }

          inline Parfait::Extent<double> getChild6Extent(const Parfait::Extent<double> &extent){
              Parfait::Extent<double> childExtent = extent;
              childExtent.lo[0] = 0.5*(childExtent.lo[0] + childExtent.hi[0]);
              childExtent.lo[1] = 0.5*(childExtent.lo[1] + childExtent.hi[1]);
              childExtent.lo[2] = 0.5*(childExtent.lo[2] + childExtent.hi[2]);
              return childExtent;
          }

          inline Parfait::Extent<double> getChild7Extent(const Parfait::Extent<double> &extent){
              Parfait::Extent<double> childExtent = extent;
              childExtent.lo[1] = 0.5*(childExtent.lo[1] + childExtent.hi[1]);
              childExtent.hi[0] = 0.5*(childExtent.lo[0] + childExtent.hi[0]);
              childExtent.lo[2] = 0.5*(childExtent.lo[2] + childExtent.hi[2]);
              return childExtent;
          }
      };
      OctTree(const Parfait::Extent<double>& e) :root_extent(e){
      }

      inline void insert(const Parfait::Facet& f){
          if(voxels.size() == 0)
              initializeRoot();
          int facet_id = facets.size();
          facets.push_back(f);
          insert(0, f, facet_id);
      }

      inline void setMaxDepth(int depth){
          max_depth = depth;
      }

      inline Parfait::Point<double> closestPoint(const Parfait::Point<double>& p) const {
          Tracer::begin("closestPoint");
          std::stack<int> process;
          process.push(0);
          double m = std::numeric_limits<double>::max();
          Parfait::Point<double> current_closest = {m,m,m};

          while(not process.empty()) {
              current_closest = closestPoint(p, current_closest, process);
          }
          Tracer::end("closestPoint");
          return current_closest;
      }
  private:
      int max_depth = 2;
      const Parfait::Extent<double> root_extent;
      std::vector<Node> voxels;
      std::vector<Parfait::Facet> facets;

      inline void initializeRoot() {
          int depth = 0;
          auto root = Node(root_extent, depth);
          voxels.push_back(root);
      }


      inline void splitVoxel(int voxel_index) {
          int before_voxel_count = voxels.size();
          for(int i = 0; i < 8; i++) {
              voxels[voxel_index].children[i] = before_voxel_count + i;
              voxels.push_back(Node(voxels[voxel_index].childExtent(i), voxels[voxel_index].depth + 1));
          }
      }

      inline void insert(int voxel_index, const Parfait::Facet &f, int facet_index) {
          const auto extent = voxels[voxel_index].extent;
          if(f.intersects(extent)) {
              if (voxels[voxel_index].depth == max_depth)
                  voxels[voxel_index].inside_facets.push_back(facet_index);
              else {
                  if (voxels[voxel_index].isLeaf()) {
                      splitVoxel(voxel_index);
                  }
                  for (auto &child : voxels[voxel_index].children) {
                      insert(child, f, facet_index);
                  }
              }
          }
      }

      inline Parfait::Point<double> closestPoint(const Parfait::Point<double> &query_point,
                                                 Parfait::Point<double> current_closest,
                                                 std::stack<int> &process) const {

          int voxel_index = process.top();
          process.pop();
          double min_distance = (query_point - current_closest).magnitude();

          double voxel_closest_possible_distance = getClosestPossibleDistanceToVoxel(query_point, voxel_index);
          if(voxel_closest_possible_distance > min_distance)
              return current_closest;

          if(voxels[voxel_index].isLeaf())
              return getClosestPointInLeaf(voxel_index, query_point, current_closest, min_distance);

          for (const auto &child : voxels[voxel_index].children)
              if (child != Node::EMPTY)
                  process.push(child);
          return current_closest;
      }
      double getClosestPossibleDistanceToVoxel(const Point<double> &query_point, int voxel_index) const {
          auto extent_closest = getClosestPointInExtent(voxels[voxel_index].extent, query_point);
          double extent_dist = (extent_closest - query_point).magnitude();
          return extent_dist;
      }

      Point<double> getClosestPointInLeaf(int voxel_index,
                                          const Point<double> &query_point,
                                          Point<double> &current_closest,
                                          double min_distance) const {
          for (const auto f : voxels[voxel_index].inside_facets) {
              auto p = facets[f].GetClosestPoint(query_point);
              double dist = (query_point - p).magnitude();
              if (dist < min_distance) {
                  min_distance = dist;
                  current_closest = p;
              }
          }
          return current_closest;
      }

  };

  inline bool extentSphereIntersection(const Parfait::Extent<double>& e, const Parfait::Point<double>& center, const double radius){
      double r2 = radius * radius;
      double dmin = 0;
      for( int i = 0; i < 3; i++ ) {
          if( center[i] < e.lo[i] ) dmin += std::pow((center[i] - e.lo[i]), 2);
          else if( center[i] > e.hi[i] ) dmin += std::pow((center[i] - e.hi[i]), 2);
      }
      return dmin <= r2;
  }
}


