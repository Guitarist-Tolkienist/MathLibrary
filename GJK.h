#ifndef MATH_GJK_H
#define MATH_GJK_H

#include <vector>
#include "MATH.h"

using Geometry_2D::SVector_2D;

namespace Collision {
    // Minkowski addition or Minkowski sum
    std::vector<SVector_2D> MinkowskiSum(const std::vector<SVector_2D>& Set1,
                                         const std::vector<SVector_2D>& Set2);

    // Minkowski difference (or geometric difference)
    std::vector<SVector_2D> MinkowskiDiff(const std::vector<SVector_2D>& Set1,
                                          const std::vector<SVector_2D>& Set2);
}

#endif //MATH_GJK_H
