#include "object.h"

#include "vector4.h"

OLMatrix4f OLObject::objectMatrix(const OLVector3f& position, const OLVector3f& right, const OLVector3f& up, const OLVector3f& back, const OLVector3f& scale)
{
    OLMatrix4f translation
    {
        1.0f, 0.0f, 0.0f, position.x,
        0.0f, 1.0f, 0.0f, position.y,
        0.0f, 0.0f, 1.0f, position.z,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    OLMatrix4f rotation
    {
        right.x, up.x, back.x, 0.0f,
        right.y, up.y, back.y, 0.0f,
        right.z, up.z, back.z, 0.0f,
        0.0f,    0.0f, 0.0f,   1.0f
    };

    OLMatrix4f stretch
    {
        scale.x, 0.0f,    0.0f,    0.0f,
        0.0f,    scale.y, 0.0f,    0.0f,
        0.0f,    0.0f,    scale.z, 0.0f,
        0.0f,    0.0f,    0.0f,    1.0f
    };

    OLMatrix4f transform = translation * rotation * stretch;
    return transform;
}
