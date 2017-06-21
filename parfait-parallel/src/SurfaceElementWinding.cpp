#include "SurfaceElementWinding.h"

namespace {

  Parfait::Point<double> computeTriangleArea(
          const Parfait::Point<double>& a,
          const Parfait::Point<double>& b,
          const Parfait::Point<double>& c) {
      auto v1 = b - a;
      auto v2 = c - a;
      return 0.5 * Point<double>::cross(v1, v2);
  }

  Parfait::Point<double> average(const std::vector<Parfait::Point<double>>& points) {
      Parfait::Point<double> avg = {0, 0, 0};
      for (auto& p : points)
          avg += p;
      return avg / (double) points.size();
  }

  Parfait::Point<double> computeCellCenter(const std::shared_ptr<Parfait::ParallelMesh>& mesh, int cell_id) {
      auto cell = mesh->getCell(cell_id);
      auto cell_length = mesh->numberOfNodesInCell(cell_id);
      Parfait::Point<double> center = {0, 0, 0};
      for (int i = 0; i < cell_length; i++) {
          center += mesh->getXyz(cell[i]);
      }
      return center / (double) cell_length;
  }

  bool isSurfacePointingOut(const std::shared_ptr<ParallelMesh>& mesh,
                            const int* surface,
                            int volume_neighbor) {
      auto a = mesh->getXyz(surface[0]);
      auto b = mesh->getXyz(surface[1]);
      auto c = mesh->getXyz(surface[2]);
      auto surface_area = computeTriangleArea(a, b, c);
      auto surface_center = average({a, b, c});
      auto neighbor_center = computeCellCenter(mesh, volume_neighbor);
      auto out_direction = surface_center - neighbor_center;
      out_direction.normalize();
      surface_area.normalize();
      return (Parfait::Point<double>::dot(out_direction, surface_area) > 0);
  }

  template<typename T>
  void swap(T& a, T& b) {
      auto t = a;
      a = b;
      b = t;
  }

  void rewindTriangle(int* triangle) {
      swap(triangle[1], triangle[2]);
  }

  void rewindQuad(int* quad) {
      int d = quad[0];
      int c = quad[1];
      int b = quad[2];
      int a = quad[3];
      quad[0] = a;
      quad[1] = b;
      quad[2] = c;
      quad[3] = d;
  }
}

bool areSurfaceElementsWoundOut(const std::shared_ptr<Parfait::ParallelMesh>& mesh,
                                const std::vector<int>& surface_neighbors) {
    for (int t = 0; t < mesh->numberOfTriangles(); t++) {
        auto surface = mesh->getTriangle(t);
        int neighbor = surface_neighbors[t];
        if (not isSurfacePointingOut(mesh, surface, neighbor))
            return false;
    }

    for (int q = 0; q < mesh->numberOfQuads(); q++) {
        auto surface = mesh->getQuad(q);
        auto neighbor = surface_neighbors[q + mesh->numberOfTriangles()];
        if (not isSurfacePointingOut(mesh, surface, neighbor))
            return false;
    }

    return true;
}

void windAllSurfaceElementsOut(std::shared_ptr<MessagePasser> mp, std::shared_ptr<Parfait::ParallelMesh>& mesh,
                               const std::vector<int>& surface_neighbors) {

    auto builder = Parfait::ParallelMeshBuilder(mp, *mesh.get());

    for (int t = 0; t < mesh->numberOfTriangles(); t++) {
        auto surface = mesh->getTriangle(t);
        int neighbor = surface_neighbors[t];
        if (not isSurfacePointingOut(mesh, surface, neighbor))
            rewindTriangle(&builder.data->triangles[3 * t]);
    }

    for (int q = 0; q < mesh->numberOfQuads(); q++) {
        auto surface = mesh->getQuad(q);
        auto neighbor = surface_neighbors[q + mesh->numberOfTriangles()];
        if (not isSurfacePointingOut(mesh, surface, neighbor))
            rewindQuad(&builder.data->quads[4 * q]);
    }
    mesh = builder.exportMesh();
}
