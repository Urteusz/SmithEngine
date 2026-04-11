#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "raylib.h"
#include "raymath.h"

/** namespace zrobione aby nie kolidowało z innymi bibliotekami */
namespace smith {

    struct Transform {
        Vector3 position = {0.0f, 0.0f, 0.0f};
        Vector3 rotation = {0.0f, 0.0f, 0.0f};
        Vector3 scale    = {1.0f, 1.0f, 1.0f};
        bool isDirty = true;
        mutable Matrix cachedMatrix;

        Matrix GetLocalMatrix() {
            if (isDirty) {
                Matrix matScale = MatrixScale(scale.x, scale.y, scale.z);
                Matrix matRot   = MatrixRotateXYZ({
                    rotation.x * DEG2RAD,
                    rotation.y * DEG2RAD,
                    rotation.z * DEG2RAD
                });
                Matrix matTrans = MatrixTranslate(position.x, position.y, position.z);
                cachedMatrix = MatrixMultiply(MatrixMultiply(matScale, matRot), matTrans);
                isDirty = false;
            }
            return cachedMatrix;
        }

        // Przesuń w kierunku lokalnym
        void Translate(Vector3 offset) {
            position = Vector3Add(position, offset);
            isDirty = true;
        }

        void Rotate(Vector3 eulerDeg) {
            rotation = Vector3Add(rotation, eulerDeg);
            isDirty = true;
        }
    };

}

#endif //TRANSFORM_H