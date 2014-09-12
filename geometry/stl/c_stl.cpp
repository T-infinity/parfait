#include "c_stl.h"
#include <Point.h>
#include <assert.h>
#include <voxel3d.h>
#include <vector_tools.h>

inline void CROSS(double *dest, double *v1, double *v2) {
    dest[0] = v1[1] * v2[2] - v1[2] * v2[1];
    dest[1] = v1[2] * v2[0] - v1[0] * v2[2];
    dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

inline double DOT(double *v1, double *v2) {
    return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

inline void SUB(double *dest, double *v1, double *v2) {
    dest[0] = v1[0] - v2[0];
    dest[1] = v1[1] - v2[1];
    dest[2] = v1[2] - v2[2];
}

double c_STL_FACET::GetMax(int i) {
    double t = p1[i];
    t = (t > p2[i]) ? (t) : (p2[i]);
    t = (t > p3[i]) ? (t) : (p3[i]);
    return t;
}

Point &c_STL_FACET::operator[](int i) {
    switch (i) {
        case 0:
            return p1;
            break;
        case 1:
            return p2;
            break;
        case 2:
            return p3;
            break;
        default:
            printf(
                "\n\n\nRequested element %d of an stl facet.  This is "
                "invalid.\n\n",
                i);
            exit(0);
    }
}

double c_STL_FACET::GetMin(int i) {
    double t = p1[i];
    t = (t < p2[i]) ? (t) : (p2[i]);
    t = (t < p3[i]) ? (t) : (p3[i]);
    return t;
}

double c_STL_FACET::GetXMin() { return GetMax(0); }
double c_STL_FACET::GetYMin() { return GetMax(1); }
double c_STL_FACET::GetZMin() { return GetMax(2); }

bool c_STL_FACET::DoesRayIntersect(double orig[3], double dir[3], double &t) {

    double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
    double det, inv_det;
    double u, v;

    SUB(edge1, &p2[0], &p1[0]);
    SUB(edge2, &p3[0], &p1[0]);

    /* begin calculating the determinant -also used to calculate U parameter. */
    CROSS(pvec, dir, edge2);

    /* if determinant is near zero, ray lies in plane of triangle. */
    det = DOT(edge1, pvec);

    if (fabs(det) < 1.0e-14) {
        return false;
    }

    inv_det = 1.0 / det;
    SUB(tvec, orig, &p1[0]);

    u = DOT(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0) {
        return false;
    }

    CROSS(qvec, tvec, edge1);
    v = DOT(dir, qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    t = DOT(edge2, qvec) * inv_det;
    if (t < 0.0) {
        return false;
    }
    return true;
}

bool c_STL_FACET::WhereDoesEdgeIntersect(Point &edgelo, Point &edgehi,
                                         Point &point) {

    double dir[3];
    double dist;
    dir[0] = edgehi[0] - edgelo[0];
    dir[1] = edgehi[1] - edgelo[1];
    dir[2] = edgehi[2] - edgelo[2];
    dist = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);

    double oodist = 1.0 / dist;
    dir[0] *= oodist;
    dir[1] *= oodist;
    dir[2] *= oodist;

    double t;
    double lo[3];
    if (!DoesRayIntersect(&edgelo[0], dir, t)) {
        return false;
    }
    if (t > dist) {
        return false;
    }

    point[0] = edgelo[0] + dir[0] * t;
    point[1] = edgelo[1] + dir[1] * t;
    point[2] = edgelo[2] + dir[2] * t;
    return true;
}

void c_STL_FACET::TestRayIntersection() {
    p1[0] = 0;
    p1[1] = 0;
    p1[2] = 0;

    p2[0] = 0.5;
    p2[1] = 0;
    p2[2] = 0;

    p3[0] = 0;
    p3[1] = 0.5;
    p3[2] = 0;

    Point edgelo;
    edgelo[0] = 0.25;
    edgelo[1] = 0.25;
    edgelo[2] = 1;

    Point edgehi;
    edgehi[0] = 0.25;
    edgehi[1] = 0.25;
    edgehi[2] = -1;

    Point intersect;

    if (WhereDoesEdgeIntersect(edgelo, edgehi, intersect)) {
        printf("\nIntersection occurs at %lf %lf %lf", intersect[0],
               intersect[1], intersect[2]);
    } else {
        printf("\nNo Intersection reported.");
    }
}

void c_STL_FACET::GetExtent(double min_spacing, double extent[6]) {

    double x, y, z;
    x = (p1[0] < p2[0]) ? (p1[0]) : (p2[0]);
    x = (x < p3[0]) ? (x) : (p3[0]);
    y = (p1[1] < p2[1]) ? (p1[1]) : (p2[1]);
    y = (y < p3[1]) ? (y) : (p3[1]);
    z = (p1[2] < p2[2]) ? (p1[2]) : (p2[2]);
    z = (z < p3[2]) ? (z) : (p3[2]);
    extent[0] = x - min_spacing;
    extent[1] = y - min_spacing;
    extent[2] = z - min_spacing;

    x = (p1[0] > p2[0]) ? (p1[0]) : (p2[0]);
    x = (x > p3[0]) ? (x) : (p3[0]);
    y = (p1[1] > p2[1]) ? (p1[1]) : (p2[1]);
    y = (y > p3[1]) ? (y) : (p3[1]);
    z = (p1[2] > p2[2]) ? (p1[2]) : (p2[2]);
    z = (z > p3[2]) ? (z) : (p3[2]);
    extent[3] = x + min_spacing;
    extent[4] = y + min_spacing;
    extent[5] = z + min_spacing;
}

Point c_STL_FACET::GetClosestPoint_Approx(Point point, double &dist) {

    Point closest;
    double dtemp1 = point.distance(p1);
    double dtemp2 = point.distance(p2);
    double dtemp3 = point.distance(p3);
    if (dtemp1 < dtemp2) {
        closest = p1;
        dist = dtemp1;
    } else {
        closest = p2;
        dist = dtemp2;
    }
    if (dtemp3 < dist) {
        closest = p3;
        dist = dtemp3;
    }
    return closest;
}

Point c_STL_FACET::GetClosestPoint_Exact(Point safe, double &dist) {

    Point point = safe;
    Point diff = p1 - point;
    Point edge0 = p2 - p1;
    Point edge1 = p3 - p1;

    double a00 = edge0.squared_magnitude();
    double a01 = dot(edge0, edge1);
    double a11 = edge1.squared_magnitude();
    double b0 = dot(diff, edge0);
    double b1 = dot(diff, edge1);
    double c = diff.squared_magnitude();
    double det = fabs(a00 * a11 - a01 * a01);
    double s = a01 * b1 - a11 * b0;
    double t = a01 * b0 - a00 * b1;
    double sqrDistance = 1.0e20;
    //  \     |
    //   \reg2|
    //    \   |
    //     \  |
    //      \ |
    //       \|
    //        *P2
    //        |\
    //        | \
    //  reg3  |  \ reg1
    //        |   \
    //        |reg0\ 
    //        |     \ 
    //        |      \ P1
    // -------*-------*------->s
    //        |P0      \ 
    //  reg4  | reg5    \ reg6

    if (s + t <= det) {
        if (s < (double)0) {
            if (t < (double)0)  // region 4
            {
                if (b0 < (double)0) {
                    t = (double)0;
                    if (-b0 >= a00) {
                        s = (double)1;
                        sqrDistance = a00 + ((double)2) * b0 + c;
                    } else {
                        s = -b0 / a00;
                        sqrDistance = b0 * s + c;
                    }
                } else {
                    s = (double)0;
                    if (b1 >= (double)0) {
                        t = (double)0;
                        sqrDistance = c;
                    } else if (-b1 >= a11) {
                        t = (double)1;
                        sqrDistance = a11 + ((double)2) * b1 + c;
                    } else {
                        t = -b1 / a11;
                        sqrDistance = b1 * t + c;
                    }
                }
            } else  // region 3
            {
                s = (double)0;
                if (b1 >= (double)0) {
                    t = (double)0;
                    sqrDistance = c;
                } else if (-b1 >= a11) {
                    t = (double)1;
                    sqrDistance = a11 + ((double)2) * b1 + c;
                } else {
                    t = -b1 / a11;
                    sqrDistance = b1 * t + c;
                }
            }
        } else if (t < (double)0)  // region 5
        {
            t = (double)0;
            if (b0 >= (double)0) {
                s = (double)0;
                sqrDistance = c;
            } else if (-b0 >= a00) {
                s = (double)1;
                sqrDistance = a00 + ((double)2) * b0 + c;
            } else {
                s = -b0 / a00;
                sqrDistance = b0 * s + c;
            }
        } else  // region 0
        {
            // minimum at interior point
            double invDet = ((double)1) / det;
            s *= invDet;
            t *= invDet;
            sqrDistance = s * (a00 * s + a01 * t + ((double)2) * b0) +
                          t * (a01 * s + a11 * t + ((double)2) * b1) + c;
        }
    } else {
        double tmp0, tmp1, numer, denom;

        if (s < (double)0)  // region 2
        {
            tmp0 = a01 + b0;
            tmp1 = a11 + b1;
            if (tmp1 > tmp0) {
                numer = tmp1 - tmp0;
                denom = a00 - ((double)2) * a01 + a11;
                if (numer >= denom) {
                    s = (double)1;
                    t = (double)0;
                    sqrDistance = a00 + ((double)2) * b0 + c;
                } else {
                    s = numer / denom;
                    t = (double)1 - s;
                    sqrDistance = s * (a00 * s + a01 * t + ((double)2) * b0) +
                                  t * (a01 * s + a11 * t + ((double)2) * b1) +
                                  c;
                }
            } else {
                s = (double)0;
                if (tmp1 <= (double)0) {
                    t = (double)1;
                    sqrDistance = a11 + ((double)2) * b1 + c;
                } else if (b1 >= (double)0) {
                    t = (double)0;
                    sqrDistance = c;
                } else {
                    t = -b1 / a11;
                    sqrDistance = b1 * t + c;
                }
            }
        } else if (t < (double)0)  // region 6
        {
            tmp0 = a01 + b1;
            tmp1 = a00 + b0;
            if (tmp1 > tmp0) {
                numer = tmp1 - tmp0;
                denom = a00 - ((double)2) * a01 + a11;
                if (numer >= denom) {
                    t = (double)1;
                    s = (double)0;
                    sqrDistance = a11 + ((double)2) * b1 + c;
                } else {
                    t = numer / denom;
                    s = (double)1 - t;
                    sqrDistance = s * (a00 * s + a01 * t + ((double)2) * b0) +
                                  t * (a01 * s + a11 * t + ((double)2) * b1) +
                                  c;
                }
            } else {
                t = (double)0;
                if (tmp1 <= (double)0) {
                    s = (double)1;
                    sqrDistance = a00 + ((double)2) * b0 + c;
                } else if (b0 >= (double)0) {
                    s = (double)0;
                    sqrDistance = c;
                } else {
                    s = -b0 / a00;
                    sqrDistance = b0 * s + c;
                }
            }
        } else  // region 1
        {
            numer = a11 + b1 - a01 - b0;
            if (numer <= (double)0) {
                s = (double)0;
                t = (double)1;
                sqrDistance = a11 + ((double)2) * b1 + c;
            } else {
                denom = a00 - ((double)2) * a01 + a11;
                if (numer >= denom) {
                    s = (double)1;
                    t = (double)0;
                    sqrDistance = a00 + ((double)2) * b0 + c;
                } else {
                    s = numer / denom;
                    t = (double)1 - s;
                    sqrDistance = s * (a00 * s + a01 * t + ((double)2) * b0) +
                                  t * (a01 * s + a11 * t + ((double)2) * b1) +
                                  c;
                }
            }
        }
    }

    // Account for numerical round-off error.
    if (sqrDistance < (double)0) {
        sqrDistance = (double)0;
    }

    point = p1 + s * edge0 + t * edge1;
    dist = safe.distance(point);  // sqrt(sqrDistance);
    return point;
}

// ==================== STL =============================

void c_STL::Rescale(double scale) {

    for (unsigned int i = 0; i < facets.size(); i++) {
        for (unsigned int j = 0; j < 3; j++) {
            facets[i].p1[j] /= scale;
        }
        for (unsigned int j = 0; j < 3; j++) {
            facets[i].p2[j] /= scale;
        }
        for (unsigned int j = 0; j < 3; j++) {
            facets[i].p3[j] /= scale;
        }
    }
}

void c_STL::Rescale() {

    Center();
    double length = MaxDistance();
    Rescale(length);
}

void c_STL::Translate(double shift[3]) {

    for (unsigned int i = 0; i < facets.size(); i++) {
        for (unsigned int j = 0; j < 3; j++) {
            facets[i].p1[j] += shift[j];
        }
        for (unsigned int j = 0; j < 3; j++) {
            facets[i].p2[j] += shift[j];
        }
        for (unsigned int j = 0; j < 3; j++) {
            facets[i].p3[j] += shift[j];
        }
    }
}

double c_STL::MaxDistance() {
    double max_d = -20e20;
    for (unsigned int i = 0; i < facets.size(); i++) {
        double r;
        r = sqrt(facets[i].p1[0] * facets[i].p1[0] +
                 facets[i].p1[1] * facets[i].p1[1] +
                 facets[i].p1[2] * facets[i].p1[2]);
        max_d = (max_d < r) ? (r) : (max_d);

        r = sqrt(facets[i].p2[0] * facets[i].p2[0] +
                 facets[i].p2[1] * facets[i].p2[1] +
                 facets[i].p2[2] * facets[i].p2[2]);
        max_d = (max_d < r) ? (r) : (max_d);

        r = sqrt(facets[i].p3[0] * facets[i].p3[0] +
                 facets[i].p3[1] * facets[i].p3[1] +
                 facets[i].p3[2] * facets[i].p3[2]);
        max_d = (max_d < r) ? (r) : (max_d);
    }
    return max_d;
}

void c_STL::Center() {
    //--- Loop over all the facets, find the min and max x, y, z and shift each
    //facet so the origin is at the middle.

    double x_min = 20e20;
    double x_max = -20e20;
    double y_min = 20e20;
    double y_max = -20e20;
    double z_min = 20e20;
    double z_max = -20e20;
    for (unsigned int i = 0; i < facets.size(); i++) {
        if (x_min > facets[i].p1[0]) {
            x_min = facets[i].p1[0];
        }
        if (y_min > facets[i].p1[1]) {
            y_min = facets[i].p1[1];
        }
        if (z_min > facets[i].p1[2]) {
            z_min = facets[i].p1[2];
        }
        if (x_min > facets[i].p2[0]) {
            x_min = facets[i].p2[0];
        }
        if (y_min > facets[i].p2[1]) {
            y_min = facets[i].p2[1];
        }
        if (z_min > facets[i].p2[2]) {
            z_min = facets[i].p2[2];
        }
        if (x_min > facets[i].p3[0]) {
            x_min = facets[i].p3[0];
        }
        if (y_min > facets[i].p3[1]) {
            y_min = facets[i].p3[1];
        }
        if (z_min > facets[i].p3[2]) {
            z_min = facets[i].p3[2];
        }

        if (x_max < facets[i].p1[0]) {
            x_max = facets[i].p1[0];
        }
        if (y_max < facets[i].p1[1]) {
            y_max = facets[i].p1[1];
        }
        if (z_max < facets[i].p1[2]) {
            z_max = facets[i].p1[2];
        }
        if (x_max < facets[i].p2[0]) {
            x_max = facets[i].p2[0];
        }
        if (y_max < facets[i].p2[1]) {
            y_max = facets[i].p2[1];
        }
        if (z_max < facets[i].p2[2]) {
            z_max = facets[i].p2[2];
        }
        if (x_max < facets[i].p3[0]) {
            x_max = facets[i].p3[0];
        }
        if (y_max < facets[i].p3[1]) {
            y_max = facets[i].p3[1];
        }
        if (z_max < facets[i].p3[2]) {
            z_max = facets[i].p3[2];
        }
    }

    double x_mid = 0.5 * (x_max + x_min);
    double y_mid = 0.5 * (y_max + y_min);
    double z_mid = 0.5 * (z_max + z_min);

    for (unsigned int i = 0; i < facets.size(); i++) {
        facets[i].p1[0] -= x_mid;
        facets[i].p2[0] -= x_mid;
        facets[i].p3[0] -= x_mid;

        facets[i].p1[1] -= y_mid;
        facets[i].p2[1] -= y_mid;
        facets[i].p3[1] -= y_mid;

        facets[i].p1[2] -= z_mid;
        facets[i].p2[2] -= z_mid;
        facets[i].p3[2] -= z_mid;
    }
}

/**
 * @brief Reads a binary STL file into the c_STL class.
 *
 * @param fname The file that will be loaded.
 */
void c_STL::LoadFileB(string fname) {

    filename = fname;
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == NULL) {
        printf("Could not find file <%s>.", filename.c_str());
        exit(0);
    }
    char header[80];
    fread(header, sizeof(char), 80, fp);
    printf("\n--------- STL FILE ----------");
    printf("\nHeader <%s>", header);
    int num_facets;
    fread(&num_facets, sizeof(int), 1, fp);
    printf("\nThere are %d facets", num_facets);

    facets.resize(num_facets);

    for (int i = 0; i < num_facets; i++) {
        float ddum[3] = {-999, -999, -999};
        fread(ddum, sizeof(float), 3, fp);  // Normal xyz
        facets[i].n[0] = ddum[0];
        facets[i].n[1] = ddum[1];
        facets[i].n[2] = ddum[2];
        fread(ddum, sizeof(float), 3, fp);  // Point 1
        facets[i].p1[0] = ddum[0];
        facets[i].p1[1] = ddum[1];
        facets[i].p1[2] = ddum[2];
        fread(ddum, sizeof(float), 3, fp);  // Point 2
        facets[i].p2[0] = ddum[0];
        facets[i].p2[1] = ddum[1];
        facets[i].p2[2] = ddum[2];
        fread(ddum, sizeof(float), 3, fp);  // Point 3
        facets[i].p3[0] = ddum[0];
        facets[i].p3[1] = ddum[1];
        facets[i].p3[2] = ddum[2];
        fread(ddum, sizeof(char), 2, fp);
    }
    fclose(fp);
    printf("\n--------------------------\n");
}

/**
 * @brief Writes an ASCII stl file for the current c_STL class.
 *
 * @param fname The desired file name.
 * @param solid_name The tag for this stl file.
 */
void c_STL::WriteFileA(string fname, string solid_name) {

    FILE *fp = fopen(fname.c_str(), "w");
    assert(fp != NULL);
    printf("\nWriting stl to <%s>", fname.c_str());
    fflush(stdout);

    fprintf(fp, "solid %s", solid_name.c_str());
    for (unsigned int i = 0; i < facets.size(); i++) {
        fprintf(fp, "\nfacet normal %lf %lf %lf", facets[i].n[0],
                facets[i].n[1], facets[i].n[2]);
        fprintf(fp, "\nouter loop");
        fprintf(fp, "\nvertex %lf %lf %lf", facets[i].p1[0], facets[i].p1[1],
                facets[i].p1[2]);
        fprintf(fp, "\nvertex %lf %lf %lf", facets[i].p2[0], facets[i].p2[1],
                facets[i].p2[2]);
        fprintf(fp, "\nvertex %lf %lf %lf", facets[i].p3[0], facets[i].p3[1],
                facets[i].p3[2]);
        fprintf(fp, "\nendloop\nendfacet");
    }
    fprintf(fp, "\nendsolid %s", solid_name.c_str());

    fclose(fp);
}

void c_STL::InitTree(double root_extent[6]) {

    double dx = root_extent[3] - root_extent[0];
    double dy = root_extent[4] - root_extent[1];
    double dz = root_extent[5] - root_extent[2];
    domain_size = sqrt(dx * dx + dy * dy + dz * dz);

    tree.init(root_extent);
    TreeIsInit = true;

    for (unsigned int i = 0; i < size(); i++) {
        double extent[6];
        facets[i].GetExtent(0, extent);

        tree.store(i, extent);
    }
}

void c_STL::GetExtentsInsideExtent(std::vector<int> &inside, double extent[6]) {

    tree.retrieve(&inside, extent);
}

Point c_STL::GetClosestPoint_Approx(Point &point, double &dist,
                                    double seed_distance) {
    dist = seed_distance;
    return GetClosestPoint(point, dist, STL_NEAREST_APPROX);
}

Point c_STL::GetClosestPoint_Exact(Point &point, double &dist,
                                   double seed_distance) {
    dist = seed_distance;
    return GetClosestPoint(point, dist, STL_NEAREST_EXACT);
}

Point c_STL::LoopClosest(Point &point, double &dist, int flag) {
    // Refactor this and below so we can check if a point returned is inside the
    // extent box.
    bool found = false;
    double extent[6];
    std::vector<int> inside;
    Point closest;
    bool done = false;
    int count = 0;
    while (!found) {
        extent[0] = point[0] - dist;
        extent[1] = point[1] - dist;
        extent[2] = point[2] - dist;
        extent[3] = point[0] + dist;
        extent[4] = point[1] + dist;
        extent[5] = point[2] + dist;
        GetExtentsInsideExtent(inside, extent);
        for (unsigned int i = 0; i < inside.size(); i++) {
            double dtemp;
            Point ptemp;
            if (flag == STL_NEAREST_EXACT) {
                ptemp = facets[inside[i]].GetClosestPoint_Exact(point, dtemp);
            } else {
                ptemp = facets[inside[i]].GetClosestPoint_Approx(point, dtemp);
            }
            if (dtemp < dist) {
                dist = dtemp;
                closest = ptemp;
                found = true;
            }
        }
        if (dist < 1.0e-13) {
            dist = 1.0e-13;
        }
        count++;
        if (count == 20) {
            fprintf(stderr,
                    "\n[WARNING]: It is taking a while to find the nearest "
                    "point on a geometry surface.");
            fprintf(stderr, "\n.......... Seed distance %e, Point: ", dist);
            point.Print();
        } else if (count == 50) {
            fprintf(stderr,
                    "\n[ERROR]: You are clearly stuck in a loop and can't find "
                    "the nearest point on the geometry.");
            exit(0);
        }
        inside.clear();
        if (!found) {
            dist *= 2.0;
        } else if (!done) {
            // If this is the first candidate node we've found.
            // Make sure there aren't any closer points that we just barely
            // missed.
            // Do this by making an extent box that includes things we would
            // have missed
            // in a spherical extent.
            dist *= 1.7;
            done = true;
            found = false;
        }
    }

    return closest;
}

Point c_STL::GetClosestPoint(Point &point, double &dist, int flag) {

    Point closest = LoopClosest(point, dist, flag);
    return closest;
}

void BuildRayExtentBox(Point &orig, Point dir, double extent[6]) {
    extent[0] = (dir[0] > 0) ? (orig[0]) : (orig[0] + dir[0]);
    extent[1] = (dir[1] > 0) ? (orig[1]) : (orig[1] + dir[1]);
    extent[2] = (dir[2] > 0) ? (orig[2]) : (orig[2] + dir[2]);

    extent[3] = (dir[0] < 0) ? (orig[0]) : (orig[0] + dir[0]);
    extent[4] = (dir[1] < 0) ? (orig[1]) : (orig[1] + dir[1]);
    extent[5] = (dir[2] < 0) ? (orig[2]) : (orig[2] + dir[2]);
}

bool c_STL::GetRayIntersectionPoint(Point orig, Point &dir,
                                    Point &intersection_point) {

    assert(ReadyForSearching());

    int nSteps = 15;
    dir.normalize();
    double ray_step = domain_size / (double)nSteps;
    dir *= ray_step;
    Point end = orig + dir;

    double extent[6];
    for (unsigned int step = 0; step < nSteps; step++) {
        BuildRayExtentBox(orig, dir, extent);
        std::vector<int> inside;
        GetExtentsInsideExtent(inside, extent);
        for (unsigned int i = 0; i < inside.size(); i++) {
            if (facets[inside[i]]
                    .WhereDoesEdgeIntersect(orig, end, intersection_point)) {
                return true;
            }
        }
        orig += dir;
        end += dir;
    }

    return false;
}

