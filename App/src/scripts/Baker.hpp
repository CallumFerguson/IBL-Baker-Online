#pragma once

#include "GameEngine.hpp"

class Baker : public GameEngine::ScriptableEntity {
public:
    void onStart();

    void onImGui();

    [[nodiscard]] const char *objectName() const override {
        return "Baker";
    }
};
