// Type definition must be the same as in kernel,
// so that it can be copied from host to device
typedef struct {
    float3 coordinates;
    float3 velocity;
    bool isConst;
    float mass;
} Node;

typedef struct {
    //IDs in the array of the cl_Nodes
    uint beginNodeId, endNodeId;

    float springConstant;
    float length;
} Link;

typedef struct {
    uint sizeX;
    uint sizeY;
    uint sizeZ;

    uint numLinks;   // Number of links in the array below
    Link *links;     // Array of links that can be copied directly to the device

    uint numNodes;   // Number of nodes in the array below
    Node *nodes;     // Array of nodes to be copied directly to the device
} HairPiece;

// Each launched kernel handles one link and therefore two nodes
__kernel void SolvePositionsFromLinksKernel(__global HairPiece *hairPiece,
                                            __global float3 *forces,
                                            float deltaSeconds) {
    unsigned int i = get_global_id(0);
    Link *currentLink = &hairPiece->links[i];

    uint a_id = currentLink->beginNodeId;
    uint b_id = currentLink->endNodeId;
    Node *a = &hairPiece->nodes[a_id];
    Node *b = &hairPiece->nodes[b_id];

    // add gravity
    float3 gravitationalAcceleration = float3(0.0f, 0.0f, -0.00981f);
    float3 forcesNodeA = gravitationalAcceleration * a->mass;
    float3 forcesNodeB = gravitationalAcceleration * b->mass;

    // add link force
    Link *pre = hairPiece->getIngoingLinkFor(currentLink->getBegin());
    if (pre != NULL) {
        float3 linkForce = currentLink->getLinkForce(pre);
        forcesNodeB -= linkForce;
    }

    //add spring force
    const Vector springForce = currentLink->getSpringForce(deltaSeconds);
    forcesNodeB += springForce;

    // add wind
    deltaTime += deltaSeconds;

    float3 windForce = float3(0.09f, -0.08f, 0.05f) * (sin(deltaTime * 0.02f) + 1.0f);

    forcesNodeA += windForce;
    forcesNodeB += windForce;

    // Move nodes
    if (!a->isConst) {
        a->move(forcesNodeA, deltaSeconds);
    }
    if (!b->isConst) {
        b->move(forcesNodeB, deltaSeconds);
    }
}

__kernel uint getIngoingLinkIndexFor(HairPiece *hp, uint nodeId) {
    uint index = 0;
    for (uint i = 0; i < hp->numLinks; ++i) {
        Link *link = &hp->links[i];
        if (link->endNodeId == nodeId) {
            return i;
        }
    }
    return -1;
}
