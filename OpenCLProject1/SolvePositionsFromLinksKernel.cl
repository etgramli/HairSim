// Type definition must be the same as in kernel,
// so that it can be copied from host to device
typedef struct __attribute__((packed)) {
	float3 coordinates;
	float3 velocity;
	int isConst;
	float mass;
} Node;

typedef struct {
	// indices in the array of the cl_Nodes
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


uint getIngoingLinkIndexFor(uint numLinks, __global Link *links, uint nodeId);
void move(__global Node *node, float3 force, float deltaSeconds);

float3 getSpringForce(__global Node *nodes, __global Link *link);
float3 getLinkForce(__global Node *nodes, __global Link *thisLink, __global Link *preLink);

// Each launched kernel handles one link and therefore two nodes
__kernel void solvePositionsFromLinksKernel(__global HairPiece *hairPiece,
	__global Node *nodes,
	__global Link *links,
	__global float *deltaTime,
	__global float *deltaSeconds) {
	unsigned int i = get_global_id(0);

	uint a_id = links[i].beginNodeId;
	uint b_id = links[i].endNodeId;
	__global Node *a = &nodes[a_id];
	__global Node *b = &nodes[b_id];

	// add gravity
	const float3 gravitationalAcceleration = (float3)(0.0f, 0.0f, -0.00981f);
	float3 forcesNodeA = gravitationalAcceleration * a->mass;
	float3 forcesNodeB = gravitationalAcceleration * b->mass;

	// add link force
	uint linkIdx = getIngoingLinkIndexFor(hairPiece->numLinks, links, links[i].beginNodeId);
	if (linkIdx > -1) {
		float3 linkForce = getLinkForce(nodes, &links[i], &links[linkIdx]);
		forcesNodeB -= linkForce;
	}

	//add spring force
	forcesNodeB += getSpringForce(nodes, &links[i]);

	// add wind
	(*deltaTime) += (*deltaSeconds);

	float3 windForce = (float3)(0.09f, -0.08f, 0.05f) * (sin(*deltaTime * 0.02f) + 1.0f);

	forcesNodeA += windForce;
	forcesNodeB += windForce;

	// Move nodes
	//move(a, forcesNodeA, *deltaSeconds);
	move(b, forcesNodeB, *deltaSeconds);
}

uint getIngoingLinkIndexFor(uint numLinks, __global Link *links, uint nodeId) {
	for (uint i = 0; i < numLinks; ++i) {
		if (links[i].endNodeId == nodeId) {
			return i;
		}
	}
	return -1;
}


void move(__global Node *node, float3 force, float deltaSeconds) {
	if (node->isConst == true) return;
	float3 v = force * (deltaSeconds / node->mass) + (node->velocity * 0.9f);
	node->coordinates += v;
	node->velocity = v;

	if (node->coordinates.z <= 0) {
		node->coordinates.z = 0;
		if (node->velocity.z <= 0) {
			node->velocity.z = 0;
		}
	}
}


float3 getSpringForce(__global Node *nodes, __global Link *link) {
	float3 diff = nodes[link->endNodeId].coordinates
		- nodes[link->beginNodeId].coordinates;
	float diffLength = length(diff);
	normalize(diff);
	const float s = link->length - diffLength;

	return diff * (link->springConstant * s);
};

float3 getLinkForce(__global Node *nodes, __global Link *thisLink, __global Link *preLink) {
	float3 a = nodes[preLink->beginNodeId].coordinates
		- nodes[preLink->endNodeId].coordinates;
	float3 b = nodes[thisLink->endNodeId].coordinates
		- nodes[thisLink->beginNodeId].coordinates;
	float3 diff = a + b;
	float diffLength = length(diff);
	normalize(diff);
	float angle = acos(dot(a, b) / (length(a) * length(b))) * 180.0f / M_PI;
	if (angle < 178) {
		return diff * (0.3f * diffLength);
	}
	else {
		return (float3)(0.0f, 0.0f, 0.0f);
	}
};
