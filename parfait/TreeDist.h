#pragma once
#include <vector>
#include <parfait/Point.h>
#include <parfait/Extent.h>
#include <parfait/ExtentBuilder.h>
#include <parfait/Facet.h>
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
  inline bool extentSphereIntersection(const Parfait::Extent<double>& e, const Parfait::Point<double>& center, const double radius){
      auto p = getClosestPointInExtent(e, center);
      double dist = (p - center).magnitude();
      return (dist < radius);
  }

  class DistanceTree {
  public:
      typedef std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> PriorityQueue;
      class CurrentState{
      public:
          CurrentState(const Parfait::Point<double> q):
              query_point(q),
              found_on_surface(false),
              actual_distance(std::numeric_limits<double>::max()){
          }
          bool isPotentiallyCloser(const Parfait::Point<double> p) const {
              double relative_percent_bounds = 0.0;
              double d = (p - query_point).magnitude();
              return (d < (1.0-relative_percent_bounds)*actual_distance);
          }
          void changeLocationIfCloser(const Parfait::Point<double> p){
              found_on_surface = true;
              double d = (p - query_point).magnitude();
              if(d < actual_distance){
                  actual_distance = d;
                  surface_location = p;
              }
          }
          double actualDistance() const {
              return actual_distance;
          }
          Parfait::Point<double> surfaceLocation() const {
              return surface_location;
          }
      private:
          Parfait::Point<double> query_point;
          Parfait::Point<double> surface_location;
          double actual_distance = std::numeric_limits<double>::max();
          bool found_on_surface;
      };
      class Node {
      public:
          enum {EMPTY = -11};
          inline Node(const Parfait::Extent<double>& e, int depth)
              : extent(e), depth(depth) {
          }
          Parfait::Extent<double> extent;
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
      DistanceTree(const Parfait::Extent<double>& e) :root_extent(e){
      }

      inline void insert(const Parfait::Facet& f){
          if(locked) throw std::logic_error("Cannot insert after calling contractExtents()");
          if(voxels.size() == 0)
              initializeRoot();
          int facet_id = facets.size();
          facets.push_back(f);
          insert(0, f, facet_id);
      }

      inline void setMaxDepth(int depth){
          max_depth = depth;
      }

      inline void contractExtents(){
          locked = true;
          if(voxels.size() == 0) return;
          contraceExtents(0);
      }

      inline void pruneEmpty(){
          if(voxels.size() == 0) return;
          Tracer::begin("pruneEmpty");
          pruneVoxel(0);
          Tracer::end("pruneEmpty");
      }

      inline Parfait::Point<double> closestPoint(const Parfait::Point<double>& p) const {
          PriorityQueue process;
          process.push(std::make_pair(std::numeric_limits<double>::max(), 0));
          double m = std::numeric_limits<double>::max();

          CurrentState current_state(p);

          while(not process.empty()) {
              current_state = closestPoint(p, current_state, process);
              if(current_state.actualDistance() < process.top().first) {
                  break;
              }
          }
          return current_state.surfaceLocation();
      }

  private:
      int max_depth = 2;
      bool locked = false;
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
              voxels.at(voxel_index).children[i] = before_voxel_count + i;
              voxels.push_back(Node(voxels.at(voxel_index).childExtent(i), voxels.at(voxel_index).depth + 1));
          }
      }

      inline void insert(int voxel_index, const Parfait::Facet &f, int facet_index) {
          const auto extent = voxels.at(voxel_index).extent;
          if(f.intersects(extent)) {
              if (voxels.at(voxel_index).depth == max_depth)
                  voxels.at(voxel_index).inside_facets.push_back(facet_index);
              else {
                  if (voxels.at(voxel_index).isLeaf()) {
                      splitVoxel(voxel_index);
                  }
                  auto children = voxels.at(voxel_index).children;
                  for (auto child : children) {
                      if(child != Node::EMPTY)
                          insert(child, f, facet_index);
                  }
              }
          }
      }

      inline CurrentState closestPoint(const Parfait::Point<double> &query_point,
                                       const CurrentState& current_state,
                                       PriorityQueue &process) const {

          int voxel_index = process.top().second;
          process.pop();
          if(voxels.size() == 0) return current_state;

          auto closest_possible_to_voxel = getClosestPointInExtent(voxels.at(voxel_index).extent, query_point);
          if(not current_state.isPotentiallyCloser(closest_possible_to_voxel)){
              return current_state;
          }

          if(voxels.at(voxel_index).isLeaf()) {
              return getClosestPointInLeaf(voxel_index, query_point, current_state);
          }

          for (const auto &child : voxels.at(voxel_index).children) {
              if (child != Node::EMPTY) {
                  const auto &child_extent = voxels.at(child).extent;
                  auto p = getClosestPointInExtent(child_extent, query_point);
                  double dist = (p - query_point).magnitude();
                  process.push(std::make_pair(dist, child));
              }
          }
          return current_state;
      }

      CurrentState getClosestPointInLeaf(int voxel_index,
                                         const Point<double> &query_point,
                                         CurrentState current_state ) const {
          for (const auto f : voxels.at(voxel_index).inside_facets) {
              auto p = facets[f].GetClosestPoint(query_point);
              current_state.changeLocationIfCloser(p);
          }
          return current_state;
      }

      inline Parfait::Extent<double> contraceExtents(int voxel_index){

          auto extent = determineShrunkExtent(voxel_index);
          voxels.at(voxel_index).extent = extent;
          return extent;
      }

      Extent<double> determineShrunkExtent(int voxel_index) {
          if(voxels.at(voxel_index).isLeaf())
              return determineShrunkExtentLeaf(voxel_index);
          else
              return determineShrunkExtentChildren(voxel_index);
      }

      Extent<double> determineShrunkExtentChildren(int voxel_index) {
          auto extent = ExtentBuilder::createEmptyBuildableExtent(Extent<double>());
          for(auto& child : voxels.at(voxel_index).children){
              if(child != Node::EMPTY){
                  ExtentBuilder::expandExtentWithAnother(extent, contraceExtents(child));
              }
          }
          if(extent.hi[0] == std::numeric_limits<double>::min())
              extent = voxels.at(voxel_index).extent;
          return extent;
      }

      Extent<double> determineShrunkExtentLeaf(int voxel_index) const {
          auto extent = ExtentBuilder::createEmptyBuildableExtent(Extent<double>());
          for(auto& facet_index : voxels.at(voxel_index).inside_facets){
              const auto& facet = facets[facet_index];
              for(int i = 0; i < 3; i++)
                  ExtentBuilder::addPointToExtent(extent, facet[i]);
          }
          if(extent.hi[0] == std::numeric_limits<double>::min()){
              auto c = voxels.at(voxel_index).extent.center();
              extent = {c, c};
          }
          return extent;
      }

      bool pruneVoxel(int voxel_index){
          if(voxels.at(voxel_index).isLeaf()){
              if(voxels.at(voxel_index).inside_facets.size() == 0){
                  return true;
              }
              return false;
          } else {
              bool prune = true;
              for(auto& child : voxels.at(voxel_index).children) {
                  if (child != Node::EMPTY) {
                      if (pruneVoxel(child)) {
                          child = Node::EMPTY;
                      } else {
                          prune = false;
                      }
                  }
              }
              return prune;
          }
      }
  };
}
