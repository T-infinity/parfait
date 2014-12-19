#ifndef PARALLEL_VTK_H
#define PARALLEL_VTK_H
#include <vector>
#include <string>
#include <stdlib.h>
#include <assert.h>
#include "vtk_field.h"
#include "message_passer.h"
#include "generic_mesh.h"

using std::vector;
using std::string;

template<class MeshType>
class VtkWriter
{
	public:	
		VtkWriter(MeshType &meshInterface,string vtkFilename);
		void writeAscii();
        void writeFile();
		void addScalarField(bool isNodeData,string fieldName,int *data);
		void addScalarField(bool isNodeData,string fieldName,float *data);
		void addScalarField(bool isNodeData,string fieldName,double *data);
		void addVectorField(bool isNodeData,string fieldName,int blockSize,int *data);
		void addVectorField(bool isNodeData,string fieldName,int blockSize,float *data);
		void addVectorField(bool isNodeData,string fieldName,int blockSize,double *data);
	private:
		Mesh<MeshType> mesh;
		vector<VtkField<int> >    intFields;	
		vector<VtkField<float> >  floatFields;	
		vector<VtkField<double> > doubleFields;	
		int global_nnodes;
		int global_ncells;
		std::string filename;
		
		void writeHeader();
		void writeFooter();
		void writePiece();
		void writePoints();
		void writeCellConnectivity();
		void writeCellOffsets();
		void writeCellTypes();
		void writeFaces();
		void writeFaceOffsets();
		void writeFieldData();
};

#include "vtk_generic_writer.hpp"

#endif
