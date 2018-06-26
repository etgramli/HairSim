#pragma once

#include <unordered_set>
#include <map>
#include "Node.h"
#include "Link.h"
#include <time.h>


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
    std::vector<Node *> nodes;
    std::vector<Link*> links;
    size_t width, length;
    size_t hairlength;
	const float linkLength = 1.0f;
    unsigned int getHairLength() const;


public:
    HairPiece(size_t dimX = 5, size_t dimY = 5, size_t dimZ = 10);
    HairPiece(const cl_HairPiece hairPiece);
    ~HairPiece();

    HairPiece& operator=(const HairPiece& hairPiece2);

    Link* getOutgoingLinkFor(Node *node) const;
	Link* getIngoingLinkFor(Node *node) const;
    Node* getNextNodeFor(Node *node) const;
    std::vector<Link *> getLinks();

    size_t getIndexOfNode(Node *node) const;

    int getNumberOfNodes() const;
    Node* getNode(const int index) const;
    int getNumberOfLinks() const;
    Link* getLink(const int index) const;

    std::vector<float> getCoordinatesForGL() const;

    cl_HairPiece getClData() const;
    static cl_Link getClLinkForLink(Link const * const link, std::map<Node *, int> * const nodeAddressToId);
    static void cleanUpClData(cl_HairPiece hairPiece);

    bool test(HairPiece *other) const;
    bool test(cl_HairPiece hp) const;
};
