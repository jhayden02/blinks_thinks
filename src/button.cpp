/***********************************************************************************************
*
*   button.cpp - The library for creating, drawing, and interacting with buttons.
*
*   Copyright (c) 2026 Josh Hayden (@jhayden02)
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
#include "button.hpp"
#include "game.hpp"

// Standard library.
#include <cmath>

using engine::button;
using engine::text;
using engine::game;

button::button(
    text* text_obj,
    Color bg_color,
    Rectangle rec,
    int layer,
    Color outline_color,
    float outline_size)
    :
    box(
        outline_color,
        bg_color,
        {rec.width, rec.height},
        outline_size,
        {rec.x + rec.width / 2.0f, rec.y + rec.height / 2.0f},
        layer
    ),
    m_text_obj(text_obj),
    m_default_text_color(text_obj->get_text_color()),
    m_current_text_color(m_default_text_color),
    m_default_bg_color(bg_color)
{
    m_text_obj->set_position(m_position);
}

button::~button()
{
    delete m_text_obj;

    for (auto* trait : m_traits) {
        delete trait;
    }
    m_traits.clear();
}

bool button::is_hovered()
{
    return CheckCollisionPointRec(GetMousePosition(), get_rectangle());
}

bool button::is_pressed()
{
    return is_hovered() && IsMouseButtonPressed(0);
}

void button::update()
{
    for (auto& trait : m_traits) {
        trait->update(*this);
    }

    box::update();

    set_fill_color(
        (is_hovered() && m_default_bg_color.a != 0)
            ? brighten_color(m_default_bg_color)
            : m_default_bg_color
    );

    m_current_text_color = is_hovered()
        ? brighten_color(m_default_text_color)
        : m_default_text_color;

    if (is_pressed() && m_sfx_press.has_value()) {
        PlaySound(*m_sfx_press);
    }

    m_text_obj->set_text_color(m_current_text_color);
    m_text_obj->set_scale(get_scale());
    m_text_obj->set_position(m_position);
    m_text_obj->update();
}

void button::draw()
{
    box::draw();
    m_text_obj->draw();
}

Color button::brighten_color(Color color)
{
    return {
        static_cast<unsigned char>(fmin(color.r * m_brighten_factor, 255)),
        static_cast<unsigned char>(fmin(color.g * m_brighten_factor, 255)),
        static_cast<unsigned char>(fmin(color.b * m_brighten_factor, 255)),
        color.a
    };
}
