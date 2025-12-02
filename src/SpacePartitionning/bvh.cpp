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

Vec3 GetMax(Vec3 a, Vec3 b)
{
    Vec3 newMax;
    newMax.x = (a.x > b.x) ? a.x : b.x;
    newMax.y = (a.y > b.y) ? a.y : b.y;
    newMax.z = (a.z > b.z) ? a.z : b.z;
    return newMax;
}

Vec3 GetMin(Vec3 a, Vec3 b)
{
    Vec3 newMin;
    newMin.x = (a.x < b.x) ? a.x : b.x;
    newMin.y = (a.y < b.y) ? a.y : b.y;
    newMin.z = (a.z < b.z) ? a.z : b.z;
    return newMin;
}

void AABB::normalize()
{
    if (min.x > max.x) std::swap(min.x, max.x);
    if (min.y > max.y) std::swap(min.y, max.y);
    if (min.z > max.z) std::swap(min.z, max.z);
}

AABB BVH::Union(AABB a, AABB b)
{
    AABB r;
    r.min = GetMin(a.min, b.min);
    r.max = GetMax(a.max, b.max);
    r.normalize();
    return r;
}

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

int BVH::AppliedSah(bvh_stack_t &stack)
{
    int count = stack.end - stack.start;
    float maxCost = std::numeric_limits<float>::infinity();
    int   pivot = stack.start + 1;

    for (int i = 0; i < count - 1; i++) {
        int leftCount  = i + 1;
        int rightCount = count - (i + 1);
        float cost = LeftSide[i].surfaceArea()  * leftCount + RightSide[i + 1].surfaceArea() * rightCount;

        if (cost < maxCost) {
            maxCost = cost;
            pivot = stack.start + (i + 1);
        }
    }
    return pivot;
}

void BVH::FillNode(std::vector<bvh_stack_t> &myStacks)
{
    int pivot;
    int nodeIndex = this->SpThree.size();
    node_t newNode;
    bvh_stack_t stack = myStacks.back();
    Axis axis;
    
    /* Init */
    myStacks.pop_back();
    newNode.start = stack.start;
    newNode.count = stack.end - stack.start;

    /*Construction Node*/
    if (newNode.count == OBJECT_LEAF){
        newNode.isLeaf = true;
        Objects[IndexTab[stack.start]]->aabb.normalize();
        newNode.nodeShape =  Objects[IndexTab[stack.start]]->aabb;
        newNode.count = OBJECT_LEAF;
    }
    if (newNode.isLeaf != true){
        newNode.nodeShape.normalize();
        axis = setAxis(newNode.nodeShape, stack);
        CentroidSort(stack, axis);
        buildleftrightAABB(stack);
        pivot = this->AppliedSah(stack);
    }
    if (stack.parentIndex != -1){
        if (stack.isLeftChild == true)
            this->SpThree[stack.parentIndex].left = nodeIndex;
        if (stack.isLeftChild == false)
            this->SpThree[stack.parentIndex].right = nodeIndex;
    }
    SpThree.push_back(newNode);
    if (newNode.isLeaf == true)
        return;
    /*Création récursif*/
    myStacks.push_back((bvh_stack_t){.start = stack.start, .end = pivot, .parentIndex=nodeIndex, .isLeftChild=true});
    myStacks.push_back((bvh_stack_t){.start = pivot, .end = stack.end, .parentIndex=nodeIndex, .isLeftChild=false});
}

void BVH::BuildSpacePartitionning(void)
{
    this->SpThree.reserve(THREE_ALLOC(Objects.size())); /* Arbre*/
    this->IndexTab.reserve(Objects.size()); /*Liste d'index pour trier les obj*/
    this->myStacks.reserve(THREE_ALLOC(Objects.size())); /**/
    this->LeftSide.reserve(Objects.size());
    this->RightSide.reserve(Objects.size());
    Log::Logger::debug("Three alloc:" + std::to_string(THREE_ALLOC(Objects.size())));

    myStacks.push_back((bvh_stack_t){.start = 0, .end = (int)Objects.size(), .parentIndex = -1, .isLeftChild = false});

    for (size_t i = 0; i < Objects.size(); i++)
        IndexTab.push_back(int(i));

    while (myStacks.empty() == false){
        FillNode(myStacks);
    }

    /*Display node*/
    if (Log::Logger::GetLogLvl() == Log::Logger::LogLvl::DEBUG)
        for (size_t i = 0; i != SpThree.size(); i++)
            std::cout << CLR_BOLD_DEBUG << "Three BVH:" << CLR_DEBUG
            "Node: " << i << " " <<
            "Is leaf:" << SpThree[i].isLeaf << " " <<
            "Left" << SpThree[i].left << " " <<
            "Right" << SpThree[i].right << " " <<  CLR_RESET <<
            std::endl;
}