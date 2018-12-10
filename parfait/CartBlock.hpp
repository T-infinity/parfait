
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
inline Parfait::CartBlock::CartBlock(const Point<double>& min_xyz, const Point<double>& max_xyz,
                     int ncells_x,int ncells_y,int ncells_z)
    : Extent(min_xyz, max_xyz),
      kx(ncells_x),
      ky(ncells_y),
      kz(ncells_z)
{
  number_of_cells = kx*ky*kz;
}

inline Parfait::CartBlock::CartBlock(const Extent<double>& extent,
                     int ncells_x,int ncells_y,int ncells_z)
    : Extent(extent.lo,extent.hi),
      kx(ncells_x),
      ky(ncells_y),
      kz(ncells_z)
{
  number_of_cells = kx*ky*kz;
}

inline double Parfait::CartBlock::get_dx() const {return getLength_X() / (double) kx;}

inline double Parfait::CartBlock::get_dy() const {return getLength_Y() / (double) ky;}

inline double Parfait::CartBlock::get_dz() const {return getLength_Z() / (double) kz;}

inline int Parfait::CartBlock::numberOfCells_X() const {return kx;}
inline int Parfait::CartBlock::numberOfCells_Y() const {return ky;}
inline int Parfait::CartBlock::numberOfCells_Z() const {return kz;}
inline int Parfait::CartBlock::numberOfNodes_X() const {return kx+1;}
inline int Parfait::CartBlock::numberOfNodes_Y() const {return ky+1;}
inline int Parfait::CartBlock::numberOfNodes_Z() const {return kz+1;}

inline int Parfait::CartBlock::numberOfCells() const {return number_of_cells;}

inline int Parfait::CartBlock::numberOfNodes() const {return (kx+1)*(ky+1)*(kz+1);}

inline std::vector<int> Parfait::CartBlock::getNodesInCell(int id) const {
    int i, j, k;
    convertCellIdTo_ijk(id, i, j, k);

    std::vector<int> cellNodes(8);
    cellNodes[0] = convert_ijk_ToNodeId(i  , j  , k  );
    cellNodes[1] = convert_ijk_ToNodeId(i+1, j  , k  );
    cellNodes[2] = convert_ijk_ToNodeId(i+1, j+1, k  );
    cellNodes[3] = convert_ijk_ToNodeId(i  , j+1, k  );
    cellNodes[4] = convert_ijk_ToNodeId(i  , j  , k+1);
    cellNodes[5] = convert_ijk_ToNodeId(i+1, j  , k+1);
    cellNodes[6] = convert_ijk_ToNodeId(i+1, j+1, k+1);
    cellNodes[7] = convert_ijk_ToNodeId(i  , j+1, k+1);
    return cellNodes;
}

inline int Parfait::CartBlock::getIdOfContainingCell(const double point[3]) const
{
    if(not contains(Point<double>(point)))
        throw std::domain_error("Point not contained by any cell in block");
  double hx,hy,hz;
  hx = point[0] - lo[0]; // get distance from min point
  hy = point[1] - lo[1];
  hz = point[2] - lo[2];
  int i,j,k;
  i = std::min(int(hx/get_dx()),numberOfCells_X()-1);     // calc discrete coord
  j = std::min(int(hy/get_dy()),numberOfCells_Y()-1);
  k = std::min(int(hz/get_dz()),numberOfCells_Z()-1);
  return convert_ijk_ToCellId(i,j,k);
}

inline std::vector<int> Parfait::CartBlock::getCellIdsInExtent(const Extent<double> &b) const {
    std::vector<int> cell_ids;
    auto slice = getRangeOfOverlappingCells(b);
    for(int i=slice.lo[0];i<slice.hi[0];i++)
        for(int j=slice.lo[1];j<slice.hi[1];j++)
            for(int k=slice.lo[2];k<slice.hi[2];k++)
                cell_ids.push_back(convert_ijk_ToCellId(i,j,k));
    return cell_ids;
}

inline Parfait::Extent<int> Parfait::CartBlock::getRangeOfOverlappingCells(const Extent<double> &search_extent) const {
    if(!contains(search_extent))
        return Extent<int>({0,0,0},{0,0,0});

    Extent<double> fitted_search_extent = search_extent;
    for(int i=0;i<3;i++) {
        fitted_search_extent.lo[i] = std::max(fitted_search_extent.lo[i],lo[i]);
        fitted_search_extent.hi[i] = std::min(fitted_search_extent.hi[i],hi[i]);
    }
    // I don't remember exactly how this works, but it passes the bindings_tests....
    double hx,hy,hz;
    hx = fitted_search_extent.lo[0] - lo[0];
    hy = fitted_search_extent.lo[1] - lo[1];
    hz = fitted_search_extent.lo[2] - lo[2];
    int ilow,jlow,klow,ihigh,jhigh,khigh;
    ilow = (int)(hx/get_dx());
    jlow = (int)(hy/get_dy());
    klow = (int)(hz/get_dz());
    ilow = ilow==kx? ilow-1: ilow;
    jlow = jlow==ky? jlow-1: jlow;
    klow = klow==kz? klow-1: klow;
    hx = hi[0] - fitted_search_extent.hi[0];
    hy = hi[1] - fitted_search_extent.hi[1];
    hz = hi[2] - fitted_search_extent.hi[2];
    ihigh = kx - (int)(hx/get_dx());
    jhigh = ky - (int)(hy/get_dy());
    khigh = kz - (int)(hz/get_dz());
    ihigh = std::max(1,ihigh);
    jhigh = std::max(1,jhigh);
    khigh = std::max(1,khigh);
    return Extent<int>({ilow,jlow,klow},{ihigh,jhigh,khigh});
}

inline void Parfait::CartBlock::getNode(int node_id,double point[3]) const
{
  int i,j,k;
  convertNodeIdTo_ijk(node_id,i,j,k);
  point[0] = lo[0] + (get_dx())*((double)(i));
  point[1] = lo[1] + (get_dy())*((double)(j));
  point[2] = lo[2] + (get_dz())*((double)(k));
}

inline Parfait::Point<double> Parfait::CartBlock::getPoint(int node_id) const {
    Point<double> p;
    getNode(node_id,p.data());
    return p;
}

inline void Parfait::CartBlock::getCellCentroid(int cell_id,double point[3]) const
{
  int i,j,k;
  convertCellIdTo_ijk(cell_id,i,j,k);
  point[0] = lo[0] + (get_dx())*((double)(i))+0.5*get_dx();
  point[1] = lo[1] + (get_dy())*((double)(j))+0.5*get_dy();
  point[2] = lo[2] + (get_dz())*((double)(k))+0.5*get_dz();
}

inline Parfait::Extent<double> Parfait::CartBlock::createExtentFromCell(int cell_id) const
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

inline int Parfait::CartBlock::convert_ijk_ToCellId(int i,int j,int k) const
{
  int id = i + j*kx + k*kx*ky;
  return id;
}

inline void Parfait::CartBlock::convertCellIdTo_ijk(int cell_id,int &i,int &j,int &k) const
{
  k = cell_id / (kx*ky);
  j = (cell_id-k*kx*ky) / kx;
  i = cell_id - k*kx*ky - j*kx;
}

inline int Parfait::CartBlock::convert_ijk_ToNodeId(int i,int j,int k) const
{
  int nx = kx+1;
  int ny = ky+1;
  return i + j*nx + k*nx*ny;
}

inline void Parfait::CartBlock::convertNodeIdTo_ijk(int node_id,int &i,int &j,int &k) const
{
  int nx = kx+1;
  int ny = ky+1;
  k = node_id / (nx*ny);
  j = (node_id-k*nx*ny) / nx;
  i = node_id - k*nx*ny - j*nx;
}
