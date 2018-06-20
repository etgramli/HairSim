#include <iostream>

#include "HairPiece.h"

HairPiece::HairPiece(size_t dimX, size_t dimY, size_t dimZ)
    : width(dimX), length(dimY), hairlength(dimZ) {

	srand((unsigned)time(NULL));
    for (size_t x = 0; x < dimX; ++x) {
        for (size_t y = 0; y < dimY; ++y) {
            Node *currentRowStartNode = new Node(x, y, 0.0, 0.0f, true);
            nodes.insert(currentRowStartNode);

            Node *previousNode = currentRowStartNode;
            for (size_t z = 1; z < dimZ; ++z) {
				float r = 0.4f * ((float)rand() / RAND_MAX - 0.5f);

				Node *currentNode;
				if (z == dimZ - 1) {
					currentNode = new Node(x, y, z * linkLength, 1.0f + r);
				} else if (z == 1) {
					currentNode = new Node(x, y, z * linkLength, 1.0f + r, true);
				}
				else {
					currentNode = new Node(x, y, z * linkLength, 1.0f + r);
                }
                nodes.insert(currentNode);
                Link *currentLink = new Link(previousNode, currentNode, dimZ - z, linkLength + r);
                this->links.insert(currentLink);
                previousNode = currentNode;
            }
        }
    }
}

HairPiece::HairPiece(cl_HairPiece hairPiece): width(hairPiece.sizeX), length(hairPiece.sizeY), hairlength(hairPiece.sizeZ) {
    std::map<int, Node *> indexToNode;
    for (int i = 0; i < hairPiece.numNodes; ++i) {
        Node *currentNode = new Node(hairPiece.nodes[i]);
        nodes.insert(currentNode);
        indexToNode.emplace(i, currentNode);
    }

    for (int i = 0; i < hairPiece.numLinks; ++i) {
        cl_Link currentClLink = hairPiece.links[i];
        Link *currentLink = new Link(
            indexToNode[currentClLink.beginNodeId],
            indexToNode[currentClLink.endNodeId],
            currentClLink.num,
            currentClLink.length,
            currentClLink.springConstant);
        links.insert(currentLink);
    }
}


HairPiece::~HairPiece() {
    for (Node *currentNode : nodes) {
        delete(currentNode);
    }
    
    for (Link *current : links) {
        delete(current);
    }
}

Link* HairPiece::getOutgoingLinkFor(Node *node) const {
    if (node == NULL) {
        return NULL;
    }
    for (Link *current : links) {
        if (current->getBegin() == node) {
            return current;
        }
    }
    return NULL;
}

Link* HairPiece::getIngoingLinkFor(Node *node) const {
	if (node == NULL) {
		return NULL;
	}
	for (Link *current : links) {
		if (current->getEnd() == node) {
			return current;
		}
	}
	return NULL;
}

Node* HairPiece::getNextNodeFor(Node *node) const {
    if (node == NULL) {
        return NULL;
    }

    Link const * const outGoingLink = getOutgoingLinkFor(node);
    if (outGoingLink == NULL) {
        return NULL;
    } else {
        return outGoingLink->getEnd();
    }
}

std::unordered_set<Link *> HairPiece::getLinks() {
    return links;
}

// Length in Nodes
unsigned int HairPiece::getHairLength() const {
    return hairlength;
}

std::vector<float> HairPiece::getCoordinatesForGL() {
    std::vector<float> out;

    for (Link *currentLink : links) {
        Node *a = currentLink->getBegin();
        out.push_back(a->getX());
        out.push_back(a->getZ());
        out.push_back(a->getY());
        Node *b = currentLink->getEnd();
        out.push_back(b->getX());
        out.push_back(b->getZ());
        out.push_back(b->getY());
    }
    return out;
}




cl_HairPiece HairPiece::getClData() const {
    cl_HairPiece cl_hairPiece;

    cl_hairPiece.sizeX = width;
    cl_hairPiece.sizeY = length;
    cl_hairPiece.sizeZ = getHairLength();

    cl_hairPiece.numLinks = links.size();
    cl_hairPiece.links = new cl_Link[cl_hairPiece.numLinks];

    cl_hairPiece.numNodes = cl_hairPiece.sizeX * cl_hairPiece.sizeY * cl_hairPiece.sizeZ;
    cl_hairPiece.nodes = new cl_Node[cl_hairPiece.numNodes];

    // ToDo: Copy Buffers to Device and get Addresses correctly
    std::map<Node *, int> nodeToId; // Map to map node addresses to IDs in the array on device
    unsigned int nodeCounter = 0;
    for (Node *currentNode : nodes) {
        // Copy cl data of node to cl_hairPiece
        cl_hairPiece.nodes[nodeCounter] = currentNode->getClData();
        
        nodeToId.emplace(currentNode, nodeCounter); // Add mapping
        ++nodeCounter;
    }
    std::cout << "Created " << nodeCounter << " nodes for copying to CL device!" << std::endl;
    std::cout << "Originally " << cl_hairPiece.numNodes << " nodes exist!" << std::endl << std::endl;

    unsigned int linkCounter = 0;
    for (Link *currentLink : links) {
        cl_hairPiece.links[linkCounter] = cl_Link();
        cl_hairPiece.links[linkCounter].beginNodeId = nodeToId[currentLink->getBegin()];
        cl_hairPiece.links[linkCounter].endNodeId = nodeToId[currentLink->getEnd()];
        cl_hairPiece.links[linkCounter].length = currentLink->getLength();
        cl_hairPiece.links[linkCounter].num = currentLink->getNum();
        cl_hairPiece.links[linkCounter].springConstant = currentLink->getSpringConstant();
        cl_hairPiece.links[linkCounter].threshold = currentLink->getTreshold();

        ++linkCounter;
    }
    std::cout << "Created " << linkCounter << " links for copying to CL device!" << std::endl;
    std::cout << "Originally " << cl_hairPiece.numLinks << " links exist!" << std::endl << std::endl;


    return cl_hairPiece;
}

void HairPiece::cleanUpClData(cl_HairPiece hairPiece) const {
    delete[] hairPiece.links;
    delete[] hairPiece.nodes;
}

bool HairPiece::test(cl_HairPiece hp) const {
    // ToDo
    return false;
}
