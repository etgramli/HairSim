#include <iostream>

#include "HairPiece.h"

HairPiece::HairPiece(size_t dimX, size_t dimY, size_t dimZ)
    : width(dimX), length(dimY), hairlength(dimZ) {

	srand((unsigned)time(NULL));
    for (size_t x = 0; x < dimX; ++x) {
        for (size_t y = 0; y < dimY; ++y) {
            Node *currentRowStartNode = new Node(x, y, 0.0, 0.0f, true);
            nodes.push_back(currentRowStartNode);

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
                nodes.push_back(currentNode);
                Link *currentLink = new Link(previousNode, currentNode, linkLength + r);
                links.push_back(currentLink);
                previousNode = currentNode;
            }
        }
    }
}

HairPiece::HairPiece(const cl_HairPiece hairPiece) {
    this->width = hairPiece.sizeX;
    this->length = hairPiece.sizeY;
    this->hairlength = hairPiece.sizeZ;
    //std::cout << "Importing Hair Piece from CL types (" << width << ", " << length << ", " << hairlength << ")" << std::endl;

    std::map<int, Node *> indexToNode;
    for (unsigned int i = 0; i < hairPiece.numNodes; ++i) {
        Node *currentNode = new Node(hairPiece.nodes[i]);

        nodes.push_back(currentNode);
        indexToNode[i] = currentNode;
    }
    if (nodes.size() != hairPiece.numNodes) {
        std::cout << "Importing failed: " << nodes.size() << " from " << hairPiece.numNodes << " nodes imported!" << std::endl;
    }
    

    for (unsigned int i = 0; i < hairPiece.numLinks; ++i) {
        cl_Link currentClLink = hairPiece.links[i];
        links.push_back(new Link(
            indexToNode[currentClLink.beginNodeId],
            indexToNode[currentClLink.endNodeId],
            currentClLink.length,
            currentClLink.springConstant));
    }
    if(links.size() != hairPiece.numLinks) {
        std::cout << "Imported failed: " << links.size() << " from " << hairPiece.numLinks << " links!" << std::endl << std::endl;
    }
}


HairPiece::~HairPiece() {
    for (Node *currentNode : nodes) {
        delete(currentNode);
    }
    nodes.clear();
    
    for (Link *current : links) {
        delete(current);
    }
    links.clear();
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

std::vector<Link *> HairPiece::getLinks() {
    return links;
}

size_t HairPiece::getIndexOfNode(Node *node) const {
    ptrdiff_t pos = find(nodes.begin(), nodes.end(), node) - nodes.begin();
    if (pos >= nodes.size()) {
        return -1;
    } else {
        return pos;
    }
}

int HairPiece::getNumberOfNodes() const {
    return nodes.size();
}

Node* HairPiece::getNode(const int index) const {
    return nodes[index];
}

int HairPiece::getNumberOfLinks() const {
    return links.size();
}

Link* HairPiece::getLink(const int index) const {
    return links[index];
}

// Length in Nodes
unsigned int HairPiece::getHairLength() const {
    return hairlength;
}

std::vector<float> HairPiece::getCoordinatesForGL() const {
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

    std::map<Node *, int> nodeToId; // Map to map node addresses to IDs in the array on device
    unsigned int nodeCounter = -1;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        Node *currentNode = nodes[i];
        // Copy cl data of node to cl_hairPiece
        cl_hairPiece.nodes[i] = currentNode->getClData();
        
        nodeToId[currentNode] = i; // Add mapping
        nodeCounter = i + 1;
    }
    //std::cout << "Created " << nodeCounter << " nodes for copying to CL device!" << std::endl;
    //std::cout << "Originally " << cl_hairPiece.numNodes << " nodes exist!" << std::endl << std::endl;

    unsigned int linkCounter = 0;
    for (Link *currentLink : links) {
        cl_hairPiece.links[linkCounter++] = HairPiece::getClLinkForLink(currentLink, &nodeToId);
    }
    //std::cout << "Created " << linkCounter << " links for copying to CL device!" << std::endl;
    //std::cout << "Originally " << cl_hairPiece.numLinks << " links exist!" << std::endl << std::endl;

    return cl_hairPiece;
}

cl_Link HairPiece::getClLinkForLink(Link const * const link, std::map<Node *, int> * const nodeAddressToId) {
    cl_Link clLink;
    clLink.beginNodeId    = (*nodeAddressToId)[link->getBegin()];
    clLink.endNodeId      = (*nodeAddressToId)[link->getEnd()];
    clLink.length         = link->getLength();
    clLink.springConstant = link->getSpringConstant();
    return clLink;
}

void HairPiece::cleanUpClData(cl_HairPiece hairPiece) {
    delete[] hairPiece.links;
    delete[] hairPiece.nodes;
}



bool HairPiece::test(HairPiece *other) const {
    if (other->getNumberOfNodes() != this->getNumberOfNodes()) {
        std::cout << std::endl
            << "Number of nodes are not Equal! (" << nodes.size()
            << " v s" << other->getNumberOfNodes()
            << std::endl << std::endl;
        return false;
    }
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        Node *myCurrentNode = nodes[i];
        Node *otherCurrentNode = other->getNode(i);
        if (otherCurrentNode->getX() != myCurrentNode->getX() ||
            otherCurrentNode->getY() != myCurrentNode->getY() ||
            otherCurrentNode->getZ() != myCurrentNode->getZ()) {
            std::cout << "Nodes are DIFFERENT at index: " << i << std::endl;
            std::cout << "Node1(" << myCurrentNode->getX() << ", " << myCurrentNode->getY()<<", " << myCurrentNode->getZ() << ")" << std::endl;
            std::cout << "Node2(" << otherCurrentNode->getX() << ", " << otherCurrentNode->getY()<<", " << otherCurrentNode->getZ() << ")" << std::endl;
            return false;
        }
    }

    std::cout << "All nodes ar EQUAL" << std::endl;

    
    if (other->getNumberOfLinks() != this->getNumberOfLinks()) {
        std::cout << std::endl
            << "Number of links are not Equal! (" << this->getNumberOfLinks()
            << " v s" << other->getNumberOfLinks()
            << std::endl << std::endl;
        return false;
    }
    for (unsigned int i = 0; i < links.size(); ++i) {
        Link *myLink = links[i];
        Link *clLink = other->getLink(i);

        size_t beginNodeIdx = getIndexOfNode(myLink->getBegin());
        size_t endNodeIdx = getIndexOfNode(myLink->getEnd());
        size_t beginClNodeIdx = other->getIndexOfNode(clLink->getBegin());
        size_t endClNodeIdx = other->getIndexOfNode(clLink->getEnd());

        if (myLink->getLength() != clLink->getLength() ||
            myLink->getSpringConstant() != clLink->getSpringConstant() ||
            beginNodeIdx != beginClNodeIdx ||
            endNodeIdx != endClNodeIdx) {
            std::cout << "Links are DIFFERENT at index: " << i << std::endl;
            return false;
        }
    }

    return true;
}


bool HairPiece::test(cl_HairPiece hp) const {
    return test(&HairPiece(hp));
}


HairPiece& HairPiece::operator=(const HairPiece& hairPiece2) {
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