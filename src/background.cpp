/***********************************************************************************************
*
*   background.cpp - The library for creating and drawing backgrounds.
*
*   Copyright (c) 2025 Josh Hayden (@BlinkDynamo)
*
*   Blink's Thinks is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License v3.0 as published
*   by the Free Software Foundation.
*  
*   Blink's Thinks is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*  
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
***********************************************************************************************/

// Source.
#include "game.hpp"
#include "background.hpp"

// Standard library.
#include <cmath>

// Source.
using engine::background;
using engine::game;

float background::m_scroll_offset = 0.0f;

background::background(
    Color dark_color,
    Color light_color,
    int square_size)
    :
    entity({0, 0}, -1000), // -1000 is the default layer of Backgrounds.
    m_dark_color(dark_color),
    m_light_color(light_color),
    m_square_size(square_size)
{}

background::~background()
{}

void background::update()
{
    set_scroll_offset(get_scroll_offset() + GetFrameTime() * 30.0f);
}

void background::draw()
{
    const int cols = (game::get_w() / m_square_size) + 2;
    const int rows = (game::get_h() / m_square_size) + 2;

    const float effective_offset = std::fmod(get_scroll_offset(), 2 * m_square_size);

    game& game_inst = game::get_instance();
    game_inst.shaders->begin();

    ClearBackground(RAYWHITE);
    for (int y = -2; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            const bool is_dark = (x + y) % 2 == 0;
            const Color color = is_dark ? m_dark_color : m_light_color;

            const float draw_x = x * m_square_size;
            const float draw_y = y * m_square_size + effective_offset;

            DrawRectangle(draw_x, draw_y, m_square_size, m_square_size, color);
        }
    }

    game_inst.shaders->append("blur");
    game_inst.shaders->append("vignette");
    game_inst.shaders->process();
}
