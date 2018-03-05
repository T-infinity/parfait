
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
#include <MessagePasser/MessagePasser.h>
#include "ExtentBuilder.h"

namespace Parfait {
  namespace ParallelExtent {

    template<typename T>
    Extent<T> getBoundingBox(MessagePasser mp, Extent<T> e) {
        int root = 0;
        std::vector<Extent<T>> extents;
        mp.Gather(e, extents, root);

        Extent<T> out;
        if (mp.Rank() == root)
            out = ExtentBuilder::getBoundingBox(extents);

        mp.Broadcast(out, root);
        return out;
    }

    template<typename T>
    Extent<T> getBoundingBox(MessagePasser mp,
                             std::vector<Extent<T>>& boxes) {
        auto out = ExtentBuilder::getBoundingBox(boxes);
        return getBoundingBox(mp, out);
    }
  }
}