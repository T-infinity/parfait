#ifndef PARFAIT_VTKEXPLODEDVIEWWRITER_HPP
#define PARFAIT_VTKEXPLODEDVIEWWRITER_HPP

namespace Parfait {
  template<class Mesh>
  VtkExplodedViewWriter<Mesh>::VtkExplodedViewWriter(const Mesh &mesh_in, std::string filename_in)
          : mesh(mesh_in), filename(filename_in) { }

  template <class Mesh>
  void VtkExplodedViewWriter<Mesh>::VtkExplodedViewWriter::writeAscii() {

  }
}



#endif
