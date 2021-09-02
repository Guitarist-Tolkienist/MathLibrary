/* Collisions:
 * Rectangle to rectangle
 * Quadtree
 * */

#include "QuadTree.h"
#include <queue>

using namespace Geometry_2D;

namespace Collision {
    // Physics Cookbook

    CRectangle FromMinMax(const SVector_2D& min, const SVector_2D& max) {
        return CRectangle(min, max);
    }

    // CHECKS IF TWO RECTS OVERLAP
    bool RectangleRectangle(const CRectangle& rect1,
                            const CRectangle& rect2) {
        // Find the min and max points of rectangle 1
        SVector_2D aMin = rect1.TopLeft;
        SVector_2D aMax = rect1.BottomRight;

        // Find the min and max points of rectangle 2:
        SVector_2D bMin = rect2.TopLeft;
        SVector_2D bMax = rect2.BottomRight;

        // Check for overlap on t X and Y axes separately
        bool overX = ((bMin.X <= aMax.X) && (aMin.X <= bMax.X));
        bool overY = ((bMin.Y <= aMax.Y) && (aMin.Y <= bMax.Y));

        return overX && overY;
    }


    int QuadTreeNode::maxDepth = 5;
    int QuadTreeNode::maxObjectsPerNode = 10;


    bool QuadTreeNode::IsLeaf() const{
        return children.size() == 0;
    }


    int QuadTreeNode::NumObjects() {
        Reset();
        int objectCount = contents.size();
        for (int i = 0, size = contents.size(); i< size; ++i) {
            contents[i]->flag = true;
        }
        std::queue<QuadTreeNode*> process;
        process.push(this);
        while (process.size() > 0) {
            QuadTreeNode* processing = process.back();
            if (!processing->IsLeaf()) {
                for (int i = 0, size = processing->children.size(); i < size; ++i) {
                    process.push(&processing->children[i]);
                }
            } else {
                for (int i = 0, size =
                        processing->contents.size(); i < size; ++i) {
                    if (!processing->contents[i]->flag) {
                        objectCount += 1;
                        processing->contents[i]->flag = true;
                    }
                }
            }
            process.pop();
        }
        Reset();
        return objectCount;
    }


    void QuadTreeNode::Insert(QuadTreeData& data) {
        if (!RectangleRectangle(data.bounds, nodeBounds)) {
            std::cout << "QuadTreeNode::Insert | RectangleRectangle() is false\n";
            return; // The object does not fit into this node
        }

        if (IsLeaf() && contents.size() + 1 > maxObjectsPerNode) {
            std::cout << "QuadTreeNode::Insert | Is Leaf and MAX number has been exceeded\n";
            Split(); // Try splitting!
        }
        if (IsLeaf()) {
            std::cout << "QuadTreeNode::Insert | Is Leaf\n";
            contents.push_back(&data);
        } else {
            for (int i = 0, size = children.size(); i < size; ++i) {
                children[i].Insert(data);
            }
        }
    }


    void QuadTreeNode::Remove(QuadTreeData& data) {
        if (IsLeaf()) {
            int removeIndex = -1;
            for (int i=0, size=contents.size(); i<size; ++i) {
                if (contents[i]->Object == data.Object) {
                    removeIndex = i;
                    break;
                }
            }
            if (removeIndex != -1) {
                contents.erase(contents.begin() + 1);
            }
        } else {
            for (int i=0, size=children.size(); i<size; ++i) {
                children[i].Remove(data);
            }
        }
        Shake();
    }


    void QuadTreeNode::Update(QuadTreeData& data) {
        Remove(data);
        Insert(data);
    }


    void QuadTreeNode::Reset() {
        if (IsLeaf()) {
            for (int i=0, size=contents.size(); i<size; ++i) {

                contents[i]->flag = false;
            }
        } else {

            for (int i=0, size=children.size(); i<size; ++i) {
                children[i].Reset();
            }
        }
    }


    void QuadTreeNode::Shake() {
        if (!IsLeaf()) {
            int numObjects = NumObjects();
            if (numObjects == 0) {
                children.clear();
            } else if (numObjects < maxObjectsPerNode) {
                std::queue<QuadTreeNode*> process;
                process.push(this);
                while (process.size() > 0) {
                    QuadTreeNode* processing = process.back();
                    if (!processing->IsLeaf()) {
                        for (int i = 0, size =
                                processing->children.size();
                             i < size; ++i) {
                            process.push(&processing->children[i]);
                        }
                    }
                    else {
                        contents.insert(contents.end(),
                                        processing->contents.begin(),
                                        processing->contents.end());
                    }
                    process.pop();
                }
                children.clear();
            }
        }
    }


    void QuadTreeNode::Split() {
        std::cout << "| SPLIT |\n";
        if (currentDepth + 1 >= maxDepth) {
            return;
        }

        SVector_2D min = nodeBounds.TopLeft;
        SVector_2D max = nodeBounds.BottomRight;
        SVector_2D center = min + ((max - min) * 0.5f);
        CRectangle childAreas[] = {
                CRectangle(
                        FromMinMax(
                                SVector_2D(min),
                                SVector_2D(center))),
                CRectangle(
                        FromMinMax(
                                SVector_2D(center.X, min.Y),
                                SVector_2D(max.X, center.Y))),
                CRectangle(
                        FromMinMax(
                                SVector_2D(center.X, center.Y),
                                SVector_2D(max.X, max.Y))),
                CRectangle(
                        FromMinMax(
                                SVector_2D(min.X, center.Y),
                                SVector_2D(center.X, max.Y))),
        };

        for (int i = 0; i < 4; ++i) {
            children.push_back(QuadTreeNode(childAreas[i]));
            children[i].currentDepth = currentDepth + 1;
        }

        for (int i = 0, size = contents.size(); i < size; ++i) {
            children[i].Insert(*contents[i]);
        }

        contents.clear();
    }


    std::vector<QuadTreeData*> QuadTreeNode::Query(const CRectangle& area) {
        std::vector<QuadTreeData*> result;
        if (!RectangleRectangle(area, nodeBounds)) {
            return result;
        }
        if (IsLeaf()) {
            for (int i=0, size=contents.size(); i<size; ++i) {
                if(RectangleRectangle(contents[i]->bounds,area)){
                    result.push_back(contents[i]);
                }
            }
        } else {
            for (int i=0, size=children.size(); i<size; ++i) {
                std::vector<QuadTreeData*> recurse = children[i].Query(area);
                if (recurse.size() > 0) {
                    result.insert(result.end(),
                                  recurse.begin(),
                                  recurse.end());
                }
            }
        }
        return result;
    }
}