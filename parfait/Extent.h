#pragma once

#include "../MessagePasser/MessagePasserStream.h"
#include "Point.h"

namespace Parfait {
    template <typename T>
    class Extent {

    public:
        Point<T> lo, hi;

        Extent();
        Extent(const T extent[6]);
        Extent(const Point<T> &lo, const Point<T> &hi);
        T* data() {return lo.data();};
        const T* data() const {return lo.data();};

        bool contains(const Point<T> &p) const;
        bool contains(const Extent &box) const;
        bool approxEqual(const Extent &e) const;
        T getLength_X() const;
        T getLength_Y() const;
        T getLength_Z() const;
        Point<T> center() const;

        void resize(double scale);
        void resize(double scaleX, double scaleY, double scaleZ);

        void makeIsotropic();

        void Print() const;

        static Extent extentAroundPoint(const Point<T> &p, T tol);
        static double volume(const Extent &domain);
        friend Stream::Stream& operator<<(Stream::Stream &stream, const Extent<T>&extent){
          stream << Stream::Element((char*) extent.data(),6*sizeof(T));
            return stream;
        }
        friend Stream::Stream& operator>>(Stream::Stream &stream, Extent<T>&extent){
            Stream::Element element(6*sizeof(T));
            stream >> element;
            memcpy(extent.data(),element.data(),6*sizeof(T));
            return stream;
        }
    };




}

#include "Extent.hpp"