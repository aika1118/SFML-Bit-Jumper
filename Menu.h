#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer.h"

using namespace sf;

class Menu {
public:
    virtual ~Menu() = default;
    virtual void init(RenderWindow& window) = 0;
    virtual void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) = 0;
    virtual void render(Renderer& renderer) = 0;
};