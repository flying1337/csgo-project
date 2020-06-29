#include "../cheat.h"

bool c_dynamicbb::bounding_box(ImVec2* a, ImVec2* b) {
    auto collid = g_offset_ptr(c_collider, this, 0x320);
    if (!collid)
        return false;

    auto min = collid->mins;
    auto max = collid->maxs;

    auto trans = g_offset(c_matrix4x4, this, 0x444);

    c_vector points[] = {
        c_vector(min.x, min.y, min.z),
        c_vector(min.x, max.y, min.z),
        c_vector(max.x, max.y, min.z),
        c_vector(max.x, min.y, min.z),
        c_vector(max.x, max.y, max.z),
        c_vector(min.x, max.y, max.z),
        c_vector(min.x, min.y, max.z),
        c_vector(max.x, min.y, max.z)
    };

    static auto vector_transform = [](const c_vector in1, const c_matrix4x4& in2)
    {
        static auto dot_product = [](const c_vector& v1, const float* v2)
        {
            return v1.x * v2[0] + v1.y * v2[1] + v1.z * v2[2];
        };

        auto out = c_vector();
        out[0] = dot_product(in1, in2.mtx[0]) + in2.mtx[0][3];
        out[1] = dot_product(in1, in2.mtx[1]) + in2.mtx[1][3];
        out[2] = dot_product(in1, in2.mtx[2]) + in2.mtx[2][3];

        return out;
    };

    c_vector pointsTransformed[8];
    for (int i = 0; i < 8; i++)
        pointsTransformed[i] = vector_transform(points[i], trans);

    auto
        pos = get_abs_origin(), flb = c_vector(), brt = c_vector(), blb = c_vector(), frt = c_vector(),
        frb = c_vector(), brb = c_vector(), blt = c_vector(), flt = c_vector();

    if (!utils->world_to_screen(pointsTransformed[3], &flb) || !utils->world_to_screen(pointsTransformed[5], &brt)
        || !utils->world_to_screen(pointsTransformed[0], &blb) || !utils->world_to_screen(pointsTransformed[4], &frt)
        || !utils->world_to_screen(pointsTransformed[2], &frb) || !utils->world_to_screen(pointsTransformed[1], &brb)
        || !utils->world_to_screen(pointsTransformed[6], &blt) || !utils->world_to_screen(pointsTransformed[7], &flt))
        return false;

    c_vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
    auto left = flb.x, top = flb.y, right = flb.x, bottom = flb.y;

    for (int i = 1; i < 8; i++)
    {
        if (left > arr[i].x)
            left = arr[i].x;

        if (top < arr[i].y)
            top = arr[i].y;

        if (right < arr[i].x)
            right = arr[i].x;

        if (bottom > arr[i].y)
            bottom = arr[i].y;
    }

    auto BotCenter = c_vector(right - ((right - left) / 2), top, 0);
    auto TopCenter = c_vector(right - ((right - left) / 2), bottom, 0);

    a->x = left;
    a->y = TopCenter.y;
    b->x = a->x + (right - left);
    b->y = a->y + (BotCenter.y - TopCenter.y);
    return true;
}