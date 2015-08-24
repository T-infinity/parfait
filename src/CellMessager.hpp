

CellMessager::CellMessager(std::vector<int> &triangles_in, std::vector<int> &triangleTags)
        : triangles(triangles_in){

}

void CellMessager::sendTriangles(const std::vector<int> &packedTriangles) {
    int numTriangles = packedTriangles.size() / 4;
    for(int t = 0; t < numTriangles; t++){
        for (int i = 0; i < 3; i++) {
            int index = t*4 + i;
            triangles.push_back(packedTriangles[index]);
        }
    }
}
