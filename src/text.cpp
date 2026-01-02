/***********************************************************************************************
*
*   text.cpp - The library for creating and drawing formatted text.
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
#include "text.hpp"

using engine::text;

// Standard library.
#include <cmath>

text::text(
    string text_str,
    float font_size,
    Color text_color,
    Vector2 position,
    int layer,
    Color outline_color,
    float outline_size)

    :
    entity(position, layer),

    m_text_str(text_str),
    m_scale(1.0f),

    m_base_font_size(font_size),
    m_scaled_font_size(m_base_font_size * m_scale),

    m_text_color(text_color),
    m_outline_color(outline_color),

    // Updated every frame in update().
    m_letter_spacing(m_base_font_size / 10.0f),
    m_text_dim(MeasureTextEx(GetFontDefault(), m_text_str.c_str(), m_base_font_size, m_letter_spacing)),
    m_origin({ m_text_dim.x / 2.0f, m_text_dim.y / 2.0f }),

    m_outline_size(outline_size),

    // Set to 0, with the prospect of being set at a later time.
    m_rotation(0.0f),
    m_rotation_speed(0.0f),
    m_rotation_depth(0.0f)
{}

void text::update()
{ 
    entity::update();

    // update the scaled font size in case scale has changed since last frame. 
    m_scaled_font_size = m_base_font_size * m_scale;
    
    m_letter_spacing = m_scaled_font_size / 10.0f;
    m_text_dim = MeasureTextEx(GetFontDefault(), m_text_str.c_str(), m_scaled_font_size, m_letter_spacing);
    m_origin = { m_text_dim.x / 2.0f, m_text_dim.y / 2.0f };
    m_rotation = sin(GetTime() * m_rotation_speed) * m_rotation_depth;
}

void text::draw()
{
    // Draw outline by rendering text in 8 directions around the center.
    // Only draw the outline if it has a visible alpha and non-zero size.
    if (m_outline_color.a != 0 && m_outline_size > 0.0f) {
        for (int i = 0; i < 8; ++i) {
            float angle = i * (PI * 2.0f) / 8.0f;
            Vector2 offset = {
                cosf(angle) * m_outline_size,
                sinf(angle) * m_outline_size
            };
            DrawTextPro(
                GetFontDefault(),
                m_text_str.c_str(),
                { m_position.x + offset.x, m_position.y + offset.y },
                m_origin,
                m_rotation,
                m_scaled_font_size,
                m_letter_spacing,
                m_outline_color
            );
        }
    }

    // Draw main text on top.
    DrawTextPro(
        GetFontDefault(),
        m_text_str.c_str(),
        m_position,
        m_origin,
        m_rotation,
        m_scaled_font_size,
        m_letter_spacing,
        m_text_color
    );
}
