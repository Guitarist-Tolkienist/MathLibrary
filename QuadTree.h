/* Collisions:
 * Rectangle to rectangle
 * Quadtree
 * */

#ifndef PROGRAM_QUADTREE_H
#define PROGRAM_QUADTREE_H

#include "MATH.h"

namespace Collision {
    using Geometry_2D::SVector_2D;
    using Geometry_2D::CFigure;
    using Geometry_2D::CRectangle;

    // Physics Cookbook
    Geometry_2D::CRectangle FromMinMax(const SVector_2D& min, const SVector_2D& max);

    bool RectangleRectangle(const CRectangle& rect1,
                            const CRectangle& rect2);

    struct QuadTreeData {
//        void* object;
        CFigure* Object;
        Geometry_2D::CRectangle bounds;
        bool flag;
        inline QuadTreeData(CFigure* o, const Geometry_2D::CRectangle& b) :
                Object(o),
                bounds(b),
                flag(false) {}
    };

    class QuadTreeNode {
    public:
        std::vector<QuadTreeNode> children;
        std::vector<QuadTreeData*> contents;
        int currentDepth;
        static int maxDepth;
        static int maxObjectsPerNode;
        Geometry_2D::CRectangle nodeBounds;
    public:
        inline QuadTreeNode(const Geometry_2D::CRectangle& bounds):
                nodeBounds(bounds), currentDepth(0) {}
        bool IsLeaf() const;
        int NumObjects();
        void Insert(QuadTreeData& data);
        void Remove(QuadTreeData& data);
        void Update(QuadTreeData& data);
        void Shake();
        void Split();
        void Reset();
        std::vector<QuadTreeData*>Query(const Geometry_2D::CRectangle& area);
    };
    typedef QuadTreeNode QuadTree;


    // AABB
    bool AABB(const CRectangle& Rect1, const CRectangle& Rect2) {
        float x1 = Rect1.TopLeft.X;
        float y1 = Rect1.TopLeft.Y;
        float w1 = Rect1.GetSize().X;
        float h1 = Rect1.GetSize().Y;

        float x2 = Rect2.TopLeft.X;
        float y2 = Rect2.TopLeft.Y;
        float w2 = Rect2.GetSize().X;
        float h2 = Rect2.GetSize().Y;

        return x1 <= x2 + w2 && x1 + h1 >= x2 &&
               y1 <= y2 + h2 && y1 + h1 >= y2;
    }


}

#endif //PROGRAM_QUADTREE_H