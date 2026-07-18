#pragma once
#include "IScreen.h"

class ViewerScreen : public IScreen {
public:
    void update(AppContext& ctx, float dt) override;
    void draw(AppContext& ctx) override;
};
