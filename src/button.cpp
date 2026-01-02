/***********************************************************************************************
*
*   button.cpp - The library for creating, drawing, and interacting with buttons.
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
#include "button.hpp"
#include "game.hpp"

using engine::button;
using engine::text;
using engine::game;

// Standard library.
#include <cmath>

//
// Base button class.
//
button::button(
    text* text_obj,
    Color bg_color,
    Vector2 size,
    Vector2 position,
    int layer)

    :
    // Arguments.
    entity(position, layer),
    m_text_obj(text_obj),
    m_size(size),

    // Updated every frame in 'update()'.
    m_rectangle({m_position.x - (m_size.x / 2.0f), m_position.y - (m_size.y / 2.0f), m_size.x, m_size.y}),

    // Set once here, but are still mutable.
    m_default_text_color(text_obj->get_text_color()),
    m_current_text_color(m_default_text_color),
    m_default_bg_color(bg_color),
    m_current_bg_color(m_default_bg_color),
    m_scale(1.0f)
{
    this->m_text_obj->set_position(m_position);
}

button::~button() {
    delete m_text_obj;

    for (auto* trait : m_traits) {
        delete trait;
    }
    m_traits.clear();
}

bool button::is_hovered()
{
    return CheckCollisionPointRec(GetMousePosition(), m_rectangle);
}

bool button::is_pressed()
{
    return is_hovered() && IsMouseButtonPressed(0);
}

void button::update()
{ 
    // Update the base entity.
    entity::update();

    // Update all button traits.
    for (auto& trait : m_traits) {
        trait->update(*this);
    }

    // update the rectangle, multiplying size elements by scale.
    m_rectangle = {
        m_position.x - ((m_size.x * m_scale) / 2.0f),
        m_position.y - ((m_size.y * m_scale) / 2.0f),
        m_size.x * m_scale,
        m_size.y * m_scale
    };

    // update the text object's scale so that it's update() step can scale the font size.
    m_text_obj->set_scale(m_scale);

    // Set the text object's position to the button's position in case the button moves.
    m_text_obj->set_position(m_position);

    // update the color of the button's text object if hovered.
    if (is_hovered()) {
        float brightenFactor = 2.0f;

        // If the background is visible, brighten the background and the text color.
        if (m_default_bg_color.a != 0) {
            m_current_bg_color = { 
                static_cast<unsigned char>(fmin(m_default_bg_color.r * brightenFactor, 255)),
                static_cast<unsigned char>(fmin(m_default_bg_color.g * brightenFactor, 255)),
                static_cast<unsigned char>(fmin(m_default_bg_color.b * brightenFactor, 255)),
                m_default_bg_color.a
            };
            m_current_text_color = { 
                static_cast<unsigned char>(fmin(m_default_text_color.r * brightenFactor, 255)),
                static_cast<unsigned char>(fmin(m_default_text_color.g * brightenFactor, 255)),
                static_cast<unsigned char>(fmin(m_default_text_color.b * brightenFactor, 255)),
                m_default_text_color.a
            };
        }
        // Otherwise only brighten the text object.
        else { 
            m_current_text_color = { 
                static_cast<unsigned char>(fmin(m_default_text_color.r * brightenFactor, 255)),
                static_cast<unsigned char>(fmin(m_default_text_color.g * brightenFactor, 255)),
                static_cast<unsigned char>(fmin(m_default_text_color.b * brightenFactor, 255)),
                m_default_text_color.a
            };
        }
    }
    else {
        m_current_bg_color = m_default_bg_color;
        m_current_text_color = m_default_text_color;
    }

    if (is_pressed() && m_sfx_press.has_value()) {
        PlaySound(*m_sfx_press);
    }

    m_text_obj->set_text_color(m_current_text_color); 

    // Update the text object.
    m_text_obj->update(); 
}

void button::draw()
{
    // draw the rectangle and text object.
    DrawRectangleRec(m_rectangle, m_current_bg_color);
    m_text_obj->draw(); 
}
