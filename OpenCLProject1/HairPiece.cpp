#include "HairPiece.h"

HairPiece::HairPiece(size_t dimX, size_t dimY, size_t dimZ) {

	std::vector<Node *> startNodes;

    for (size_t x = 0; x < dimX; ++x) {
        std::vector<Node*> rowNodes;
        for (size_t y = 0; y < dimY; ++y) {
            Node *currentRowStartNode = new Node(x, y, 0.0, 0.0, true);
			
			
			startNodes.push_back(currentRowStartNode);


            rowNodes.push_back(currentRowStartNode);
            for (size_t z = 1; z < dimZ; ++z) {
                Node *currentNode = new Node(x,y,z);
                
                Link *currentLink = new Link(rowNodes.back(), currentNode);
                this->links.insert(currentLink);

                rowNodes.push_back(currentNode);
            }
        }
        hairs.push_back(rowNodes);
    }

	for (Node *current : startNodes) {
		for (Node *c : startNodes) {
			Link *newLink = new Link(current, c);
			links.insert(newLink);
		}
	}
}

HairPiece::~HairPiece() {
    for (size_t x = 0; x < hairs.size(); ++x) {
        std::vector<Node*> rowNodes = hairs[x];
        for (size_t y = 0; y < rowNodes.size(); ++y) {
            Node *currentNode = rowNodes.back();
            Link *currentLink = NULL;
            while ((currentLink = getOutgoingLinkFor(currentNode)) != NULL) {
                // ToDo: �ber Links h�ngeln
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

Link* HairPiece::getOutgoingLinkFor(Node *node) {
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

Node* HairPiece::getNextNodeFor(Node *node) {
    if (node == NULL) {
        return NULL;
    }

    Link *outGoingLink = getOutgoingLinkFor(node);
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
unsigned int HairPiece::getHairLength() {
    Node *startNode = hairs[0][0];
    unsigned int length = 1;
    Node *currentNode = startNode;
    while ((currentNode = getNextNodeFor(currentNode)) != NULL) {
        ++length;
    }
    return length;
}

cl_HairPiece HairPiece::toClData() {
    cl_HairPiece hairPieceCL;
    hairPieceCL.sizeX = hairs.size();
    hairPieceCL.sizeY = hairs[0].size();
    hairPieceCL.zizeZ = getHairLength();
    // ToDo: Copy Buffers to Device and get Addresses correctly

    return hairPieceCL;
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
