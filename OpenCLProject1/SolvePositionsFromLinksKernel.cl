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


uint getIngoingLinkIndexFor(__global HairPiece *hp, uint nodeId);
void move(Node *node, float3 force, float deltaSeconds);

float3 getSpringForce(__global HairPiece *hp, Link *link);
float3 getLinkForce(__global HairPiece *hp, Link *thisLink, Link *preLink);

// Each launched kernel handles one link and therefore two nodes
__kernel void solvePositionsFromLinksKernel(__global HairPiece *hairPiece,
                                            __global float3 *forces,
                                            __global float *deltaTime,
                                            float deltaSeconds) {
    unsigned int i = get_global_id(0);
    Link *currentLink = &hairPiece->links[i];

    uint a_id = currentLink->beginNodeId;
    uint b_id = currentLink->endNodeId;
    Node *a = &hairPiece->nodes[a_id];
    Node *b = &hairPiece->nodes[b_id];

    // add gravity
    float3 gravitationalAcceleration = (float3)(0.0f, 0.0f, -0.00981f);
    float3 forcesNodeA = gravitationalAcceleration * a->mass;
    float3 forcesNodeB = gravitationalAcceleration * b->mass;

    // add link force
    uint linkIdx = getIngoingLinkIndexFor(hairPiece, currentLink->beginNodeId);
    if (linkIdx > -1) {
        Link *pre = &hairPiece->links[linkIdx];
        float3 linkForce = getLinkForce(hairPiece, currentLink, pre);
        forcesNodeB -= linkForce;
    }

    //add spring force
    const float3 springForce = getSpringForce(hairPiece, currentLink);
    forcesNodeB += springForce;

    // add wind
    (*deltaTime) += deltaSeconds;

    float3 windForce = (float3)(0.09f, -0.08f, 0.05f) * (sin(*deltaTime * 0.02f) + 1.0f);

    forcesNodeA += windForce;
    forcesNodeB += windForce;

    // Move nodes
    move(a, forcesNodeA, deltaSeconds);
    move(b, forcesNodeB, deltaSeconds);
}

uint getIngoingLinkIndexFor(__global HairPiece *hp, uint nodeId) {
    uint index = 0;
    for (uint i = 0; i < hp->numLinks; ++i) {
        Link *link = &hp->links[i];
        if (link->endNodeId == nodeId) {
            return i;
        }
    }
    return -1;
}


void move(Node *node, float3 force, float deltaSeconds) {
    if (node->isConst) {
        return;
    }
    float3 v = force * (deltaSeconds / node->mass) + (node->velocity * 0.9f);// damping factor
    node->coordinates += v;
    node->velocity = v;

    if (node->coordinates.z <= 0) {
        node->coordinates.z = 0;
        if (node->velocity.z <= 0) {
            node->velocity.z = 0;
        }
    }
}


float3 getSpringForce(__global HairPiece *hp, Link *link) {
    float3 diff =
        hp->nodes[link->endNodeId].coordinates
        - hp->nodes[link->beginNodeId].coordinates;
    float diffLength = length(diff);
    normalize(diff);
    float s = link->length - diffLength;

    return diff * (link->springConstant * s);
};

float3 getLinkForce(__global HairPiece *hp, Link *thisLink, Link *preLink) {
    float3 a = 
        hp->nodes[preLink->beginNodeId].coordinates
        - hp->nodes[preLink->endNodeId].coordinates;
    float3 b = 
        hp->nodes[thisLink->endNodeId].coordinates
        - hp->nodes[thisLink->beginNodeId].coordinates;
    float3 diff = a + b;
    float diffLength = length(diff);
    normalize(diff);
    float angle = acos(dot(a, b) / (length(a) * length(b))) * 180.0f / M_PI;
    if (angle < 178) {
        return diff * (0.3f * diffLength);
    } else {
        return (float3)(0.0f, 0.0f, 0.0f);
    }
};
