#ifndef ADT_BUILDER_H
#define ADT_BUILDER_H
#include "range_loop.h"
#include "adt_3d_extent.h"
#include "generic_mesh.h"
#include "extent_builder.h"

namespace AdtBuilder
{
	template<typename MeshType>
	Adt3DExtent putCellsInAdt(MeshType &mesh_in)
	{
		Mesh<MeshType> mesh(mesh_in);
		Extent domain = ExtentBuilder::build(mesh);
		Adt3DExtent adt(domain);
		for(auto cell : mesh.cells())
			adt.store(cell.Id(),ExtentBuilder::build(cell));
		return adt;
	}

};


#endif
