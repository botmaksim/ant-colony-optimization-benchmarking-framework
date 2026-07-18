#pragma once
#include "IScreen.h"

class TournamentScreen : public IScreen {
public:
    void update(AppContext& ctx, float dt) override;
    void draw(AppContext& ctx) override;
};
