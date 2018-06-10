#pragma once

#include <unordered_set>
#include "Node.h"
#include "Link.h"


typedef struct {
    cl_Node *begin;
    cl_Node *end;
} cl_Link;


typedef struct {
    cl_Node **grid;
    cl_uint sizeX;
    cl_uint sizeY;
    cl_uint zizeZ;
    cl_Link *links;
} cl_HairPiece;

class HairPiece
{
public:
    HairPiece(size_t dimX = 5, size_t dimY = 5, size_t dimZ = 5);
    ~HairPiece();

    Link* getOutgoingLinkFor(Node *node);
    Node* getNextNodeFor(Node *node);
    std::vector<std::vector<Node*>> getStartNodes();
    std::unordered_set<Link *> getLinks() {
        return links;
    }

    cl_HairPiece toClData();
private:
    std::vector<std::vector<Node*>> hairs;
    std::unordered_set<Link*> links;

    unsigned int getHairLength();
};
