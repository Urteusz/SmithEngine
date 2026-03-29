#include "nodes/Node3D.h"

Matrix Node3D::GetWorldTransform() const {
    if (parent) {
        auto* parentNode3D = dynamic_cast<Node3D*>(parent);
        if (parentNode3D) {
            return transform.GetWorldMatrix(&parentNode3D->transform);
        }
    }
    return transform.GetLocalMatrix();
}
