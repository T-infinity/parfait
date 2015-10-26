#ifndef VTK_SURFACE_WRITER_H
#define VTK_SURFACE_WRITER_H
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include "VtkField.h"


namespace Parfait {
    template<typename Mesh>
    class VtkSurfaceWriter {
    public:
        VtkSurfaceWriter(Mesh &mesh, std::string name);
        void writeAscii();

        void addScalarField(bool isNodeData, std::string fieldName, int *data);
        void addScalarField(bool isNodeData, std::string fieldName, float *data);
        void addScalarField(bool isNodeData, std::string fieldName, double *data);
        void addVectorField(bool isNodeData, std::string fieldName, int blockSize, int *data);
        void addVectorField(bool isNodeData, std::string fieldName, int blockSize, float *data);
        void addVectorField(bool isNodeData, std::string fieldName, int blockSize, double *data);

    private:
        Mesh &mesh;
        std::string filename;
        int numberOfSurfaceNodes;
        std::vector<int> boundaryNodeIdMap;
        std::vector<VtkField<int> > intFields;
        std::vector<VtkField<float> > floatFields;
        std::vector<VtkField<double> > doubleFields;

        void setUpLocalLists();
        void writeHeader();
        void writeFooter();
        void writePiece();
        void writeNodes();
        void writeCellConnectivity();
        void writeCellOffsets();
        void writeCellTypes();
        void writeFieldData();
#if 0
		
		std::vector<VtkField*> fields;	
		int local_nnodes;
		int local_ncells;
		int global_nnodes;
		int global_ncells;
		
		// These functions can be accessed by derived classes, which
		// allows them to customize the way files are written.
		// (Not necessary to expose, but may be nice later).	
		void write_footer(FILE *f);	
		void write_piece(FILE *f);
	private:
		// Inheriting classes must implement the following functions,
		// then the whole visualization process should be pretty much plug
		// and play from there.	
		virtual int nnodes() = 0;
		virtual int nboundary_faces() = 0;
		virtual void get_point(int node_id,double p[3]) = 0;
		virtual int nnodes_in_boundary_face(int boundary_face_id) = 0;
		virtual void get_nodes_in_boundary_face(int boundary_face_id,int face[]) = 0;
	
		// These functions are needed by the base class, but the derived
		// classes should not have to worry about them	
		void set_up_local_lists();
		void calc_global_nnodes();	
		void calc_global_ncells();	
		void write_field_data(FILE *f);
		void write_points(FILE *f);
		void write_cell_offsets(FILE *f);
		void write_cell_connectivity(FILE *f);
		void write_cell_types(FILE *f);
		void write_data(FILE *f);

		// the base class will make a map to convert boundary nodes 
		// from global ids to local ids (local to the boundaries)
		std::vector<int>  boundary_node_id_map;	
#endif
    };
}
#include "VtkSurfaceWriter.hpp"

#endif
