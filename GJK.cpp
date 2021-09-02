
#include "GJK.h"

std::vector<SVector_2D> Collision::MinkowskiSum(const std::vector<SVector_2D>& Set1,
                                                const std::vector<SVector_2D>& Set2) {
    std::vector<SVector_2D> SumResult;

    for (auto PointOfSet1 : Set1) {
        for (auto PointOfSet2: Set2) {
            SVector_2D Sum = PointOfSet1 + PointOfSet2;
            SumResult.push_back(Sum);
        }
    }

    return SumResult;
}

std::vector<SVector_2D> Collision::MinkowskiDiff(const std::vector<SVector_2D>& Set1,
                                                 const std::vector<SVector_2D>& Set2) {
    std::vector<SVector_2D> SumResult;

    for (auto PointOfSet1 : Set1) {
        for (auto PointOfSet2: Set2) {
            SVector_2D Sum = PointOfSet1 - PointOfSet2;
            SumResult.push_back(Sum);
        }
    }

    return SumResult;
}