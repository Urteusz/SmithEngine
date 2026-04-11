#ifndef NODE3D_H
#define NODE3D_H

#include "core/Node.h"
#include "core/Transform.h"

class Node3D : public Node {
public:
    smith::Transform transform;

    Node3D() = default;
    ~Node3D() override = default;

    Matrix GetWorldTransform();
};

#endif //NODE3D_H
