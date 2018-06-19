#pragma once

#include <unordered_set>
#include <map>
#include "Node.h"
#include "Link.h"

typedef struct {
    //cl_Node *begin, *end;
    cl_uint beginNodeId, endNodeId;

    cl_float springConstant;
    cl_float length;
    cl_float threshold;  // 5% of length
} cl_Link;

typedef struct {
    //cl_Node **grid;
    cl_uint sizeX;
    cl_uint sizeY;
    cl_uint sizeZ;

    cl_uint numLinks;
    cl_Link *links;
} cl_HairPiece;

// Container class for multiple hair strings.
class HairPiece {

    std::vector<std::vector<Node*>> hairs;
    std::unordered_set<Link*> links;

    unsigned int getHairLength();


    // Returns all nodes and mapping from Node address to index
    // nodeAddressesToIndex must be initialized!
    std::vector<cl_Node> getNodesAsVector(std::map<Node *, int> *nodeAddressesToIndex);

    // Returns a sinle cl_Link with data filled
    cl_Link getLinkClData(const Link * const link, std::map<Node *, int> nodeAddressToIndex);


public:
    HairPiece(size_t dimX = 5, size_t dimY = 5, size_t dimZ = 10);
    ~HairPiece();

    Link* getOutgoingLinkFor(Node *node);
    Node* getNextNodeFor(Node *node);
    std::vector<std::vector<Node*>> getStartNodes();
    std::unordered_set<Link *> getLinks();


    std::vector<float> getCoordinatesForGL();


    cl_HairPiece getClData();
};
