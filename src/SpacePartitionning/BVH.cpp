#include <algorithm>
#include "Ray.hpp"
#include "AABB.hpp"

void BVH::centroidSort(bvh_stack_t &stack, int axis)
{
    std::sort(_indexTab.begin() + stack.start, _indexTab.begin() + stack.end,
        [&](int a, int b) {
            const Vec3 &ca = _objects[a]->getCentroid();
            const Vec3 &cb = _objects[b]->getCentroid();

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
AABB BVH::makeUnion(AABB a, AABB b)
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

    _leftSide.resize(count);
    _rightSide.resize(count);
    _leftSide[0] = _objects[_indexTab[stack.start]]->getAABB();
    for (int i = 1; i < count; i++) {
        _leftSide[i] = makeUnion(
            _leftSide[i - 1],
            _objects[_indexTab[stack.start + i]]->getAABB()
        );
    }
    _rightSide[count - 1] = _objects[_indexTab[stack.end - 1]]->getAABB();

    for (int i = count - 2; i >= 0; i--) {
        _rightSide[i] = makeUnion(
            _rightSide[i + 1],
            _objects[_indexTab[stack.start + i]]->getAABB()
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
        if (max.x < _objects[_indexTab[i]]->getAABB().max.x)
            max.x = _objects[_indexTab[i]]->getAABB().max.x;
        if (max.y < _objects[_indexTab[i]]->getAABB().max.y)
            max.y = _objects[_indexTab[i]]->getAABB().max.y;
        if (max.z < _objects[_indexTab[i]]->getAABB().max.z)
            max.z = _objects[_indexTab[i]]->getAABB().max.z;
        if (min.x > _objects[_indexTab[i]]->getAABB().min.x)
            min.x = _objects[_indexTab[i]]->getAABB().min.x;
        if (min.y > _objects[_indexTab[i]]->getAABB().min.y)
            min.y = _objects[_indexTab[i]]->getAABB().min.y;
        if (min.z > _objects[_indexTab[i]]->getAABB().min.z)
            min.z = _objects[_indexTab[i]]->getAABB().min.z;
    }
    a.max = max;
    a.min = min;
    Vec3 AAtoBB = max - min;
    return getAxis(AAtoBB);
}

void BVH::appliedSah(bvh_stack_t &stack, int &outPivot, float &outBestCost, float parentArea)
{
    int count = stack.end - stack.start;
    outBestCost = std::numeric_limits<float>::infinity();
    outPivot = stack.start + 1;

    float costTraversal = 1.0f;
    float costIntersect = 1.5f;

    for (int i = 0; i < count - 1; i++) {
        int leftCount  = i + 1;
        int rightCount = count - (i + 1);

        float pLeft = _leftSide[i].surfaceArea() / parentArea;
        float pRight = _rightSide[i + 1].surfaceArea() / parentArea;
        
        float cost = costTraversal + costIntersect * (pLeft * leftCount + pRight * rightCount);

        if (cost < outBestCost) {
            outBestCost = cost;
            outPivot = stack.start + (i + 1);
        }
    }
}

/*===Create every single node of the bvh with a simulate stack alloc on the heaps===*/
void BVH::fillNode(std::vector<bvh_stack_t> &_myStacks)
{
    int pivot = 0;
    int nodeIndex = this->_spacialThree.size();
    node_t newNode;
    bvh_stack_t stack = _myStacks.back();
    Axis axis;
    float splitCost = std::numeric_limits<float>::infinity();
    
    _myStacks.pop_back();
    newNode.start = stack.start;
    newNode.count = stack.end - stack.start;

    /*===Define SAH costs===*/
    float costIntersect = 1.5f;
    float leafCost = newNode.count * costIntersect;

    axis = setAxis(newNode.nodeShape, stack);
    centroidSort(stack, axis);
    buildleftrightAABB(stack);

    float parentArea = newNode.nodeShape.surfaceArea();
    this->appliedSah(stack, pivot, splitCost, parentArea);

    const int MAX_LEAF_SIZE = 8; 
    if (newNode.count <= 2 || (leafCost <= splitCost && newNode.count <= MAX_LEAF_SIZE)) {
        newNode.isLeaf = true;
        _objects[_indexTab[stack.start]]->getAABB();//.normalize();
        newNode.nodeShape = _objects[_indexTab[stack.start]]->getAABB();
        
        for (int i = 1; i < newNode.count; i++) {
            _objects[_indexTab[stack.start + i]]->getAABB();//.normalize();
            newNode.nodeShape = makeUnion(newNode.nodeShape, _objects[_indexTab[stack.start + i]]->getAABB());
        }
    } else {
        newNode.isLeaf = false;
    }

    /*===set the left or right node of the parent to build the three===*/
    if (stack.parentIndex != -1){
        if (stack.isLeftChild == true)
            this->_spacialThree[stack.parentIndex].left = nodeIndex;
        if (stack.isLeftChild == false)
            this->_spacialThree[stack.parentIndex].right = nodeIndex;
    }
    
    _spacialThree.push_back(newNode);
    
    if (newNode.isLeaf == true)
        return;
        
    /*===Build Heaps stack===*/
    _myStacks.push_back(bvh_stack_t(stack.start, pivot, nodeIndex, true));
    _myStacks.push_back(bvh_stack_t(pivot, stack.end, nodeIndex, false));
}

/*===Init the bvh construction===*/
void BVH::buildSpacePartitionning(void)
{
    /*===Binary Space three===*/
    this->_spacialThree.reserve(THREE_ALLOC(_objects.size()));
    /*===Index list for sorting obj===*/
    this->_indexTab.reserve(_objects.size());
    
    /*===Alloc heap stack to build a binary tree and init is first node===*/
    this->_myStacks.reserve(THREE_ALLOC(_objects.size()));
    this->_myStacks.push_back(bvh_stack_t(0, (int)_objects.size(), -1, false));
    /*===Alloc two array of node for SAH algo===*/
    this->_leftSide.reserve(_objects.size());
    this->_rightSide.reserve(_objects.size());

    Log::Logger::debug("Three alloc:" + std::to_string(THREE_ALLOC(_objects.size())));

    for (size_t i = 0; i < _objects.size(); i++)
        _indexTab.push_back(int(i));

    while (_myStacks.empty() == false){
        fillNode(_myStacks);
    }

    /*===Clean Memory===*/
    this->_myStacks.clear();
    this->_myStacks.shrink_to_fit();
    
    this->_leftSide.clear();
    this->_leftSide.shrink_to_fit();
    
    this->_rightSide.clear();
    this->_rightSide.shrink_to_fit();

    /*===Debug: Display three node===*/
    if (Log::Logger::GetLogLvl() == Log::Logger::LogLvl::DEBUG)
        for (size_t i = 0; i != _spacialThree.size(); i++)
            std::cout << CLR_BOLD_DEBUG << "Three BVH:" << CLR_DEBUG
            "Node: " << i << " " <<
            "Is leaf:" << _spacialThree[i].isLeaf << " " <<
            "Left" << _spacialThree[i].left << " " <<
            "Right" << _spacialThree[i].right << " " <<  CLR_RESET <<
            std::endl;
}

bool BVH::intersect(Ray& ray, Hit& hit) noexcept
{
    int stack[64]; 
    int stackPtr = 0;

    stack[stackPtr++] = 0;
    bool hitValide = false;

    while (stackPtr > 0) {
        int index = stack[--stackPtr]; 
        Hit tmpHit;
        if (_spacialThree[index].nodeShape.intersect(ray) == false)
            continue;

        if (_spacialThree[index].isLeaf == true) {
            for (int i = 0; i < _spacialThree[index].count; i++) {
                int objIdx = _indexTab[_spacialThree[index].start + i];
                
                if (_objects[objIdx]->getShape()->intersect(ray, tmpHit) == true) {
                    if (tmpHit.t > 0.0001f && tmpHit.t < ray.maxHit) {
                        hit = tmpHit;
                        hit.ObjectIdx = objIdx;
                        ray.maxHit = tmpHit.t; 
                        hitValide = true;
                    }
                }
            }
            continue;
        }
        if (_spacialThree[index].right != -1)
            stack[stackPtr++] = _spacialThree[index].right;   
            
        if (_spacialThree[index].left != -1)
            stack[stackPtr++] = _spacialThree[index].left;
    }
    
    return hitValide;
}
