/***********************************************************************************************
*
*   box.cpp - The library for creating, drawing, and interacting with a simple rectangle.
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
#include "box.hpp"

using engine::box;

// Standard library.
#include <cmath>

box::box(
    Color line_color,
    Color fill_color,
    Vector2 size,
    float thickness,
    Vector2 position,
    int layer)

    :
    // Arguments.
    entity(position, layer),

    m_line_color(line_color),
    m_fill_color(fill_color),
    m_size(size),
    m_thickness(thickness),

    // Updated every frame in 'update()'.
    m_rectangle({m_position.x - (m_size.x / 2.0f), m_position.y - (m_size.y / 2.0f), m_size.x, m_size.y}),

    m_scale(1.0f)
{}

// ------------------------------------------------------------------------------------------ //
//                                          Methods.                                          //
// ------------------------------------------------------------------------------------------ //
void box::update()
{ 
    entity::update();

    // update the rectangle, multiplying size elements by scale.
    m_rectangle = {
        m_position.x - ((m_size.x * m_scale) / 2.0f),
        m_position.y - ((m_size.y * m_scale) / 2.0f),
        m_size.x * m_scale,
        m_size.y * m_scale
    };
}

void box::draw()
{
    // Draw the filled portion.
    DrawRectangleRec(m_rectangle, m_fill_color);

    // Draw the outline if an above-zero thickness is specified.
    if (m_thickness > 0) {
        DrawRectangleLinesEx(m_rectangle, m_thickness, m_line_color);
    }
}
