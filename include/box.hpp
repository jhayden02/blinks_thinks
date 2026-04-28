/***********************************************************************************************
*
*   box.hpp - The library for creating, drawing, and interacting with a simple rectangle.
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

#pragma once

// Source.
#include "entity.hpp"

namespace engine
{

class box : public entity
{
    public:
        box(
            Color line_color,
            Color fill_color,
            Vector2 size,
            float thickness,
            Vector2 position = {0, 0},
            int layer = 0);

        void update() override;
        void draw() override;

        void set_scale(float scale) { m_scale = scale; }

        Rectangle get_rectangle() { return m_rectangle; }

    private:
        Color m_line_color;

        Color m_fill_color;

        Vector2 m_size;

        float m_thickness;

        Rectangle m_rectangle;

        float m_scale;
};

} // NAMESPACE ENGINE.
