#include <algorithm>
#include "RayTracer.hpp"

void BVH::FindBiggestAABB(VObjects Objects)
{
    for (size_t i = 0; i != Objects.size(); i++) {
    }
}

#define OBJECT_LEAF 1 /*Provisoire*/

void BVH::CentroidSort(bvh_stack_t &stack, int axis)
{
    std::sort(IndexTab.begin() + stack.start, IndexTab.begin() + stack.end,
        [&](int a, int b) {
            const Vec3 &ca = Objects[a]->getCentroid();
            const Vec3 &cb = Objects[b]->getCentroid();

            if (axis == X)
                return ca.x < cb.x;
            if (axis == Y)
                return ca.y < cb.y;
            if (axis == Z)
                return ca.z < cb.z;
            return false;
        }
    );
}

/*Fast methode to link two AABB*/
AABB BVH::Union(AABB a, AABB b)
{
    AABB r;
    r.min = GetMin(a.min, b.min);
    r.max = GetMax(a.max, b.max);
    r.normalize();
    return r;
}


/*Update the left side and the right side of each element for estimate the cost in the SAH*/
void BVH::buildleftrightAABB(bvh_stack_t &stack)
{
    int count = stack.end - stack.start;

    LeftSide.resize(count);
    RightSide.resize(count);
    LeftSide[0] = Objects[IndexTab[stack.start]]->aabb;
    for (int i = 1; i < count; i++) {
        LeftSide[i] = Union(
            LeftSide[i - 1],
            Objects[IndexTab[stack.start + i]]->aabb
        );
    }
    RightSide[count - 1] = Objects[IndexTab[stack.end - 1]]->aabb;

    for (int i = count - 2; i >= 0; i--) {
        RightSide[i] = Union(
            RightSide[i + 1],
            Objects[IndexTab[stack.start + i]]->aabb
        );
    }
}
/*Returne the most use axis*/
Axis BVH::getAxis(Vec3 &AAtoBB)
{
    if (AAtoBB.x >= AAtoBB.y && AAtoBB.x >= AAtoBB.z)
        return Axis::X;
    if (AAtoBB.y >= AAtoBB.z)
        return Axis::Y;
    return Axis::Z;
}

Axis BVH::setAxis(AABB &a, bvh_stack_t &stack)
{
    Vec3 max(-INFINITY, -INFINITY, -INFINITY);
    Vec3 min(INFINITY, INFINITY, INFINITY);

    for (int i = stack.start ; i < stack.end; i++){
        if (max.x < Objects[IndexTab[i]]->aabb.max.x)
            max.x = Objects[IndexTab[i]]->aabb.max.x;
        if (max.y < Objects[IndexTab[i]]->aabb.max.y)
            max.y = Objects[IndexTab[i]]->aabb.max.y;
        if (max.z < Objects[IndexTab[i]]->aabb.max.z)
            max.z = Objects[IndexTab[i]]->aabb.max.z;
        if (min.x > Objects[IndexTab[i]]->aabb.min.x)
            min.x = Objects[IndexTab[i]]->aabb.min.x;
        if (min.y > Objects[IndexTab[i]]->aabb.min.y)
            min.y = Objects[IndexTab[i]]->aabb.min.y;
        if (min.z > Objects[IndexTab[i]]->aabb.min.z)
            min.z = Objects[IndexTab[i]]->aabb.min.z;
    }
    a.max = max;
    a.min = min;
    Vec3 AAtoBB = max - min;
    return getAxis(AAtoBB);
}

void BVH::AppliedSah(bvh_stack_t &stack, int &outPivot, float &outBestCost, float parentArea)
{
    int count = stack.end - stack.start;
    outBestCost = std::numeric_limits<float>::infinity();
    outPivot = stack.start + 1;

    float costTraversal = 1.0f;
    float costIntersect = 1.5f;

    for (int i = 0; i < count - 1; i++) {
        int leftCount  = i + 1;
        int rightCount = count - (i + 1);

        float pLeft = LeftSide[i].surfaceArea() / parentArea;
        float pRight = RightSide[i + 1].surfaceArea() / parentArea;
        
        float cost = costTraversal + costIntersect * (pLeft * leftCount + pRight * rightCount);

        if (cost < outBestCost) {
            outBestCost = cost;
            outPivot = stack.start + (i + 1);
        }
    }
}

/*===Create every single node of the bvh with a simulate stack alloc on the heaps===*/
void BVH::FillNode(std::vector<bvh_stack_t> &myStacks)
{
    int pivot = 0;
    int nodeIndex = this->SpThree.size();
    node_t newNode;
    bvh_stack_t stack = myStacks.back();
    Axis axis;
    float splitCost = std::numeric_limits<float>::infinity();
    
    myStacks.pop_back();
    newNode.start = stack.start;
    newNode.count = stack.end - stack.start;

    /*===Define SAH costs===*/
    float costIntersect = 1.5f;
    float leafCost = newNode.count * costIntersect;

    axis = setAxis(newNode.nodeShape, stack);
    CentroidSort(stack, axis);
    buildleftrightAABB(stack);

    float parentArea = newNode.nodeShape.surfaceArea();
    this->AppliedSah(stack, pivot, splitCost, parentArea);

    const int MAX_LEAF_SIZE = 8; 
    if (newNode.count <= 2 || (leafCost <= splitCost && newNode.count <= MAX_LEAF_SIZE)) {
        newNode.isLeaf = true;
        Objects[IndexTab[stack.start]]->aabb.normalize();
        newNode.nodeShape = Objects[IndexTab[stack.start]]->aabb;
        
        for (int i = 1; i < newNode.count; i++) {
            Objects[IndexTab[stack.start + i]]->aabb.normalize();
            newNode.nodeShape = Union(newNode.nodeShape, Objects[IndexTab[stack.start + i]]->aabb);
        }
    } else {
        newNode.isLeaf = false;
    }

    /*===set the left or right node of the parent to build the three===*/
    if (stack.parentIndex != -1){
        if (stack.isLeftChild == true)
            this->SpThree[stack.parentIndex].left = nodeIndex;
        if (stack.isLeftChild == false)
            this->SpThree[stack.parentIndex].right = nodeIndex;
    }
    
    SpThree.push_back(newNode);
    
    if (newNode.isLeaf == true)
        return;
        
    /*===Build Heaps stack===*/
    myStacks.push_back((bvh_stack_t){.start = stack.start, .end = pivot, .parentIndex=nodeIndex, .isLeftChild=true});
    myStacks.push_back((bvh_stack_t){.start = pivot, .end = stack.end, .parentIndex=nodeIndex, .isLeftChild=false});
}

/*===Init the bvh construction===*/
void BVH::BuildSpacePartitionning(void)
{
    /*===Binary Space three===*/
    this->SpThree.reserve(THREE_ALLOC(Objects.size()));
    /*===Index list for sorting obj===*/
    this->IndexTab.reserve(Objects.size());
    
    /*===Alloc heap stack to build a binary tree and init is first node===*/
    this->myStacks.reserve(THREE_ALLOC(Objects.size()));
    this->myStacks.push_back((bvh_stack_t){.start = 0, .end = (int)Objects.size(), .parentIndex = -1, .isLeftChild = false});
    /*===Alloc two array of node for SAH algo===*/
    this->LeftSide.reserve(Objects.size());
    this->RightSide.reserve(Objects.size());

    Log::Logger::debug("Three alloc:" + std::to_string(THREE_ALLOC(Objects.size())));

    for (size_t i = 0; i < Objects.size(); i++)
        IndexTab.push_back(int(i));

    while (myStacks.empty() == false){
        FillNode(myStacks);
    }

    /*===Clean Memory===*/
    this->myStacks.clear();
    this->myStacks.shrink_to_fit();
    
    this->LeftSide.clear();
    this->LeftSide.shrink_to_fit();
    
    this->RightSide.clear();
    this->RightSide.shrink_to_fit();

    /*===Debug: Display three node===*/
    if (Log::Logger::GetLogLvl() == Log::Logger::LogLvl::DEBUG)
        for (size_t i = 0; i != SpThree.size(); i++)
            std::cout << CLR_BOLD_DEBUG << "Three BVH:" << CLR_DEBUG
            "Node: " << i << " " <<
            "Is leaf:" << SpThree[i].isLeaf << " " <<
            "Left" << SpThree[i].left << " " <<
            "Right" << SpThree[i].right << " " <<  CLR_RESET <<
            std::endl;
}


bool BVH::intersect(Ray& ray, Hit& hit) noexcept
{
    int stack[64]; 
    int stackPtr = 0;

    stack[stackPtr++] = 0;
    bool Hit_valide = false;

    while (stackPtr > 0) {
        int index = stack[--stackPtr]; 
        Hit tmpHit;
        if (SpThree[index].nodeShape.intersect(ray) == false)
            continue;

        if (SpThree[index].isLeaf == true) {
            for (int i = 0; i < SpThree[index].count; i++) {
                int objIdx = IndexTab[SpThree[index].start + i];
                
                if (Objects[objIdx]->shape->intersect(ray, tmpHit) == true) {
                    if (tmpHit.t > 0.0001f && tmpHit.t < ray.maxHit) {
                        hit = tmpHit;
                        hit.ObjectIdx = objIdx;
                        ray.maxHit = tmpHit.t; 
                        Hit_valide = true;
                    }
                }
            }
            continue;
        }
        if (SpThree[index].right != -1)
            stack[stackPtr++] = SpThree[index].right;   
            
        if (SpThree[index].left != -1)
            stack[stackPtr++] = SpThree[index].left;
    }
    
    return Hit_valide;
}