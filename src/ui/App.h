#pragma once
#include "AppContext.h"
#include "screens/IScreen.h"
#include <memory>
#include <unordered_map>

class App {
public:
    App();
    void run();

private:
    AppContext m_ctx;
    std::unordered_map<ScreenState, std::unique_ptr<IScreen>> m_screens;

    void update(float dt);
    void draw();
};
