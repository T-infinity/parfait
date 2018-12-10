
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#pragma once

#include <MessagePasser/MessagePasserStream.h>
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
        std::array<Point<T>, 8> corners() const;

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