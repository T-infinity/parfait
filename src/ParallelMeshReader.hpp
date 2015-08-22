#include <stdio.h>
#include <vector>
#include <RangeLoop.h>
#include <ImportedUgrid.h>
#include <UgridReader.h>

ImportedUgrid ParallelMeshReader::readDistributedGrid(std::string configurationFileName) {
  ConfigurationReader configurationReader(configurationFileName);
  ParallelMeshReader reader(configurationReader.getGridFilenames(), configurationReader.getGridEndianness());
  return reader.distributeGridsEvenly();
}

ImportedUgrid ParallelMeshReader::readDistributedGrid(std::vector<std::string> gridFiles, std::vector<bool> isBigEndian){
  ParallelMeshReader reader(gridFiles, isBigEndian);
  return reader.distributeGridsEvenly();
}


inline Parfait::ParallelMeshReader::ParallelMeshReader(std::vector<std::string> gridFiles_in,
                                                        std::vector<bool> isBigEndian_in)
    : isBigEndian(isBigEndian_in) {

  gridFiles = gridFiles_in;
  int ngrid = (int) gridFiles.size();
  gridNodeMap.reserve(ngrid);
  gridTriangleMap.reserve(ngrid);
  gridQuadMap.reserve(ngrid);
  gridTetMap.reserve(ngrid);
  gridPyramidMap.reserve(ngrid);
  gridPrismMap.reserve(ngrid);
  gridHexMap.reserve(ngrid);
  gridNodeMap.push_back(0);
  gridTriangleMap.push_back(0);
  gridQuadMap.push_back(0);
  gridTetMap.push_back(0);
  gridPyramidMap.push_back(0);
  gridPrismMap.push_back(0);
  gridHexMap.push_back(0);
  for (int i:range(gridFiles)) {
    int nnodes, ntri, nquad, ntet, npyr, nprism, nhex;
    UgridReader::readHeader(gridFiles[i], nnodes, ntri, nquad, ntet, npyr, nprism, nhex, isBigEndian[i]);
    gridNodeMap.push_back(nnodes);
    gridTriangleMap.push_back(ntri);
    gridQuadMap.push_back(nquad);
    gridTetMap.push_back(ntet);
    gridPyramidMap.push_back(npyr);
    gridPrismMap.push_back(nprism);
    gridHexMap.push_back(nhex);
  }

  for (int i = 2; i < (int) gridNodeMap.size(); i++) {
    gridNodeMap[i] += gridNodeMap[i - 1];
    gridTriangleMap[i] += gridTriangleMap[i - 1];
    gridQuadMap[i] += gridQuadMap[i - 1];
    gridTetMap[i] += gridTetMap[i - 1];
    gridPyramidMap[i] += gridPyramidMap[i - 1];
    gridPrismMap[i] += gridPrismMap[i - 1];
    gridHexMap[i] += gridHexMap[i - 1];
  }
  if (MessagePasser::Rank() == 0) {
    printf("Composite rootDomain has:\n");
    printf("--nnodes        %i\n", gridNodeMap.back());
    printf("--triangles     %i\n", gridTriangleMap.back());
    printf("--quads         %i\n", gridQuadMap.back());
    printf("--tets          %i\n", gridTetMap.back());
    printf("--pyramids      %i\n", gridPyramidMap.back());
    printf("--prisms        %i\n", gridPrismMap.back());
    printf("--hexs          %i\n", gridHexMap.back());
  }
  distributeUgrid();
}

inline std::vector<int> Parfait::ParallelMeshReader::getGridNodeMap() {
  return gridNodeMap;
}

inline std::vector<int> Parfait::ParallelMeshReader::getProcNodeMap() {
  return procNodeMap;
}

inline void Parfait::ParallelMeshReader::distributeUgrid() {
    if (0 == MessagePasser::Rank())
        buildDistributionMaps();
    MessagePasser::Broadcast(procNodeMap, 0);

    if (MessagePasser::Rank() == 0)
        printf("Distributing ...\n--nodes\n");
    distributeNodes();
    distributeTriangles();

    if (MessagePasser::Rank() == 0)
        printf("Done Distributing ...\n");
}

inline void Parfait::ParallelMeshReader::buildDistributionMaps() {
  int totalNodes = 0;
  for (int i:range(gridFiles)) {
    int nnodes, ntri, nquad, ntet, npyr, nprism, nhex;
    UgridReader::readHeader(gridFiles[i], nnodes, ntri, nquad,
                            ntet, npyr, nprism, nhex, isBigEndian[i]);
    totalNodes += nnodes;
  }
  int nproc = MessagePasser::NumberOfProcesses();
  procNodeMap.assign(nproc, 0);

  for (int i = 0; i < nproc; i++) {
    procNodeMap[i] = totalNodes / nproc;
  }
  for (int i = 0; i < totalNodes % nproc; i++)
    procNodeMap[i]++;
  procNodeMap.insert(procNodeMap.begin(), 0);
  for (int i = 2; i < nproc + 1; i++) {
    procNodeMap[i] += procNodeMap[i - 1];
  }
}

inline Parfait::ImportedUgrid Parfait::ParallelMeshReader::distributeGridsEvenly() {
  ImportedUgrid ugrid(myNodes, myTriangles, myQuads, myTets,
                      myPyramids, myPrisms, myHexs, myTriangleTags, myQuadTags,
                      myTriangleTags, myQuadTags);
  return ugrid;
}


inline void Parfait::ParallelMeshReader::distributeNodes()
{
  if(MessagePasser::Rank() == 0)
  {
    int nproc = MessagePasser::NumberOfProcesses();
    myNodes = getNodes(0,procNodeMap[1]);
    for(int proc=1;proc<nproc;proc++)
    {
      vector<double> nodeBuffer = getNodes(procNodeMap[proc],procNodeMap[proc+1]);
      MessagePasser::Send(nodeBuffer,proc);
    }
  }
  else
    MessagePasser::Recv(myNodes,0);
}


void ParallelMeshReader::distributeTriangles() {
    if(MessagePasser::Rank() == 0) {
        int end = gridTriangleMap.back();
        for (int grid = 0; grid < gridTriangleMap.size() - 1; grid++) {
            for (int i = gridTriangleMap[grid]; i < gridTriangleMap[grid + 1]; i++) {
                auto t = getTriangles(i, i + 1);
                for (int &id:t)
                    id += convertComponentNodeIdToGlobal(id, grid);
                std::set<int> target_procs;
                for (int id:t)
                    target_procs.insert(getOwningProcOfNode(id));
                for (int target:target_procs) {
                    fprintf(stderr,"target = %i\n",target);
                    if(0 == target)
                        saveTriangle(t);
                    else
                        MessagePasser::Send(t, target);
                }
            }
        }
        std::vector<int> done_triangle_signal = {1};
        for(int i=1;i<MessagePasser::NumberOfProcesses();i++)
            MessagePasser::Send(done_triangle_signal,i);
    }
    else {
        bool done = false;
        while (not done) {
            std::vector<int> triangle;
            MessagePasser::Recv(triangle, 0);
            if (1 == triangle.size())
                done = true;
        }
    }
}

void ParallelMeshReader::saveTriangle(const std::vector<int>& triangle){
    for(auto id:triangle)
        myTriangles.push_back(id);
}


int ParallelMeshReader::getOwningProcOfNode(int id) {
    int nnodes = gridNodeMap.back();
    int nproc = MessagePasser::NumberOfProcesses();
    int nper_proc = nnodes/nproc;
    int nprocs_with_extra = (nnodes%nproc);
    int node_break_point = (nper_proc+1)*nprocs_with_extra;
    if(id < node_break_point){
        nper_proc++;
        return id / nper_proc;
    }
    else{
        id -= node_break_point;
        return id / nper_proc + nprocs_with_extra;
    }


    return 0;
}

int ParallelMeshReader::convertComponentNodeIdToGlobal(int id,int grid) const {
    return id + gridNodeMap[grid];
}


inline std::vector<double> Parfait::ParallelMeshReader::getNodes(int begin,int end)
{
  using namespace UgridReader;
  std::vector<double> nodeBuffer(3*(end-begin),0.0);
  int firstGrid  = getFirstGrid(gridNodeMap,begin);
  int lastGrid   = getLastGrid(gridNodeMap,end);
  int beginIndex = getBeginIndex(gridNodeMap,begin);
  int endIndex   = getEndIndex(gridNodeMap,end);
  int positionInBuffer = 0;
  std::vector<double> tmp;
  if(firstGrid == lastGrid)
  {
    tmp = readNodes(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
  }
  else
  {
    tmp = readNodes(gridFiles[firstGrid],beginIndex,
                    gridNodeMap[firstGrid+1]-gridNodeMap[firstGrid],isBigEndian[firstGrid]);
  }
  for(double node : tmp)
    nodeBuffer[positionInBuffer++] = node;
  tmp.clear();

  // read all nodes from grids between first and last grid
  for(int i=firstGrid+1;i<lastGrid;i++)
  {
    //printf("--From grid %i: reading all nodes\n",i);
    tmp = readNodes(gridFiles[i],isBigEndian[i]);
    for(double node : tmp)
      nodeBuffer[positionInBuffer++] = node;
    tmp.clear();
  }
  // read nodes from last grid (start at zero and end at endIndex)
  if(lastGrid > firstGrid)
  {
    tmp.clear();
    tmp = readNodes(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
    for(double node : tmp)
      nodeBuffer[positionInBuffer++] = node;
  }

  return nodeBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getTriangles(int begin,int end)
{
  using namespace UgridReader;
  std::vector<int> triangleBuffer(3*(end-begin),0.0);
  int firstGrid  = getFirstGrid(gridTriangleMap,begin);
  int lastGrid   = getLastGrid(gridTriangleMap,end);
  int beginIndex = getBeginIndex(gridTriangleMap,begin);
  int endIndex   = getEndIndex(gridTriangleMap,end);
  int positionInBuffer = 0;
  std::vector<int> tmp;
  if(firstGrid == lastGrid)
  {
    // read triangles from the first grid (start at beginIndex and read to endIndex)
    tmp = readTriangles(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
  }
  else
  {
    // read triangles from the first grid (start at beginIndex and read to the end of the file)
    tmp = readTriangles(gridFiles[firstGrid],beginIndex,gridTriangleMap[firstGrid+1]
                                                        -gridTriangleMap[firstGrid],isBigEndian[firstGrid]);
  }
  for(int triangle : tmp)
    triangleBuffer[positionInBuffer++] = triangle + gridNodeMap[firstGrid];
  tmp.clear();

  // read all triangles from grids between first and last grid
  for(int i=firstGrid+1;i<lastGrid;i++)
  {
    tmp = readTriangles(gridFiles[i],isBigEndian[i]);
    for(int triangle : tmp)
      triangleBuffer[positionInBuffer++] = triangle + gridNodeMap[i];
    tmp.clear();
  }

  // read triangles from last grid (start at zero and end at endIndex)
  if(lastGrid > firstGrid)
  {
    tmp = readTriangles(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
    for(int triangle : tmp)
      triangleBuffer[positionInBuffer++] = triangle + gridNodeMap[lastGrid];
  }
  return triangleBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getTriangleTags(int begin,int end)
{
  using namespace UgridReader;
  std::vector<int> triangleTagBuffer(end-begin,0);
  int firstGrid  = getFirstGrid(gridTriangleMap,begin);
  int lastGrid   = getLastGrid(gridTriangleMap,end);
  int beginIndex = getBeginIndex(gridTriangleMap,begin);
  int endIndex   = getEndIndex(gridTriangleMap,end);
  int positionInBuffer = 0;
  vector<int> tmp;
  if(firstGrid == lastGrid)
  {
    // read triangles from the first grid (start at beginIndex and read to endIndex)
    tmp = readTriangleBoundaryTags(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
  }
  else
  {
    // read triangles from the first grid (start at beginIndex and read to the end of the file)
    tmp = readTriangleBoundaryTags(gridFiles[firstGrid],beginIndex,gridTriangleMap[firstGrid+1]
                                                                   -gridTriangleMap[firstGrid],isBigEndian[firstGrid]);
  }
  for(int tag : tmp)
    triangleTagBuffer[positionInBuffer++] = tag;
  tmp.clear();

  // read all triangles from grids between first and last grid
  for(int i=firstGrid+1;i<lastGrid;i++)
  {
    tmp = readTriangleBoundaryTags(gridFiles[i],isBigEndian[i]);
    for(int tag : tmp)
      triangleTagBuffer[positionInBuffer++] = tag;
    tmp.clear();
  }

  // read triangles from last grid (start at zero and end at endIndex)
  if(lastGrid > firstGrid)
  {
    tmp = readTriangleBoundaryTags(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
    for(int tag : tmp)
      triangleTagBuffer[positionInBuffer++] = tag;
  }
  return triangleTagBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getQuads(int begin,int end)
{
  using namespace UgridReader;
  std::vector<int> quadBuffer(4*(end-begin),0.0);
  int firstGrid  = getFirstGrid(gridQuadMap,begin);
  int lastGrid   = getLastGrid(gridQuadMap,end);
  int beginIndex = getBeginIndex(gridQuadMap,begin);
  int endIndex   = getEndIndex(gridQuadMap,end);
  int positionInBuffer = 0;
  std::vector<int> tmp;
  if(firstGrid == lastGrid)
  {
    // read quads from the first grid (start at beginIndex and read to endIndex)
    tmp = readQuads(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
  }
  else
  {
    // read quads from the first grid (start at beginIndex and read to the end of the file)
    tmp = readQuads(gridFiles[firstGrid],beginIndex,gridQuadMap[firstGrid+1]
                                                    -gridQuadMap[firstGrid],isBigEndian[firstGrid]);
  }
  for(int quad : tmp)
    quadBuffer[positionInBuffer++] = quad + gridNodeMap[firstGrid];
  tmp.clear();

  // read all quads from grids between first and last grid
  for(int i=firstGrid+1;i<lastGrid;i++)
  {
    tmp = readQuads(gridFiles[i],isBigEndian[i]);
    for(int quad : tmp)
      quadBuffer[positionInBuffer++] = quad + gridNodeMap[i];
    tmp.clear();
  }

  // read quads from last grid (start at zero and end at endIndex)
  if(lastGrid > firstGrid)
  {
    tmp = readQuads(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
    for(int quad : tmp)
      quadBuffer[positionInBuffer++] = quad + gridNodeMap[lastGrid];
  }
  return quadBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getQuadTags(int begin,int end)
{
  using namespace UgridReader;
  std::vector<int> quadTagBuffer(end-begin,0);
  int firstGrid  = getFirstGrid(gridQuadMap,begin);
  int lastGrid   = getLastGrid(gridQuadMap,end);
  int beginIndex = getBeginIndex(gridQuadMap,begin);
  int endIndex   = getEndIndex(gridQuadMap,end);
  int positionInBuffer = 0;
  vector<int> tmp;
  if(firstGrid == lastGrid)
  {
    // read quads from the first grid (start at beginIndex and read to endIndex)
    tmp = readQuadBoundaryTags(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
  }
  else
  {
    // read quads from the first grid (start at beginIndex and read to the end of the file)
    tmp = readQuadBoundaryTags(gridFiles[firstGrid],beginIndex,gridQuadMap[firstGrid+1]
                                                               -gridQuadMap[firstGrid],isBigEndian[firstGrid]);
  }
  for(int tag : tmp)
    quadTagBuffer[positionInBuffer++] = tag;
  tmp.clear();

  // read all quads from grids between first and last grid
  for(int i=firstGrid+1;i<lastGrid;i++)
  {
    tmp = readQuadBoundaryTags(gridFiles[i],isBigEndian[i]);
    for(int tag : tmp)
      quadTagBuffer[positionInBuffer++] = tag;
    tmp.clear();
  }

  // read quads from last grid (start at zero and end at endIndex)
  if(lastGrid > firstGrid)
  {
    tmp = readQuadBoundaryTags(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
    for(int tag : tmp)
      quadTagBuffer[positionInBuffer++] = tag;
  }
  return quadTagBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getTets(int begin,int end)
{
  using namespace UgridReader;
  std::vector<int> tetBuffer(4*(end-begin),0);
  int firstGrid  = getFirstGrid(gridTetMap,begin);
  int lastGrid   = getLastGrid(gridTetMap,end);
  int beginIndex = getBeginIndex(gridTetMap,begin);
  int endIndex   = getEndIndex(gridTetMap,end);
  int positionInBuffer = 0;
  std::vector<int> tmp;
  if(firstGrid == lastGrid)
  {
    // read tets from the first grid (start at beginIndex and read to endIndex)
    tmp = readTets(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
  }
  else
  {
    // read tets from the first grid (start at beginIndex and read to the end of the file)
    tmp = readTets(gridFiles[firstGrid],beginIndex,gridTetMap[firstGrid+1]
                                                   -gridTetMap[firstGrid],isBigEndian[firstGrid]);
  }
  for(int tet : tmp)
    tetBuffer[positionInBuffer++] = tet + gridNodeMap[firstGrid];
  tmp.clear();
  // read all tets from grids between first and last grid
  for(int i=firstGrid+1;i<lastGrid;i++)
  {
    tmp = readTets(gridFiles[i],isBigEndian[i]);
    for(int tet : tmp)
      tetBuffer[positionInBuffer++] = tet + gridNodeMap[i];
    tmp.clear();
  }

  // read tets from last grid (start at zero and end at endIndex)
  if(lastGrid > firstGrid)
  {
    tmp = readTets(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
    for(int tet : tmp)
      tetBuffer[positionInBuffer++] = tet + gridNodeMap[lastGrid];
  }
  return tetBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getPyramids(int begin,int end)
{
  using namespace UgridReader;
  std::vector<int> pyramidBuffer(5*(end-begin),0.0);
  int firstGrid  = getFirstGrid(gridPyramidMap,begin);
  int lastGrid   = getLastGrid(gridPyramidMap,end);
  int beginIndex = getBeginIndex(gridPyramidMap,begin);
  int endIndex   = getEndIndex(gridPyramidMap,end);
  int positionInBuffer = 0;
  std::vector<int> tmp;
  if(firstGrid == lastGrid)
  {
    // read pyramids from the first grid (start at beginIndex and read endIndex)
    tmp = readPyramids(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
  }
  else
  {
    // read pyramids from the first grid (start at beginIndex and read to the end of the file)
    tmp = readPyramids(gridFiles[firstGrid],beginIndex,gridPyramidMap[firstGrid+1]
                                                       -gridPyramidMap[firstGrid],isBigEndian[firstGrid]);
  }
  for(int pyramid : tmp)
    pyramidBuffer[positionInBuffer++] = pyramid + gridNodeMap[firstGrid];
  tmp.clear();

  // read all pyramids from grids between first and last grid
  for(int i=firstGrid+1;i<lastGrid;i++)
  {
    tmp = readPyramids(gridFiles[i],isBigEndian[i]);
    for(int pyramid : tmp)
      pyramidBuffer[positionInBuffer++] = pyramid + gridNodeMap[i];
    tmp.clear();
  }

  // read pyramids from last grid (start at zero and end at endIndex)
  if(lastGrid > firstGrid)
  {
    tmp = readPyramids(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
    for(int pyramid : tmp)
      pyramidBuffer[positionInBuffer++] = pyramid + gridNodeMap[lastGrid];
  }
  return pyramidBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getPrisms(int begin,int end)
{
  using namespace UgridReader;
  std::vector<int> prismBuffer(6*(end-begin),0);
  int firstGrid  = getFirstGrid(gridPrismMap,begin);
  int lastGrid   = getLastGrid(gridPrismMap,end);
  int beginIndex = getBeginIndex(gridPrismMap,begin);
  int endIndex   = getEndIndex(gridPrismMap,end);
  int positionInBuffer = 0;

  std::vector<int> tmp;
  if(firstGrid == lastGrid)
  {
    // read prisms from the first grid (start at beginIndex and read to endIndex)
    tmp = readPrisms(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
  }
  else
  {
    // read prisms from the first grid (start at beginIndex and read to the end of the file)
    tmp = readPrisms(gridFiles[firstGrid],beginIndex,gridPrismMap[firstGrid+1]
                                                     -gridPrismMap[firstGrid],isBigEndian[firstGrid]);
  }
  for(int prism : tmp)
    prismBuffer[positionInBuffer++] = prism + gridNodeMap[firstGrid];
  tmp.clear();

  // read all prisms from grids between first and last grid
  for(int i=firstGrid+1;i<lastGrid;i++)
  {
    tmp = readPrisms(gridFiles[i],isBigEndian[i]);
    for(int prism : tmp)
      prismBuffer[positionInBuffer++] = prism + gridNodeMap[i];
    tmp.clear();
  }

  // read prisms from last grid (start at zero and end at endIndex)
  if(lastGrid > firstGrid)
  {
    tmp = readPrisms(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
    for(int prism : tmp)
      prismBuffer[positionInBuffer++] = prism + gridNodeMap[lastGrid];
  }
  return prismBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getHexs(int begin,int end)
{
  using namespace UgridReader;
  std::vector<int> hexBuffer(8*(end-begin),0.0);
  int firstGrid  = getFirstGrid(gridHexMap,begin);
  int lastGrid   = getLastGrid(gridHexMap,end);
  int beginIndex = getBeginIndex(gridHexMap,begin);
  int endIndex   = getEndIndex(gridHexMap,end);
  int positionInBuffer = 0;
  std::vector<int> tmp;
  if(firstGrid == lastGrid)
  {
    // read hexs from the first grid (start at beginIndex and read to endIndex)
    tmp = readHexs(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
  }
  else
  {
    // read hexs from the first grid (start at beginIndex and read to the end of the file)
    tmp = readHexs(gridFiles[firstGrid],beginIndex,gridHexMap[firstGrid+1]
                                                   -gridHexMap[firstGrid],isBigEndian[firstGrid]);
  }
  for(int hex : tmp)
    hexBuffer[positionInBuffer++] = hex + gridNodeMap[firstGrid];
  tmp.clear();

  // read all hexs from grids between first and last grid
  for(int i=firstGrid+1;i<lastGrid;i++)
  {
    tmp = readHexs(gridFiles[i],isBigEndian[i]);
    for(int hex : tmp)
      hexBuffer[positionInBuffer++] = hex + gridNodeMap[i];
    tmp.clear();
  }

  // read hexs from last grid (start at zero and end at endIndex)
  if(lastGrid > firstGrid)
  {
    tmp = readHexs(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
    for(int hex : tmp)
      hexBuffer[positionInBuffer++] = hex + gridNodeMap[lastGrid];
  }
  return hexBuffer;
}

inline int Parfait::ParallelMeshReader::getFirstGrid(std::vector<int> &gridMap,int begin)
{
  int ngrid = (int)gridFiles.size();
  for(int i=0;i<ngrid;i++)
    if(begin <= gridMap[i+1])
      return i;
  assert(false);
}

inline int Parfait::ParallelMeshReader::getLastGrid(std::vector<int> &gridMap,int end)
{
  if(0==end){return 0;}
  int ngrid = (int)gridFiles.size();
  for(int i=ngrid-1;i>=0;i--)
    if(end >= gridMap[i])
      return i;
  assert(false);
}

inline int Parfait::ParallelMeshReader::getBeginIndex(std::vector<int> &gridMap,int begin)
{
  int ngrid = (int)gridFiles.size();
  for(int i=0;i<ngrid;i++)
  {
    if(begin <= gridMap[i+1])
      return begin-gridMap[i];
  }
  assert(false);
}

inline int Parfait::ParallelMeshReader::getEndIndex(std::vector<int> &gridMap,int end)
{
  if(0==end){return 0;}
  int ngrid = (int)gridFiles.size();
  for(int i=ngrid-1;i>=0;i--)
    if(end >= gridMap[i])
      return end-gridMap[i];
  assert(false);
}
