#include "BodySolverCPU.h"



BodySolverCPU::BodySolverCPU()
{
}

void BodySolverCPU::pSolve_Links() {
    std::vector<std::vector<Node*>> startNodes = hairPiece.getStartNodes();
    for (size_t x = 0; x < startNodes.size(); ++x) {
        std::vector<Node *> row = startNodes[x];
        for (size_t y = 0; y = row.size(); ++y) {
            Node *startNode = row[y];
            Node *current = startNode;
            while (true) {
                // ToDo: Solve forces to nodes
            }
        }
    }
}

void BodySolverCPU::pSolveSinlgeLink(Link *link) {
    // ToDo
}
