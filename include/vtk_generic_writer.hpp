using namespace MessagePasser;

template<class MeshType>
VtkWriter<MeshType>::VtkWriter(MeshType &mesh_in,std::string meshName)
	:mesh(mesh_in),
		filename(meshName)
{
	filename.append(".vtu");
}

template<class MeshType>
void VtkWriter<MeshType>::addScalarField(bool isNodeData,string fieldName,int *data)
{
	int n = isNodeData?mesh.numberOfNodes():mesh.numberOfCells();
	intFields.push_back(VtkField<int>(isNodeData,fieldName,1,n,data));
}

template<class MeshType>
void VtkWriter<MeshType>::addScalarField(bool isNodeData,string fieldName,float *data)
{
	int n = isNodeData?mesh.numberOfNodes():mesh.numberOfCells();
	floatFields.push_back(VtkField<float>(isNodeData,fieldName,1,n,data));
}

template<class MeshType>
void VtkWriter<MeshType>::addScalarField(bool isNodeData,string fieldName,double *data)
{
	int n = isNodeData?mesh.numberOfNodes():mesh.numberOfCells();
	doubleFields.push_back(VtkField<double>(isNodeData,fieldName,1,n,data));
}

template<class MeshType>
void VtkWriter<MeshType>::addVectorField(bool isNodeData,string fieldName,int blockSize,int *data)
{
	int n = isNodeData?mesh.numberOfNodes():mesh.numberOfCells();
	intFields.push_back(VtkField<int>(isNodeData,fieldName,blockSize,n,data));
}

template<class MeshType>
void VtkWriter<MeshType>::addVectorField(bool isNodeData,string fieldName,int blockSize,float *data)
{
	int n = isNodeData?mesh.numberOfNodes():mesh.numberOfCells();
	floatFields.push_back(VtkField<float>(isNodeData,fieldName,blockSize,n,data));
}

template<class MeshType>
void VtkWriter<MeshType>::addVectorField(bool isNodeData,string fieldName,int blockSize,double *data)
{
	int n = isNodeData?mesh.numberOfNodes():mesh.numberOfCells();
	doubleFields.push_back(VtkField<double>(isNodeData,fieldName,blockSize,n,data));
}

//// --------------------------write vtk -------------------------------------------
template<class MeshType>
void VtkWriter<MeshType>::writeAscii()
{
	global_nnodes = ParallelSum(mesh.numberOfNodes(),0);
	global_ncells = ParallelSum(mesh.numberOfCells(),0);
	int nproc = NumberOfProcesses();
	// create vtu file and write header info
	if(Rank() == 0)
		writeHeader();
	
	// fill in pieces
	for(int proc=0;proc<nproc;proc++)
	{
		// write out pieces round robin
		Barrier();
		if(Rank() == proc)
			writePiece();
		Barrier();
	}
	// close off containing tags
	if(Rank() == 0)
		writeFooter();
}

template<class MeshType>
void VtkWriter<MeshType>::writeHeader()
{
	FILE *f = fopen(filename.c_str(),"w");
	assert(f!=NULL);
	fprintf(f,"<?xml version=\"1.0\"?>\n");
	fprintf(f,"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n");
	fprintf(f,"   <UnstructuredGrid>\n");
	fclose(f);
}

template<class MeshType>
void VtkWriter<MeshType>::writeFooter()
{
	FILE *f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"   </UnstructuredGrid>\n");
	fprintf(f,"</VTKFile>\n");	
	fclose(f);
}

template<class MeshType>
void VtkWriter<MeshType>::writePiece()
{
	FILE *f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"<Piece NumberOfPoints=\"%i\" NumberOfCells=\"%i\">\n",
			mesh.numberOfNodes(),mesh.numberOfCells());
	fclose(f);
	writeFieldData();
	writePoints();
	f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"  <Cells>\n");
	fclose(f);
	writeCellConnectivity();
	writeCellOffsets();
	writeCellTypes();
	writeFaces();
	writeFaceOffsets();
	f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"  </Cells>\n");
	fprintf(f,"</Piece>\n");
	fclose(f);
}
//// ---------------------------------------write field data------------------------------
template<class MeshType>
void VtkWriter<MeshType>::writeFieldData()
{
	FILE *f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	
	fprintf(f,"	<CellData>\n");	
	fclose(f);
	// append cell based integer data
	for(auto &field:intFields)
		if(!field.isNodeData())
			field.append(filename);
	// append cell based float data
	for(auto &field:floatFields)
		if(!field.isNodeData())
			field.append(filename);
	// append cell based double data
	for(auto &field:doubleFields)
		if(!field.isNodeData())
			field.append(filename);
	f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"	</CellData>\n");
	fprintf(f," <PointData>\n");
	fclose(f);
	// append node based integer data
	for(auto &field:intFields)
		if(field.isNodeData())
			field.append(filename);
	// append node based float data
	for(auto &field:floatFields)
		if(field.isNodeData())
			field.append(filename);
	// append node based double data
	for(auto &field:doubleFields)
		if(field.isNodeData())
			field.append(filename);
	f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"	</PointData>\n");
	fclose(f);
}
//// --------------------------write points -------------------------------------------
template<class MeshType>
void VtkWriter<MeshType>::writePoints()
{
	FILE *f = fopen(filename.c_str(),"a");
	assert(f != NULL);
	fprintf(f,"  <Points>\n");
	fprintf(f,"    <DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n");
	int local_nnodes = mesh.numberOfNodes();
	for(int i=0;i<local_nnodes;i++)
	{
		double p[3];
		mesh.getNode(i,p);
		fprintf(f,"            %lf %lf %lf\n",p[0],p[1],p[2]);
	}
	fprintf(f,"    </DataArray>\n");
	fprintf(f,"  </Points>\n");
	fclose(f);
}
//// ------------------------------------write cells-------------------------------------
template<class MeshType>
void VtkWriter<MeshType>::writeCellConnectivity()
{
	FILE *f = fopen(filename.c_str(),"a");
	assert(f != NULL);
	fprintf(f,"     <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n");
	for(auto cell:mesh.cells())
	{
		fprintf(f,"         ");
		auto vertices = cell.getNodes();
		for(int i=0;i<vertices.size();i++)
			fprintf(f,"%i ",vertices[i]);
		fprintf(f,"\n");
	}
	fprintf(f,"     </DataArray>\n");
	fclose(f);
}

template<class MeshType>
void VtkWriter<MeshType>::writeCellOffsets()
{
	FILE *f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"     <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n");
	int offset = 0;
	for(auto cell:mesh.cells())
	{		
		offset += cell.numberOfNodes();
		fprintf(f,"          %i\n",offset);
	}
	fprintf(f,"     </DataArray>\n");
	fclose(f);
}

template<class MeshType>
void VtkWriter<MeshType>::writeCellTypes()
{
	FILE *f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"     <DataArray type=\"Int32\" Name=\"types\" format=\"ascii\">\n");
	for(auto cell:mesh.cells())
		fprintf(f,"          42\n");
	fprintf(f,"     </DataArray>\n");
	fclose(f);
}


/// ---------------------------------write faces-----------------------------------------
template<class MeshType>
void VtkWriter<MeshType>::writeFaces()
{
	FILE *f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"     <DataArray type=\"Int32\" Name=\"faces\" format=\"ascii\">\n");
	for(auto cell:mesh.cells())
	{
		int nfaces = cell.numberOfFaces();
		// write how many faces the cell has
		fprintf(f,"                  %i ",nfaces);
		for(auto face:cell)
		{
			int nvertices = face.numberOfNodes();
			// write how many nodes each face has
			fprintf(f,"%i ",nvertices);
			// then write the nodes
			for(int nodeId:face)
				fprintf(f,"%i ",nodeId);
		}
		// newline after each cell
		fprintf(f,"\n");
	}
	fprintf(f,"     </DataArray>\n");
	fclose(f);
}

template<class MeshType>
void VtkWriter<MeshType>::writeFaceOffsets()
{
	FILE *f = fopen(filename.c_str(),"a");
	assert(f!=NULL);
	fprintf(f,"     <DataArray type=\"Int32\" Name=\"faceoffsets\" format=\"ascii\">\n");
	int offset = 0;
	for(auto cell:mesh.cells())
	{
		offset += 1; // the int that tells how many faces
		offset += cell.numberOfFaces();
		for(auto face:cell)
			offset += face.numberOfNodes();
		fprintf(f,"        %i\n",offset);
	}
	fprintf(f,"     </DataArray>\n");
	fclose(f);
}
