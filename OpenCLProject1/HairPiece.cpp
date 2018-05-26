#include "HairPiece.h"

HairPiece::HairPiece(size_t dimX, size_t dimY, size_t dimZ)
{
    for (size_t x = 0; x < dimX; ++x) {
        std::vector<Node*> rowNodes = hairs[x];
        for (size_t y = 0; y < dimY; ++y) {
            Node *currentRowStartNode = new Node(x, y, 0.0, 0.0, true);
            rowNodes.push_back(currentRowStartNode);
            for (size_t z = 1; z < dimZ; ++z) {
                Node *currentNode = new Node(x,y,z);
                
                Link *currentLink = new Link(rowNodes.back(), currentNode);
                this->links.insert(currentLink);

                rowNodes.push_back(currentNode);
            }
        }
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


HairPiece::~HairPiece()
{
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
