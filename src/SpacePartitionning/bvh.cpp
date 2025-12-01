#include <algorithm>
#include "bvh.hpp"




void BVH::FindBiggestAABB(VObjects Objects)
{
    for (size_t i = 0; i != Objects.size(); i++) {
    }
}

#define OBJECT_LEAF 1 /*Provisoire*/
enum Axis {
  X,
  Y,
  Z
}; 

void BVH::CentroidSort(bvh_stack_t &stack, int axis, VObjects Objects)
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



void BVH::FillNode(VObjects Objects, std::vector<bvh_stack_t> &myStacks)
{
    int pivot;
    int nodeIndex = this->SpThree.size();
    node_t newNode;
    bvh_stack_t stack = myStacks.back();
    Axis axis;
    
    newNode.start = stack.start;
    newNode.count = stack.end - stack.start;
    myStacks.pop_back();
    if (newNode.count == OBJECT_LEAF){
        newNode.isLeaf = true;
        newNode.count = OBJECT_LEAF;
    }
    if (newNode.isLeaf != true){
        newNode.nodeShape = buildleftrightAABB();
        /*axis = setaxis();*/
        CentroidSort(stack, X, Objects);
        pivot = stack.start + (stack.end - stack.start) / 2;/*RECUPERER PIVOT*/ /*Via le test de chaque AABB*/
        //newNode.nodeShape; /*= suffixe 0*/
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
    myStacks.push_back((bvh_stack_t){.start = stack.start, .end = pivot, .parentIndex=nodeIndex, .isLeftChild=false,});
    myStacks.push_back((bvh_stack_t){.start = pivot, .end = stack.end, .parentIndex=nodeIndex, .isLeftChild=true});
}

void BVH::BuildSpacePartitionning(VObjects Objects)
{
    this->SpThree.reserve(THREE_ALLOC(Objects.size()));
    this->IndexTab.reserve(Objects.size());
    this->myStacks.reserve(THREE_ALLOC(Objects.size()));
    Log::Logger::debug("Three alloc:" + std::to_string(THREE_ALLOC(Objects.size())));
    myStacks.push_back((bvh_stack_t){.start = 0, .end = (int)Objects.size(), .parentIndex = -1, .isLeftChild = false});

    for (size_t i = 0; i < Objects.size(); i++)
        IndexTab.push_back(int(i));
    while (myStacks.empty() == false){
        FillNode(Objects, myStacks);
    }    
}