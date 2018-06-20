#pragma once

#include <unordered_set>
#include <map>
#include "Node.h"
#include "Link.h"
#include <time.h>

typedef struct {
    //IDs in the array of the cl_Nodes
    cl_uint beginNodeId, endNodeId;

    cl_float springConstant;
    cl_float length;
    cl_int num;
    cl_float threshold;  // 5% of length
} cl_Link;

typedef struct {
    cl_uint sizeX;
    cl_uint sizeY;
    cl_uint sizeZ;

    cl_uint numLinks;   // Number of links in the array below
    cl_Link *links;     // Array of links that can be copied directly to the device

    cl_uint numNodes;   // Number of nodes in the array below
    cl_Node *nodes;     // Array of nodes to be copied directly to the device
} cl_HairPiece;

// Container class for multiple hair strings.
class HairPiece {

    std::unordered_set<Node *> nodes;
    std::unordered_set<Link*> links;
    const size_t width, length;
    const size_t hairlength;
	const float linkLength = 1.0f;
    unsigned int getHairLength() const;

    bool test(cl_HairPiece hp) const;

public:
    HairPiece(size_t dimX = 5, size_t dimY = 5, size_t dimZ = 10);
    HairPiece(cl_HairPiece hairPiece);
    ~HairPiece();

    Link* getOutgoingLinkFor(Node *node) const;
	Link* getIngoingLinkFor(Node *node) const;
    Node* getNextNodeFor(Node *node) const;
    std::vector<std::vector<Node*>> getStartNodes();
    std::unordered_set<Link *> getLinks();


    std::vector<float> getCoordinatesForGL();


    cl_HairPiece getClData() const;
    void cleanUpClData(cl_HairPiece hairPiece) const;
};
