#include "cart_block.h"

CartBlock::CartBlock()
	: Extent(),
	  kx(0),
	  ky(0),
	  kz(0)
{
}

CartBlock::CartBlock(const double min_xyz[3],const double max_xyz[3])
	: Extent(min_xyz, max_xyz)
{
}

CartBlock::CartBlock(const double extent[6])
	: Extent(extent)
{
}

CartBlock::CartBlock(const double min_xyz[3], const double max_xyz[3],
		int ncells_x,int ncells_y,int ncells_z)
	: Extent(min_xyz, max_xyz),
	  kx(ncells_x),
   	  ky(ncells_y),
	  kz(ncells_z)
{
      number_of_cells = kx*ky*kz;
}

CartBlock::CartBlock(const double extent[3],
		int ncells_x,int ncells_y,int ncells_z)
	: Extent(extent),
	  kx(ncells_x),
	  ky(ncells_y),
	  kz(ncells_z)
{
      number_of_cells = kx*ky*kz;
}

void CartBlock::setDimensions(int ncells_x,int ncells_y,int ncells_z)
{
	kx = ncells_x;
	ky = ncells_y;
	kz = ncells_z;
    number_of_cells = kx*ky*kz;
}



double CartBlock::get_dx() const {return getLength_X() / (double) kx;}

double CartBlock::get_dy() const {return getLength_Y() / (double) ky;}

double CartBlock::get_dz() const {return getLength_Z() / (double) kz;}

int CartBlock::numberOfCells_X() const {return kx;}
int CartBlock::numberOfCells_Y() const {return ky;}
int CartBlock::numberOfCells_Z() const {return kz;}
int CartBlock::numberOfNodes_X() const {return kx+1;}
int CartBlock::numberOfNodes_Y() const {return ky+1;}
int CartBlock::numberOfNodes_Z() const {return kz+1;}

int CartBlock::numberOfCells() const {return number_of_cells;}

int CartBlock::numberOfNodes() const {return (kx+1)*(ky+1)*(kz+1);}

int CartBlock::getIdOfContainingCell(double point[3]) const
{
	assert(contains(Point(point)));
	double hx,hy,hz;
	//get_lo_point(lo);
	hx = point[0] - lo[0]; // get distance from min point
	hy = point[1] - lo[1];
	hz = point[2] - lo[2];
	int i,j,k;
	i = (int)(hx/get_dx());     // calc discrete coord
	j = (int)(hy/get_dy());
	k = (int)(hz/get_dz());
	return convert_ijk_ToCellId(i,j,k);
}

std::vector<int> CartBlock::getCellIdsInExtent(const Extent &b) const
{
    std::vector<int> cell_ids;
	// get min and max points of box
	Point search_lo,search_hi;
    search_lo = b.lo;
    search_hi = b.hi;
	//b.get_lo_point(search_lo);
	//b.get_hi_point(search_hi);
	// get distances to min point of mesh
	double hx,hy,hz;
    Point mesh_lo,mesh_hi;
	//get_lo_point(mesh_lo);
	//get_hi_point(mesh_hi);
    mesh_lo = lo;
    mesh_hi = hi;
	// return nothing if the box does not overlap
	cell_ids.clear();
	if(!contains(b))
		return cell_ids;
	// allow the box to be bigger than the mesh in any dimension
	for(int i=0;i<3;i++)
	{
		search_lo[i] = search_lo[i] < mesh_lo[i]? mesh_lo[i] :search_lo[i];
		search_hi[i] = search_hi[i] > mesh_hi[i]? mesh_hi[i] :search_hi[i];
	}
	hx = search_lo[0] - mesh_lo[0];
	hy = search_lo[1] - mesh_lo[1];
	hz = search_lo[2] - mesh_lo[2];
	int ilow,jlow,klow,ihigh,jhigh,khigh;
	ilow = (int)(hx/get_dx());
	jlow = (int)(hy/get_dy());
	klow = (int)(hz/get_dz());
	// adjust to be able to return lower boundary cells
	ilow = ilow==kx? ilow-1: ilow;
	jlow = jlow==ky? jlow-1: jlow;
	klow = klow==kz? klow-1: klow;
	hx = mesh_hi[0] - search_hi[0];
	hy = mesh_hi[1] - search_hi[1];
	hz = mesh_hi[2] - search_hi[2];
	ihigh = kx - (int)(hx/get_dx());
	jhigh = ky - (int)(hy/get_dy());
	khigh = kz - (int)(hz/get_dz());
	// adjust to be able to return upper boundary cells
	ihigh = ihigh==0? 1:ihigh;
	jhigh = jhigh==0? 1:jhigh;
	khigh = khigh==0? 1:khigh;
	// create a list of ids between low corner and high corner
	int icells,jcells,kcells;
	icells = ihigh - ilow;
	jcells = jhigh - jlow;
	kcells = khigh - klow;

	int ncells = icells*jcells*kcells;
	cell_ids.reserve(ncells);
	for(int i=ilow;i<ihigh;i++)
	{
		for(int j=jlow;j<jhigh;j++)
		{
			for(int k=klow;k<khigh;k++)
			{
				int id = convert_ijk_ToCellId(i,j,k);
				cell_ids.push_back(id);
			}
		}
	}
    return cell_ids;
}


void CartBlock::getNode(int node_id,double point[3]) const
{
	int i,j,k;
	convertNodeIdTo_ijk(node_id,i,j,k);
	point[0] = lo[0] + (get_dx())*((double)(i));
	point[1] = lo[1] + (get_dy())*((double)(j));
	point[2] = lo[2] + (get_dz())*((double)(k));
}

void CartBlock::getCellCentroid(int cell_id,double point[3]) const
{
	int i,j,k;
	convertCellIdTo_ijk(cell_id,i,j,k);
	point[0] = lo[0] + (get_dx())*((double)(i))+0.5*get_dx();
	point[1] = lo[1] + (get_dy())*((double)(j))+0.5*get_dy();
	point[2] = lo[2] + (get_dz())*((double)(k))+0.5*get_dz();
}

Extent CartBlock::createExtentFromCell(int cell_id) const
{
	int i,j,k;
	convertCellIdTo_ijk(cell_id,i,j,k);
	double centroid[3];
	getCellCentroid(cell_id,centroid);
	double dx = 0.5*get_dx();
	double dy = 0.5*get_dy();
	double dz = 0.5*get_dz();
	double lo[3],hi[3];
	lo[0] = centroid[0] - dx;
	lo[1] = centroid[1] - dy;
	lo[2] = centroid[2] - dz;
	hi[0] = centroid[0] + dx;
	hi[1] = centroid[1] + dy;
	hi[2] = centroid[2] + dz;
	Extent b(lo,hi);
	return b;
}

int CartBlock::convert_ijk_ToCellId(int i,int j,int k) const
{
	//assert(i >= 0);
	//assert(j >= 0);
	//assert(k >= 0);
	//assert(i < kx);
	//assert(j < ky);
	//assert(k < kz);
	int id = i + j*kx + k*kx*ky;
	//assert(id < numberOfCells());
	return id;
}

void CartBlock::convertCellIdTo_ijk(int cell_id,int &i,int &j,int &k) const
{
	k = cell_id / (kx*ky);
	j = (cell_id-k*kx*ky) / kx;
	i = cell_id - k*kx*ky - j*kx;
	//assert(i >= 0);
	//assert(j >= 0);
	//assert(k >= 0);
	//assert(i < kx);
	//assert(j < ky);
	//assert(k < kz);
}

int CartBlock::convert_ijk_ToNodeId(int i,int j,int k) const
{
	int nx = kx+1;
	int ny = ky+1;
	return i + j*nx + k*nx*ny;
}

void CartBlock::convertNodeIdTo_ijk(int node_id,int &i,int &j,int &k) const
{
	int nx = kx+1;
	int ny = ky+1;
	k = node_id / (nx*ny);
	j = (node_id-k*nx*ny) / nx;
	i = node_id - k*nx*ny - j*nx;
}
