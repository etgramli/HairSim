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
    float3 forcesNodeA = gravitationalAcceleration * a->getMass();
    float3 forcesNodeB = gravitationalAcceleration * b->getMass();

    // add link force
    Link *pre = hairPiece->getIngoingLinkFor(currentLink->getBegin());
    if (pre != NULL) {
        const Vector linkForce = currentLink->getLinkForce(pre);
        forcesNodeB -= linkForce;
    }

    //add spring force
    const Vector springForce = currentLink->getSpringForce(deltaSeconds);
    forcesNodeB += springForce;

    // add wind
    deltaTime += deltaSeconds;

    const Vector windForce = Vector(0.09f, -0.08f, 0.05f) * (sin(deltaTime * 0.02f) + 1.0f);

    forcesNodeA += windForce;
    forcesNodeB += windForce;

    // Move nodes
    a->move(forcesNodeA, deltaSeconds);
    b->move(forcesNodeB, deltaSeconds);
}
