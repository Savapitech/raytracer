#pragma once
    #include <memory>
    #include <vector>

    #include "Object.hpp"

    #define THREE_ALLOC(x) ((x * 2) - 1)

typedef struct bvh_stack_s {
    int start = -1;
    int end = -1;

    int parentIndex = -1;
    bool isLeftChild = false; /* Pour suprimé */
} bvh_stack_t;

typedef struct node_s {
    int left = -1;
    int right = -1;

    int start = -1;
    int count = -1;
    AABB nodeShape;

    bool isLeaf = false;
} node_t;


using VObjects = const std::vector<std::unique_ptr<Object>>&;

class BVH {
    public:
        void BuildSpacePartitionning(VObjects Objects);
        /*Find Object*/
    private:
        void CentroidSort(bvh_stack_t &stack, int axis, VObjects Objects);
        void FindBiggestAABB(VObjects Objects);
        void FillNode(VObjects Object, std::vector<bvh_stack_t> &myStacks);
        std::vector<node_t> SpThree;
        std::vector<int> IndexTab;
        std::vector<bvh_stack_t> myStacks;
        std::vector<AABB> LeftSide;
        std::vector<AABB> RightSide;
        //size_t NodeBuild;
};