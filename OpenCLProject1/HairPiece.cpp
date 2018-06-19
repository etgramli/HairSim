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
				Node *currentNode = new Node(x, y, z * 1.0f);
				if (z == dimZ - 1) {
					currentNode = new Node(x + 0.1f, y + 0.1f, z * 1.0f);////////////////+0.001f z * length
				} else if (z == 1) {
					currentNode = new Node(x , y, z * 1.0f, 0.0f, true);
				}
                Link *currentLink = new Link(rowNodes.back(), currentNode, dimZ - z);
                this->links.insert(currentLink);

                rowNodes.push_back(currentNode);
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





std::vector<cl_Node> HairPiece::getNodesAsVector(std::map<Node *, int> *nodeAddressesToIndex) {
    std::vector<cl_Node> clNodes;

    for (size_t x = 0; x < hairs.size(); ++x) {
        std::vector<Node*> rowNodes = hairs[x];
        for (size_t y = 0; y < rowNodes.size(); ++y) {
            Node *currentNode = rowNodes[y];
            clNodes.push_back(currentNode->getClData());
            nodeAddressesToIndex->insert(std::pair<Node *, int>(currentNode, clNodes.size()));
        }
    }
    return clNodes;
}

cl_Link HairPiece::getLinkClData(const Link * const link, std::map<Node *, int> nodeAddressToId) {
    cl_Link cl_link;

    cl_link.length = link->getLength();
    cl_link.springConstant = link->getSpringConstant();
    cl_link.threshold = link->getTreshold();

    cl_link.beginNodeId = nodeAddressToId[link->getBegin()];
    cl_link.endNodeId = nodeAddressToId[link->getEnd()];

    return cl_link;
}


cl_HairPiece HairPiece::getClData() {
    cl_HairPiece cl_hairPiece;

    cl_hairPiece.sizeX = hairs.size();
    cl_hairPiece.sizeY = hairs.at(0).size();
    cl_hairPiece.sizeZ = getHairLength();

    cl_hairPiece.numLinks = links.size();

    // ToDo: Copy Buffers to Device and get Addresses correctly

    return cl_hairPiece;
}
