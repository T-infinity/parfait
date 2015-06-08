#include "message_passer.h"
#include "range_loop.h"
using std::vector;
using namespace MessagePasser;
namespace Parfait {
    template<typename Mesh>
    VtkSurfaceWriter<Mesh>::VtkSurfaceWriter(Mesh &mesh_in, std::string name_in)
            : mesh(mesh_in),
              filename(name_in) {
        filename.append(".vtu");
        setUpLocalLists();
    }

    template<class Mesh>
    void VtkSurfaceWriter<Mesh>::addScalarField(bool isNodeData, string fieldName, int *data) {
        int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
        if (Rank() == 0) {
            //printf("Adding integer field of length %i\n",n);
        }
        intFields.push_back(VtkField<int>(isNodeData, fieldName, 1, n, data, boundaryNodeIdMap));
    }

    template<class Mesh>
    void VtkSurfaceWriter<Mesh>::addScalarField(bool isNodeData, string fieldName, float *data) {
        int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
        floatFields.push_back(
                VtkField<float>(isNodeData, fieldName, 1, n, data, boundaryNodeIdMap));
    }

    template<class Mesh>
    void VtkSurfaceWriter<Mesh>::addScalarField(bool isNodeData, string fieldName, double *data) {
        int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
        doubleFields.push_back(
                VtkField<double>(isNodeData, fieldName, 1, n, data, boundaryNodeIdMap));
    }

    template<class Mesh>
    void VtkSurfaceWriter<Mesh>::addVectorField(bool isNodeData, string fieldName, int blockSize,
                                                int *data) {
        int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
        intFields.push_back(
                VtkField<int>(isNodeData, fieldName, blockSize, n, data, boundaryNodeIdMap));
    }

    template<class Mesh>
    void VtkSurfaceWriter<Mesh>::addVectorField(bool isNodeData, string fieldName, int blockSize,
                                                float *data) {
        int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
        floatFields.push_back(
                VtkField<float>(isNodeData, fieldName, blockSize, n, data, boundaryNodeIdMap));
    }

    template<class Mesh>
    void VtkSurfaceWriter<Mesh>::addVectorField(bool isNodeData, string fieldName, int blockSize,
                                                double *data) {
        int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
        doubleFields.push_back(
                VtkField<double>(isNodeData, fieldName, blockSize, n, data, boundaryNodeIdMap));
    }


//// --------------------------write vtk -------------------------------------------
    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::writeAscii() {
        // create vtu file and write header info
        if (Rank() == 0)
            writeHeader();
        int nproc = NumberOfProcesses();
        // fill in pieces
        for (int proc = 0; proc < nproc; proc++) {
            // write out pieces round robin
            Barrier();
            if (Rank() == proc)
                writePiece();
            Barrier();
        }
        Barrier();
        // close off containing tags
        if (Rank() == 0)
            writeFooter();
    }

    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::setUpLocalLists() {
        int nfaces = mesh.numberOfBoundaryFaces();
        vector<bool> isBoundNode(mesh.numberOfNodes(), false);
        // loop over boundary faces and set their nodes to true
        for (int i:range(nfaces)) {
            auto face = mesh.getNodesInBoundaryFace(i);
            for (int j = 0; j < face.size(); j++) {
                isBoundNode[face[j]] = true;
            }
        }
        // count up the total number of nodes in the surface mesh
        numberOfSurfaceNodes = 0;
        for (int i:range(mesh.numberOfNodes()))
            if (isBoundNode[i])
                numberOfSurfaceNodes++;
        // create list of surface node ids
        boundaryNodeIdMap.assign(mesh.numberOfNodes(), -1);
        int count = 0;
        for (int i:range(mesh.numberOfNodes()))
            if (isBoundNode[i])
                boundaryNodeIdMap[i] = count++;
    }

    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::writeHeader() {
        FILE *f = fopen(filename.c_str(), "w");
        assert(f != NULL);
        fprintf(f, "<?xml version=\"1.0\"?>\n");
        fprintf(f,
                "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n");
        fprintf(f, "   <UnstructuredGrid>\n");
        fclose(f);
    }

    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::writeFooter() {
        FILE *f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        fprintf(f, "   </UnstructuredGrid>\n");
        fprintf(f, "</VTKFile>\n");
        fclose(f);
    }

    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::writePiece() {
        FILE *f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        fprintf(f, "<Piece NumberOfPoints=\"%i\" NumberOfCells=\"%i\">\n",
                numberOfSurfaceNodes, mesh.numberOfBoundaryFaces());
        fclose(f);
        writeFieldData();
        writeNodes();
        f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        fprintf(f, "  <Cells>\n");
        fclose(f);
        writeCellConnectivity();
        writeCellOffsets();
        writeCellTypes();
        f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        fprintf(f, "  </Cells>\n");
        fprintf(f, "</Piece>\n");
        fflush(f);
        fclose(f);
    }

//// ---------------------------------------write field data------------------------------
    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::writeFieldData() {
        FILE *f = fopen(filename.c_str(), "a");
        assert(f != NULL);

        fprintf(f, "	<CellData>\n");
        fclose(f);
        // append cell based integer data
        for (auto &field:intFields)
            if (!field.isNodeData()) {
                if (Rank() == 0) {
                    //printf("appending an integer field for cell data\n");
                }
                field.append(filename);
            }
        // append cell based float data
        for (auto &field:floatFields)
            if (!field.isNodeData())
                field.append(filename);
        // append cell based double data
        for (auto &field:doubleFields)
            if (!field.isNodeData())
                field.append(filename);
        f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        fprintf(f, "	</CellData>\n");
        fprintf(f, " <PointData>\n");
        fclose(f);
        // append node based integer data
        for (auto &field:intFields)
            if (field.isNodeData())
                field.append(filename);
        // append node based float data
        for (auto &field:floatFields)
            if (field.isNodeData())
                field.append(filename);
        // append node based double data
        for (auto &field:doubleFields)
            if (field.isNodeData())
                field.append(filename);
        f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        fprintf(f, "	</PointData>\n");
        fclose(f);
    }


//// --------------------------write points -------------------------------------------
    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::writeNodes() {
        FILE *f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        fprintf(f, "  <Points>\n");
        fprintf(f, "    <DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n");
        int count = 0;
        for (int i:range(mesh.numberOfNodes())) {
            if (boundaryNodeIdMap[i] == -1)
                continue;
            double p[3];
            mesh.getNode(i, p);
            fprintf(f, "            %lf %lf %lf\n", p[0], p[1], p[2]);
            count++;
        }
        assert(count == numberOfSurfaceNodes);
        fprintf(f, "    </DataArray>\n");
        fprintf(f, "  </Points>\n");
        fclose(f);
    }

    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::writeCellOffsets() {
        FILE *f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        fprintf(f, "     <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n");
        int offset = 0;
        for (int i:range(mesh.numberOfBoundaryFaces())) {
            offset += mesh.numberOfNodesInBoundaryFace(i);
            fprintf(f, "          %i\n", offset);
        }
        fprintf(f, "     </DataArray>\n");
        fclose(f);
    }

    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::writeCellTypes() {
        FILE *f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        // set cell type VTK_POLYGON (=7)
        fprintf(f, "     <DataArray type=\"Int32\" Name=\"types\" format=\"ascii\">\n");
        for (int i:range(mesh.numberOfBoundaryFaces()))
            fprintf(f, "          7\n");
        fprintf(f, "     </DataArray>\n");
        fclose(f);
    }


/// ---------------------------------write faces-----------------------------------------
    template<typename Mesh>
    void VtkSurfaceWriter<Mesh>::writeCellConnectivity() {
        FILE *f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        fprintf(f, "     <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n");
        for (int i:range(mesh.numberOfBoundaryFaces())) {
            auto face = mesh.getNodesInBoundaryFace(i);
            fprintf(f, "					");
            for (int j = 0; j < face.size(); j++) {
                fprintf(f, "%i ", boundaryNodeIdMap[face[j]]);
            }
            // newline after each cell
            fprintf(f, "\n");
        }

        fprintf(f, "     </DataArray>\n");
        fclose(f);
    }
}