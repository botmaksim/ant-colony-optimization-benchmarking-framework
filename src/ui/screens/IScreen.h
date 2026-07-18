#pragma once
#include "../AppContext.h"

class IScreen {
public:
    virtual ~IScreen() = default;
    virtual void update(AppContext& ctx, float dt) = 0;
    virtual void draw(AppContext& ctx) = 0;
};
