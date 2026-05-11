#pragma once
#include <memory>
#include <vector>

#include "Object.hpp"
#include "math3d.hpp"

#define THREE_ALLOC(x) ((x * 2) - 1)

enum Axis { X = 'x', Y = 'y', Z = 'z' };

typedef struct bvh_stack_s {
  int start = -1;
  int end = -1;

  int parentIndex = -1;
  bool isLeftChild = false;
} bvh_stack_t;

// struct node_t
//{
//     AABB nodeShape;
//
//     union {
//         int left;
//         int start;
//     };
//
//     union {
//         int right;
//         struct {
//             unsigned int count : 31;
//             unsigned int isLeaf : 1;
//         };
//     };
//
//     node_t()
//     {
//         left = -1;
//         count = -1;
//         isLeaf = 0;
//     }
// };

typedef struct node_s {
  int left = -1;
  int right = -1;

  int start = -1;
  int count = -1;
  AABB nodeShape;

  bool isLeaf = false;
} node_t;

class Object;

using VObjects = const std::vector<std::unique_ptr<Object>> &;

class BVH {
public:
  bool intersect(Ray &ray, Hit &hit) noexcept;
  void buildSpacePartitionning(void);

  BVH(VObjects objects) : _objects(objects) {}
  /*Find Object*/
private:
  void centroidSort(bvh_stack_t &stack, int axis);
  void fillNode(std::vector<bvh_stack_t> &myStacks);
  void buildleftrightAABB(bvh_stack_t &myStack);
  AABB makeUnion(AABB a, AABB b);
  Axis setAxis(AABB &a, bvh_stack_t &myStack);
  Axis getAxis(Vec3 &AAtoBB);
  void appliedSah(bvh_stack_t &stack, int &outPivot, float &outBestCost,
                  float parentArea);

  VObjects _objects;

  std::vector<node_t> _spacialThree;
  std::vector<int> _indexTab;
  std::vector<int> _indexInfiniteObject;

  std::vector<bvh_stack_t> _myStacks;
  std::vector<AABB> _leftSide;
  std::vector<AABB> _rightSide;
};
