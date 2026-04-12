#pragma once
    #include <memory>
    #include <vector>

    #include "Object.hpp"

    #define THREE_ALLOC(x) ((x * 2) - 1)

    enum Axis {
      X = 'x',
      Y = 'y',
      Z = 'z'
    }; 


typedef struct bvh_stack_s {
    int start = -1;
    int end = -1;

    int parentIndex = -1;
    bool isLeftChild = false;
} bvh_stack_t;


struct node_t 
{
    AABB nodeShape;

    union {
        int left;
        int start;
    };

    union {
        int right;
        struct {
            unsigned int count : 31;  
            unsigned int isLeaf : 1; 
        };
    };

    node_t() 
    {
        left = -1;
        count = -1;
        isLeaf = 0; 
    }
};


//typedef struct node_s {
//    int left = -1;
//    int right = -1;
//
//    int start = -1;
//    int count = -1;
//    AABB nodeShape;
//
//    bool isLeaf = false;
//} node_t;




using VObjects = const std::vector<std::unique_ptr<Object>>&;

class BVH {
    public:
        bool intersect(Ray& ray, Hit& hit) noexcept;
        void BuildSpacePartitionning(void);

        BVH(VObjects Objects) 
        : Objects(Objects){}
        /*Find Object*/
    private:
        void CentroidSort(bvh_stack_t &stack, int axis);
        void FindBiggestAABB(VObjects Objects);
        void FillNode(std::vector<bvh_stack_t> &myStacks);
        void buildleftrightAABB(bvh_stack_t &myStack);
        AABB Union(AABB a, AABB b);
        Axis setAxis(AABB &a, bvh_stack_t &myStack);
        Axis getAxis(Vec3 &AAtoBB);
        void AppliedSah(bvh_stack_t &stack, int &outPivot, float &outBestCost, float parentArea);

        VObjects Objects;
        std::vector<node_t> SpThree;
        std::vector<int> IndexTab;
        std::vector<bvh_stack_t> myStacks;
        std::vector<AABB> LeftSide;
        std::vector<AABB> RightSide;
};