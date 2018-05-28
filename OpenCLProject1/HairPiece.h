#pragma once

#include <unordered_set>
#include "Node.h"
#include "Link.h"


class HairPiece
{
public:
    HairPiece(size_t dimX = 5, size_t dimY = 5, size_t dimZ = 5);
    ~HairPiece();

    Link *getOutgoingLinkFor(Node *node);

    // toClData();
private:
    std::vector<std::vector<Node*>> hairs;
    std::unordered_set<Link*> links;
};
