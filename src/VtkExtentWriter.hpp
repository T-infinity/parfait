#include "Point.h"

#include <assert.h>

namespace Parfait {
  inline void writePoints(FILE *f, std::vector<Extent<double>> &boxes);
  inline void writeConnectivity(FILE *f, std::vector<Extent<double>> &boxes);
  inline void writeOffsets(FILE *f, std::vector<Extent<double>> &boxes);
  inline void writeTypes(FILE *f, std::vector<Extent<double>> &boxes);
}

inline Parfait::VtkExtentWriter::VtkExtentWriter(std::string filename_i): filename(filename_i)
{
    filename += ".vtu";
}

inline void Parfait::VtkExtentWriter::addExtent(const Extent<double> &b)
{
    boxes.push_back(b);
    tags.push_back(0);
}

inline void Parfait::VtkExtentWriter::addExtent(const Extent<double> &b, int tag)
{

    boxes.push_back(b);
    tags.push_back(tag);
}

inline void Parfait::VtkExtentWriter::writeFile()
{
    Parfait::VtkExtentWriter::writeExtents(filename, boxes, tags);
}

inline void Parfait::VtkExtentWriter::writeExtents(std::string filename, std::vector<Extent<double>> &boxes, std::vector<int> &tags)
{
    FILE *f = fopen(filename.c_str(),"w");

    writeHeader(f, boxes.size());

    writeTags(f, tags);
    writePoints(f, boxes);
    writeConnectivity(f, boxes);
    writeOffsets(f, boxes);
    writeTypes(f, boxes);

    fprintf(f,"\n</AppendedData>\n");
    fprintf(f,"\n</VTKFile>\n");

    fclose(f);
}

inline void Parfait::VtkExtentWriter::writeTags(FILE *f, std::vector<int> &tags)
{
    uint32_t tags_length = sizeof(int)*tags.size();
    fwrite(&tags_length, sizeof(int), 1, f);
    fwrite(&tags[0], sizeof(int), tags.size(), f);
}

inline void Parfait::writePoints(FILE *f, std::vector<Extent<double>> &boxes)
{
    uint32_t points_length = 192*boxes.size();
    fwrite(&points_length, sizeof(int), 1, f);

    for(auto &b : boxes)
    {
        Point<double> lo = b.lo;
        Point<double> hi = b.hi;
        Point<double> one    = Point<double>(lo[0],lo[1],lo[2]);
        Point<double> two    = Point<double>(hi[0],lo[1],lo[2]);
        Point<double> three  = Point<double>(hi[0],hi[1],lo[2]);
        Point<double> four   = Point<double>(lo[0],hi[1],lo[2]);
        Point<double> five   = Point<double>(lo[0],lo[1],hi[2]);
        Point<double> six    = Point<double>(hi[0],lo[1],hi[2]);
        Point<double> seven  = Point<double>(hi[0],hi[1],hi[2]);
        Point<double> eight  = Point<double>(lo[0],hi[1],hi[2]);

        fwrite(&one[0],  sizeof(double), 3, f);
        fwrite(&two[0],  sizeof(double), 3, f);
        fwrite(&three[0],sizeof(double), 3, f);
        fwrite(&four[0], sizeof(double), 3, f);
        fwrite(&five[0], sizeof(double), 3, f);
        fwrite(&six[0],  sizeof(double), 3, f);
        fwrite(&seven[0],sizeof(double), 3, f);
        fwrite(&eight[0],sizeof(double), 3, f);
    }
}

inline void Parfait::writeConnectivity(FILE *f, std::vector<Extent<double>> &boxes)
{
    uint32_t connectivity_length = 8*sizeof(int64_t)*boxes.size();
    fwrite(&connectivity_length, sizeof(int), 1, f);

    int pointID = 0;
    std::vector<int64_t> connectivity(8);
    for(unsigned int boxID = 0; boxID < boxes.size(); boxID++)
    {
        for (unsigned int i = 0; i < 8; i++) 
        {
            connectivity[i] = pointID++;
        }
        fwrite(&connectivity[0], sizeof(int64_t), 8, f);
    }

}

inline void Parfait::writeOffsets(FILE *f, std::vector<Extent<double>> &boxes)
{
    uint32_t offset_length = sizeof(int64_t)*boxes.size();
    fwrite(&offset_length, sizeof(int), 1, f);

    int64_t offset = 8;
    for (unsigned int boxID = 0; boxID < boxes.size(); boxID++) 
    {
        fwrite(&offset, sizeof(int64_t), 1, f);
        offset += 8;
    }
}

inline void Parfait::writeTypes(FILE *f, std::vector<Extent<double>> &boxes)
{
    uint32_t types_length = sizeof(int64_t)*boxes.size();
    fwrite(&types_length, sizeof(int), 1, f);
    int64_t type = 12;
    for (unsigned int boxID = 0; boxID < boxes.size(); boxID++) 
    {
        fwrite(&type, sizeof(int64_t), 1, f);
    }
}

inline void Parfait::VtkExtentWriter::writeHeader(FILE *f, int numExtentes)
{
    int point_offset = numExtentes*4 + 4;
    int connectivity_offset = point_offset + 3*8*numExtentes*sizeof(double) + 4;
    int offset_offset = connectivity_offset + 8*8*numExtentes + 4;
    int type_offset = offset_offset + 8*numExtentes + 4;
    assert(f != NULL);
    fprintf(f,"<?xml version=\"1.0\"?>\n");
    fprintf(f,"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n");
    fprintf(f,"<UnstructuredGrid>\n");
    fprintf(f,"<Piece NumberOfPoints=\"%d\" NumberOfCells=\"%d\">\n", 8*numExtentes, numExtentes);
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
