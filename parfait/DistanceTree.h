// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration.
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#pragma once
#include <vector>
#include <parfait/Point.h>
#include <parfait/Extent.h>
#include <parfait/ExtentBuilder.h>
#include <parfait/Facet.h>
#include <limits>
#include <queue>
#include <stack>
#include "Octree.h"

namespace Parfait {

  inline Parfait::Point<double> getClosestPointInExtent(const Parfait::Extent<double> &e, Parfait::Point<double> p){
      if(e.contains(p))
          return p;
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

 class DistanceTree : public Octree<Parfait::Facet> {
  public:
      typedef std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> PriorityQueue;
      class CurrentState {
      public:
          inline CurrentState(const Parfait::Point<double> q):
                  query_point(q),
                  actual_distance(std::numeric_limits<double>::max()),
                                  found_on_surface(false)
          {}
          inline bool isPotentiallyCloser(const Parfait::Point<double> p) const {
              double relative_percent_bounds = 0.0;
              double d = (p - query_point).magnitude();
              return (d < (1.0-relative_percent_bounds)*actual_distance);
          }
          inline void changeLocationIfCloser(const Parfait::Point<double> p){
              found_on_surface = true;
              double d = (p - query_point).magnitude();
              if(d < actual_distance){
                  actual_distance = d;
                  surface_location = p;
              }
          }
          inline double actualDistance() const {
              return actual_distance;
          }
          inline Parfait::Point<double> surfaceLocation() const {
              return surface_location;
          }
      private:
          Parfait::Point<double> query_point {0,0,0};
          Parfait::Point<double> surface_location {0,0,0};
          double actual_distance = std::numeric_limits<double>::max();
          bool found_on_surface;
      };
      inline DistanceTree(const Parfait::Extent<double>& e) {
          auto center = (e.hi + e.lo)*0.5;
          double longest_edge = e.hi[0] - e.lo[0];
          longest_edge = std::max(longest_edge, e.hi[1] - e.lo[1]);
          longest_edge = std::max(longest_edge, e.hi[2] - e.lo[2]);
          longest_edge *= 0.5;
          Parfait::Point<double> offset = {longest_edge, longest_edge, longest_edge};
          setRootExtent({center - offset, center + offset});
          auto should_split = [this](const std::vector<Parfait::Facet*>& objects){
              return objects.size() > max_objects_per_voxel;
          };
          setShouldSplit(should_split);
      }

      inline void printVoxelStatistics(){
          int total_count = 0;
          int leaf_count = 0;
          size_t max_facets = 0;
          int max_achieved_depth = 0;
          long total_facets = 0;
          int count_too_big = 0;
          for(auto& v : voxels){
              total_count++;
              if(v.isLeaf()) {
                  max_facets = std::max(max_facets, v.inside_objects.size());
                  total_facets += v.inside_objects.size();
                  leaf_count++;
                  max_achieved_depth = std::max(v.depth, max_achieved_depth);
                  if(v.inside_objects.size() > max_objects_per_voxel)
                      count_too_big++;
              }
          }
          printf("There are %d voxels, and %d are leaves\n", total_count, leaf_count);
          printf("Heaviest voxel has %lu facets.\n", max_facets);
          printf("Num facets over max = %d\n", count_too_big);
          printf("The average is %lf facets.\n", total_facets / float(leaf_count));
          printf("The deepest voxel is at depth %d\n", max_achieved_depth);
      }


      inline Parfait::Point<double> closestPoint(const Parfait::Point<double>& p) const {
          PriorityQueue process;
          process.push(std::make_pair(std::numeric_limits<double>::max(), 0));

          CurrentState current_state(p);

          while(not process.empty()) {
              current_state = closestPoint(p, current_state, process);
              if(current_state.actualDistance() < process.top().first) {
                  break;
              }
          }
          return current_state.surfaceLocation();
      }

  protected:

      inline CurrentState closestPoint(const Parfait::Point<double> &query_point,
                                       const CurrentState& current_state,
                                       PriorityQueue &process) const {

          long voxel_index = process.top().second;
          process.pop();
          if(voxels.empty()) return current_state;

          auto closest_possible_to_voxel = getClosestPointInExtent(voxels.at(voxel_index).extent, query_point);
          if(not current_state.isPotentiallyCloser(closest_possible_to_voxel)){
              return current_state;
          }

          if(voxels.at(voxel_index).isLeaf()) {
              return getClosestPointInLeaf(voxel_index, query_point, current_state);
          }

          for (const auto &child : voxels.at(voxel_index).children) {
              if (child != Node::EMPTY) {
                  if(voxels.at(child).filled) {
                      const auto &child_extent = voxels.at(child).extent;
                      auto p = getClosestPointInExtent(child_extent, query_point);
                      double dist = (p - query_point).magnitude();
                      process.push(std::make_pair(dist, child));
                  }
              }
          }
          return current_state;
      }

      inline CurrentState getClosestPointInLeaf(int voxel_index,
                                         const Point<double> &query_point,
                                         CurrentState current_state ) const {
          for (const auto f : voxels.at(voxel_index).inside_objects) {
              auto p = objects[f].GetClosestPoint(query_point);
              current_state.changeLocationIfCloser(p);
          }
          return current_state;
      }

   private:
     size_t max_objects_per_voxel = 20;
  };
}
