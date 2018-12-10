#pragma once
#include <vector>
#include <string>
#include "Extent.h"

namespace Parfait {

    void visualizeExtents(std::string filename, const std::vector<Parfait::Extent<double>>& extents);
    void visualizeExtents(std::string filename, const std::vector<Parfait::Extent<double>>& extents, const std::vector<double>& field);

}

#include "ExtentWriter.hpp"