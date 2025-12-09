#include "RayTracer.hpp"

bool BVH::intersect(Ray& ray, Hit& hit) noexcept
{
    std::vector<int> NodeIndex;
    NodeIndex.reserve((this->SpThree.size() + 1) / 2);
    NodeIndex.push_back(0);
    bool Hit_valide = false;

    while (NodeIndex.empty() == false) {
        Hit tmpHit;
        int index = NodeIndex.back();
        NodeIndex.pop_back();

        if (SpThree[index].nodeShape.intersect(ray) == false){
            continue;
        }
        if (SpThree[index].isLeaf == true){
            if (Objects[IndexTab[SpThree[index].start]]->shape->intersect(ray, tmpHit) == true){
                if (tmpHit.t > 0 && tmpHit.t < hit.t) {
                    hit = tmpHit;
                    //hit.object = Objects[IndexTab[SpThree[index].start]].get();
                    hit.ObjectIdx = IndexTab[SpThree[index].start];
                    ray.maxHit = tmpHit.t;
                    Hit_valide = true;
                }
            }
            continue;
        }
        if (SpThree[index].right != -1)
                NodeIndex.push_back(SpThree[index].right);   
        if (SpThree[index].left != -1)
                NodeIndex.push_back(SpThree[index].left);
    }
    return Hit_valide;
}
