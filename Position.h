#pragma once
#ifndef POSITION_H
#define POSITION_H

struct Position {
    float x = 0.f;
    float y = 0.f;
    Position() = default;
    Position(float x, float y) : x(x), y(y) {}
};
#endif
