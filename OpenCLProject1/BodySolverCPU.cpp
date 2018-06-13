#include "BodySolverCPU.h"



BodySolverCPU::BodySolverCPU()
{
    Vector *gravity = new Vector(0.0f,0.0f,-9.81);
    this->forces.push_back(gravity);
}

BodySolverCPU::BodySolverCPU(std::vector<Vector *> forces) {
    for (Vector *force : forces) {
        this->forces.push_back(force);
    }
}

BodySolverCPU::~BodySolverCPU() {
    for (Vector *force : forces) {
        delete force;
    }
}


float BodySolverCPU::getDeltaTime() const {
    return deltaTime;
}
void BodySolverCPU::setDeltaTime(float deltaTime) {
    this->deltaTime = deltaTime;
}

Vector BodySolverCPU::addAllForces() {
    Vector sum;
    for (Vector *current : forces) {
        sum += *current;
    }
    return sum;
}

/*
void btSoftBody::PSolve_Links(
  btSoftBody* psb,
  btScalar kst,
  btScalar ti)
{
  for(int i=0, ni = psb->m_links.size(); i < ni; ++i)
  {
    Link &l=psb->m_links[i];
    if(l.m_c0>0) {
      Node &a = *l.m_normal[0];
      Node &b = *l.m_normal [1];
      const btVector3 del = b.m_position - a.m_position;
      const btScalar len = del.length2();
      const btScalar k =
        ((l.m_c1 - len)/(l.m_c0 * (l.m_c1 + len)))*
        simulationConstant;
      a.m_x -= del*(k*a.m_inverseMass);
      b.m_x += del*(k*b.m_inverseMass);
    }
  }
}
*/
void BodySolverCPU::pSolve_Links() {
    // Loop through all links, because all links must have begin and end point
    for (Link *currentLink : this->hairPiece.getLinks()) {
        if (currentLink->getBegin() != NULL && currentLink->getEnd() != NULL) {
            Node *a = currentLink->getBegin();
            Node *b = currentLink->getEnd();
            // Add up all forces to begin and end nodes
            const Vector diff = a->minus(b);
            const float len = diff.length2();
            const float restLength = currentLink->getRestLength();

            const Vector compinedForces = addAllForces();
            const Vector springForce = currentLink->getSpringForce();
            const Vector forcesNodeA = compinedForces + springForce;
            const Vector forcesNodeB = compinedForces - springForce;
            // Move nodes
            a->move(forcesNodeA);
            b->move(forcesNodeB);
        }
    }
    
    /*
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
    */
}

void BodySolverCPU::pSolveSinlgeLink(Link *link) {
    // ToDo
}
