#pragma once
#include "Adt3dExtent.h"
#include "GenericMesh.h"
#include "ExtentBuilder.h"

namespace Parfait {
    namespace AdtBuilder {
        template<typename MeshType>
        Adt3DExtent putCellsInAdt(MeshType &mesh_in) {
            Mesh<MeshType> mesh(mesh_in);
            auto domain = ExtentBuilder::build(mesh);
            Adt3DExtent adt(domain);
            for (auto cell : mesh.cells())
                adt.store(cell.Id(), ExtentBuilder::build(cell));
            return std::move(adt);
        }
    };
}