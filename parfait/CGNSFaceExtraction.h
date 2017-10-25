#pragma once

namespace Parfait {
namespace CGNS {

inline void throwUnexpectedFace() {
    throw std::logic_error("Parfait::CGNS::FaceExtraction.h Encountered unexpected face");
}

template <typename T>
void getTetFace(const std::vector<T> &cell, int face_id, std::vector<T> &face) {
    face.resize(3);
    switch (face_id) {
        case 0:
            face[0] = cell[0];
            face[1] = cell[2];
            face[2] = cell[1];
            break;
        case 1:
            face[0] = cell[0];
            face[1] = cell[1];
            face[2] = cell[3];
            break;
        case 2:
            face[0] = cell[1];
            face[1] = cell[2];
            face[2] = cell[3];
            break;
        case 3:
            face[0] = cell[2];
            face[1] = cell[0];
            face[2] = cell[3];
            break;
        default:throwUnexpectedFace();
    }
}

template <typename T>
void getPyramidFace(const std::vector<T> &cell, int face_id, std::vector<T> &face) {
    switch(face_id){
        case 0:
            face.resize(4);
            face[0] = cell[0];
            face[1] = cell[3];
            face[2] = cell[2];
            face[3] = cell[1];
            break;
        case 1:
            face.resize(3);
            face[0] = cell[0];
            face[1] = cell[1];
            face[2] = cell[4];
            break;
        case 2:
            face.resize(3);
            face[0] = cell[1];
            face[1] = cell[2];
            face[2] = cell[4];
            break;
        case 3:
            face.resize(3);
            face[0] = cell[2];
            face[1] = cell[3];
            face[2] = cell[4];
            break;
        case 4:
            face.resize(3);
            face[0] = cell[3];
            face[1] = cell[0];
            face[2] = cell[4];
            break;
        default:
            throwUnexpectedFace();
    }
}

template <typename T>
void getPrismFace(const std::vector<T> &cell, int face_id, std::vector<T> &face) {
    switch(face_id) {
        case 0:
            face.resize(4);
            face[0] = cell[0];
            face[1] = cell[1];
            face[2] = cell[4];
            face[3] = cell[3];
            break;
        case 1:
            face.resize(4);
            face[0] = cell[1];
            face[1] = cell[2];
            face[2] = cell[5];
            face[3] = cell[4];
            break;
        case 2:
            face.resize(4);
            face[0] = cell[2];
            face[1] = cell[3];
            face[2] = cell[0];
            face[3] = cell[5];
            break;
        case 3:
            face.resize(3);
            face[0] = cell[0];
            face[1] = cell[2];
            face[2] = cell[1];
            break;
        case 4:
            face.resize(3);
            face[0] = cell[3];
            face[1] = cell[4];
            face[2] = cell[5];
            break;
        default:
            throwUnexpectedFace();
    }
}

template <typename T>
void getHexFace(const std::vector<T> &cell, int face_id, std::vector<T> &face) {
    face.resize(4);
    switch(face_id) {
        case 0:
            face[0] = cell[0];
            face[1] = cell[3];
            face[2] = cell[2];
            face[3] = cell[1];
            break;
        case 1:
            face[0] = cell[0];
            face[1] = cell[1];
            face[2] = cell[5];
            face[3] = cell[4];
            break;
        case 2:
            face[0] = cell[1];
            face[1] = cell[2];
            face[2] = cell[6];
            face[3] = cell[5];
            break;
        case 3:
            face[0] = cell[2];
            face[1] = cell[3];
            face[2] = cell[7];
            face[3] = cell[6];
            break;
        case 4:
            face[0] = cell[0];
            face[1] = cell[4];
            face[2] = cell[7];
            face[3] = cell[3];
            break;
        case 5:
            face[0] = cell[4];
            face[1] = cell[5];
            face[2] = cell[6];
            face[3] = cell[7];
            break;
        default:
            throwUnexpectedFace();
    }
}

}
}


