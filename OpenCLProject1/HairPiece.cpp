#include <iostream>

#include "HairPiece.h"

HairPiece::HairPiece(size_t dimX, size_t dimY, size_t dimZ) {

	//std::vector<Node *> startNodes;

    for (size_t x = 0; x < dimX; ++x) {
        std::vector<Node*> rowNodes;
        for (size_t y = 0; y < dimY; ++y) {
            Node *currentRowStartNode = new Node(x, y, 0.0, 0.0, true);
			//startNodes.push_back(currentRowStartNode);
            rowNodes.push_back(currentRowStartNode);

            Node *previousNode = currentRowStartNode;
            for (size_t z = 1; z < dimZ; ++z) {
				Node *currentNode;
				if (z == dimZ - 1) {
					currentNode = new Node(x + 0.1f, y + 0.1f, z * 1.0f);////////////////+0.001f z * length
				} else if (z == 1) {
					currentNode = new Node(x , y, z * 1.0f, 0.0f, true);
                } else {
                    currentNode = new Node(x, y, z * 1.0f);
                }
                Link *currentLink = new Link(previousNode, currentNode, dimZ - z);
                this->links.insert(currentLink);
                previousNode = currentNode;
            }
        }
        hairs.push_back(rowNodes);
    }

	/*for (Node *current : startNodes) {
		for (Node *c : startNodes) {
			Link *newLink = new Link(current, c);
			links.insert(newLink);
		}
	}*/
}

HairPiece::~HairPiece() {
    for (size_t x = 0; x < hairs.size(); ++x) {
        std::vector<Node*> rowNodes = hairs[x];
        for (size_t y = 0; y < rowNodes.size(); ++y) {
            Node *currentNode = rowNodes.back();
            Link *currentLink = NULL;
            while ((currentLink = getOutgoingLinkFor(currentNode)) != NULL) {
                // ToDo: Über Links hängeln
                delete(currentNode);
                currentNode = currentLink->getEnd();
                links.erase(currentLink);
                delete(currentLink);
            }
        }
    }
    // Todo: Cleanup Nodes and Links
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

std::vector<std::vector<Node*>> HairPiece::getStartNodes() {
    return this->hairs;
}

std::unordered_set<Link *> HairPiece::getLinks() {
    return links;
}

// Length in Nodes
unsigned int HairPiece::getHairLength() const {
    Node *startNode = hairs[0][0];
    unsigned int length = 1;
    Node *currentNode = startNode;
    while ((currentNode = getNextNodeFor(currentNode)) != NULL) {
        ++length;
    }
    return length;
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

    cl_hairPiece.sizeX = hairs.size();
    cl_hairPiece.sizeY = hairs.at(0).size();
    cl_hairPiece.sizeZ = getHairLength();

    cl_hairPiece.numLinks = links.size();
    cl_hairPiece.links = new cl_Link[cl_hairPiece.numLinks];

    cl_hairPiece.numNodes = cl_hairPiece.sizeX * cl_hairPiece.sizeY * cl_hairPiece.sizeZ;
    cl_hairPiece.nodes = new cl_Node[cl_hairPiece.numNodes];

    // ToDo: Copy Buffers to Device and get Addresses correctly
    std::map<Node *, int> nodeToId; // Map to map node addresses to IDs in the array on device
    unsigned int nodeCounter = 0;
    for (std::vector<Node *> row : hairs) {
        for (Node *currentNode : row) {
            // Copy cl data of node to cl_hairPiece
            cl_hairPiece.nodes[nodeCounter] = currentNode->getClData();
            // Add mapping
            nodeToId.emplace(currentNode, nodeCounter);
            ++nodeCounter;

            Link *currentLink = NULL;
            while ((currentLink = getOutgoingLinkFor(currentNode)) != NULL) {
                currentNode = currentLink->getEnd();

                // Copy cl data of node to cl_hairPiece
                cl_hairPiece.nodes[nodeCounter] = currentNode->getClData();
                // Add mapping
                nodeToId.emplace(currentNode, nodeCounter);
                ++nodeCounter;
            }
        }
    }
    std::cout << "Created " << nodeCounter << " nodes for copying to CL device!" << std::endl;
    std::cout << "Originally " << cl_hairPiece.numNodes << " nodes exist!" << std::endl << std::endl;


    std::map<Link *, int> linkToId; // Map to map link addresses to ID
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
