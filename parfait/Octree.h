#pragma once
#include "Extent.h"
#include "ExtentBuilder.h"
#include "Throw.h"
#include <array>
#include <vector>
#include <functional>
namespace Parfait {

  template <typename T>
  class Octree {
   public:
      class Node {
       public:
          enum {EMPTY = -11};
          inline Node(const Parfait::Extent<double>& e, int depth)
              : extent(e), depth(depth), filled(false) {
          }
          Parfait::Extent<double> extent;
          const int depth;
          bool filled;
          std::array<int, 8> children {
              EMPTY, EMPTY, EMPTY, EMPTY,
              EMPTY, EMPTY, EMPTY, EMPTY
          };

          std::vector<int> inside_objects;

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
   public:
      void setMaxDepth(int depth){
          max_depth = depth;
      }
      void setRootExtent(const Parfait::Extent<double>& e){
          root_extent = e;
          root_extent_set = true;
      }
      void finalize() {
          contractExtents();
      }
      void insert(const T& f){
          if(!should_split) THROW("Cannot insert before calling setShouldSplit");
          if(not root_extent_set) THROW("Cannot insert before setting root extent");
          if(locked) THROW("Cannot insert after calling contractExtents()");
          if(voxels.size() == 0)
              initializeRoot();
          int object_id = objects.size();
          objects.push_back(f);
          insert(0, object_id);
      }
      void setShouldSplit(std::function<bool(const std::vector<T*>&)> s){
          should_split = s;
      }
      std::vector<std::pair<Parfait::Extent<double>,int>> exportLeafExtentsAndCounts() const {
          std::vector<std::pair<Parfait::Extent<double>, int>> output;
          for(auto& v : voxels){
            if(v.isLeaf())
                output.push_back({v.extent, v.inside_objects.size()});
          }
          return output;
      }
      std::vector<Parfait::Extent<double>> getLeafExtents() const {
          std::vector<Parfait::Extent<double>> extents;
          for(auto& v : voxels){
              if(v.isLeaf()) {
                  extents.push_back(v.extent);
              }
          }
          return extents;
      }
   public:
      int max_depth = 5;
      bool locked = false;
      bool root_extent_set = false;
      Parfait::Extent<double> root_extent;
      std::function<bool(std::vector<T*>&)> should_split = nullptr;
      std::vector<Node> voxels;
      std::vector<T> objects;

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
          auto children = voxels.at(voxel_index).children;
          for(int child : children){
              auto inside = voxels.at(voxel_index).inside_objects;
              for (auto &object_index : inside) {
                  insert(child, object_index);
              }
          }
          voxels.at(voxel_index).inside_objects.clear();
          voxels.at(voxel_index).inside_objects.shrink_to_fit();
      }

      inline void insert(int voxel_index, int object_index) {
          const auto extent = voxels.at(voxel_index).extent;
          if(objects.at(object_index).intersects(extent)) {
              voxels.at(voxel_index).filled = true;
              if(voxels.at(voxel_index).isLeaf()) {
                  voxels.at(voxel_index).inside_objects.push_back(object_index);
                  if(not atMaxDepth(voxel_index)) {
                      std::vector<T*> inside;
                      inside.reserve(voxels.at(voxel_index).inside_objects.size());
                      for(auto i : voxels.at(voxel_index).inside_objects){
                          inside.push_back(&objects[i]);
                      }
                      if(should_split(inside)) {
                          splitVoxel(voxel_index);
                      }
                  }
              }
              else {
                  auto children = voxels.at(voxel_index).children;
                  for (auto child : children) {
                      if(child != Node::EMPTY)
                          insert(child, object_index);
                  }
              }
          }
      }
      bool atMaxDepth(int voxel_index) const { return voxels.at(voxel_index).depth == max_depth; }
      inline void contractExtents(){
          locked = true;
          if(voxels.size() == 0) return;
          contractExtents(0);
      }
      inline Parfait::Extent<double> contractExtents(int voxel_index){

          auto extent = determineShrunkExtent(voxel_index);
          voxels.at(voxel_index).extent = extent;
          return extent;
      }

      inline Extent<double> determineShrunkExtent(int voxel_index) {
          if(voxels.at(voxel_index).filled) {
              if (voxels.at(voxel_index).isLeaf())
                  return determineShrunkExtentLeaf(voxel_index);
              else
                  return determineShrunkExtentChildren(voxel_index);
          }
          return voxels.at(voxel_index).extent;
      }

      inline Extent<double> determineShrunkExtentChildren(int voxel_index) {
          auto extent = ExtentBuilder::createEmptyBuildableExtent<double>();
          for (auto &child : voxels.at(voxel_index).children) {
              if (child != Node::EMPTY) {
                  if (voxels.at(voxel_index).filled)
                      ExtentBuilder::expandExtentWithAnother(extent, contractExtents(child));
              }
          }
          return extent;
      }

      inline Extent<double> determineShrunkExtentLeaf(int voxel_index) const {
          auto extent = ExtentBuilder::createEmptyBuildableExtent<double>();
          for (long object_index : voxels.at(voxel_index).inside_objects) {
              const auto &object = objects[object_index];
              auto e = object.getExtent();
              auto overlap = ExtentBuilder::intersection(e, voxels.at(voxel_index).extent);
              ExtentBuilder::expandExtentWithAnother(extent, overlap);
          }
          return extent;
      }
  };
}

