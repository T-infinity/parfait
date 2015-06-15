#include <assert.h>

namespace HexHelpers {
  void writeHeader(FILE *f, int numBoxes);
  void writeTags(FILE *f, std::vector<int> &tags);
  void writePoints(FILE *f, std::vector<Hex> &boxes);
  void writeConnectivity(FILE *f, std::vector<Hex> &boxes);
  void writeOffsets(FILE *f, std::vector<Hex> &boxes);
  void writeTypes(FILE *f, std::vector<Hex> &boxes);
}

inline Parfait::VtkHexWriter::VtkHexWriter(std::string filename_i)
        : filename(filename_i) {
    filename += ".vtu";
}

inline void Parfait::VtkHexWriter::addHex(Hex b) {
    boxes.push_back(b);
    tags.push_back(0);
}

inline void Parfait::VtkHexWriter::addHex(Hex b, int tag) {

    boxes.push_back(b);
    tags.push_back(tag);
}

inline void Parfait::VtkHexWriter::writeFile() {
    Parfait::VtkHexWriter::writeBoxes(filename, boxes, tags);
}

inline void Parfait::VtkHexWriter::writeBoxes(std::string filename,
                              std::vector<Hex> &boxes,
                              std::vector<int> &tags) {
    FILE *f = fopen(filename.c_str(),"w");

    HexHelpers::writeHeader(f, boxes.size());

    HexHelpers::writeTags(f, tags);
    HexHelpers::writePoints(f, boxes);
    HexHelpers::writeConnectivity(f, boxes);
    HexHelpers::writeOffsets(f, boxes);
    HexHelpers::writeTypes(f, boxes);

    fprintf(f,"\n</AppendedData>\n");
    fprintf(f,"\n</VTKFile>\n");

    fclose(f);
}

inline void HexHelpers::writeTags(FILE *f, std::vector<int> &tags) {
    uint32_t tags_length = sizeof(int)*tags.size();
    fwrite(&tags_length, sizeof(int), 1, f);
    fwrite(&tags[0], sizeof(int), tags.size(), f);
}

inline void HexHelpers::writePoints(FILE *f, std::vector<Hex> &boxes) {
    //Print the points
    uint32_t points_length = 192*boxes.size();
    fwrite(&points_length, sizeof(int), 1, f);

    for(auto &b : boxes)
    {
        for (unsigned int i = 0; i < 8; i++) {
            fwrite(&b[i][0], sizeof(double), 3, f);
        }
    }
}

inline void HexHelpers::writeConnectivity(FILE *f, std::vector<Hex> &boxes) {
    uint32_t connectivity_length = 8*sizeof(int64_t)*boxes.size();
    fwrite(&connectivity_length, sizeof(int), 1, f);

    int pointID = 0;
    std::vector<int64_t> connectivity(8);
    for(unsigned int boxID = 0; boxID < boxes.size(); boxID++) {
        for (unsigned int i = 0; i < 8; i++) {
            connectivity[i] = pointID++;
        }
        fwrite(&connectivity[0], sizeof(int64_t), 8, f);
    }
}

inline void HexHelpers::writeOffsets(FILE *f, std::vector<Hex> &boxes) {
    uint32_t offset_length = sizeof(int64_t)*boxes.size();
    fwrite(&offset_length, sizeof(int), 1, f);

    int64_t offset = 8;
    for (unsigned int boxID = 0; boxID < boxes.size(); boxID++) {
        fwrite(&offset, sizeof(int64_t), 1, f);
        offset += 8;
    }
}

inline void HexHelpers::writeTypes(FILE *f, std::vector<Hex> &boxes) {
    uint32_t types_length = sizeof(int64_t)*boxes.size();
    fwrite(&types_length, sizeof(int), 1, f);
    int64_t type = 12;
    for (unsigned int boxID = 0; boxID < boxes.size(); boxID++) {
        fwrite(&type, sizeof(int64_t), 1, f);
    }
}

inline void HexHelpers::writeHeader(FILE *f, int numBoxes) {
    int point_offset = numBoxes*4 + 4;
    int connectivity_offset = point_offset + 3*8*numBoxes*sizeof(double) + 4;
    int offset_offset = connectivity_offset + 8*8*numBoxes + 4;
    int type_offset = offset_offset + 8*numBoxes + 4;
    assert(f != NULL);
    fprintf(f,"<?xml version=\"1.0\"?>\n");
    fprintf(f,"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n");
    fprintf(f,"<UnstructuredGrid>\n");
    fprintf(f,"<Piece NumberOfPoints=\"%d\" NumberOfCells=\"%d\">\n", 8*numBoxes, numBoxes);
    fprintf(f,"		<CellData Scalars=\"tag\">\n");
    fprintf(f,"			<DataArray type=\"Int32\" Name=\"tag\" format=\"appended\" offset=\"0\">\n");
    fprintf(f,"			</DataArray>\n");
    fprintf(f,"		</CellData>\n");
    fprintf(f,"		<Points>\n");
    fprintf(f,"			<DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"appended\" offset=\"%d\">\n",point_offset);
    fprintf(f,"			</DataArray>\n");
    fprintf(f,"		</Points>\n");
    fprintf(f,"		<Cells>\n");
    fprintf(f,"			<DataArray type=\"Int64\" Name=\"connectivity\" format=\"appended\" offset=\"%d\">\n",connectivity_offset);
    fprintf(f,"			</DataArray>\n");
    fprintf(f,"			<DataArray type=\"Int64\" Name=\"offsets\" format=\"appended\" offset=\"%d\">\n", offset_offset);
    fprintf(f,"			</DataArray>\n");
    fprintf(f,"			<DataArray type=\"Int64\" Name=\"types\" format=\"appended\" offset=\"%d\">\n",type_offset);
    fprintf(f,"			</DataArray>\n");
    fprintf(f,"		</Cells>\n");
    fprintf(f,"</Piece>\n");
    fprintf(f,"</UnstructuredGrid>\n");
    fprintf(f,"<AppendedData encoding=\"raw\">\n");
    fprintf(f,"   _");
}
