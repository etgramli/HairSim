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

    std::vector<Node *> nodes;
    std::vector<Link*> links;
    size_t width, length;
    size_t hairlength;
	const float linkLength = 1.0f;
    unsigned int getHairLength() const;


public:
    HairPiece(size_t dimX = 5, size_t dimY = 5, size_t dimZ = 10);
    HairPiece(cl_HairPiece hairPiece);
    ~HairPiece();

    HairPiece& operator=(const HairPiece& hairPiece2) {
        if (this == &hairPiece2) {
            return *this;
        }
        if (links.size() > 0) {
            for (Link *current : links) {
                delete current;
            }
            links.clear();
        }
        if (nodes.size() > 0) {
            for (Node *current : nodes) {
                delete current;
            }
            nodes.clear();
        }

        for (Node *node : hairPiece2.nodes) {
            nodes.push_back(new Node(*node));
        }
        for (Link *link : hairPiece2.links) {
            links.push_back(new Link(*link));
        }
        return *this;
    }

    Link* getOutgoingLinkFor(Node *node) const;
	Link* getIngoingLinkFor(Node *node) const;
    Node* getNextNodeFor(Node *node) const;
    std::vector<Link *> getLinks();

    size_t getIndexOfNode(Node *node) const {
        ptrdiff_t pos = find(nodes.begin(), nodes.end(), node) - nodes.begin();
        if (pos >= nodes.size()) {
            return -1;
        } else {
            return pos;
        }
    }

    int getNumberOfNodes() const {
        return nodes.size();
    }
    Node* getNode(const int index) const {
        return nodes[index];
    }
    int getNumberOfLinks() const {
        return links.size();
    }
    Link* getLink(const int index) const {
        return links[index];
    }

    std::vector<float> getCoordinatesForGL();


    cl_HairPiece getClData() const;
    static cl_Link getClLinkForLink(Link const * const link, std::map<Node *, int> * const nodeAddressToId);
    static void cleanUpClData(cl_HairPiece hairPiece);

    bool test(HairPiece *other) const;
    bool test(cl_HairPiece hp) const;
};
