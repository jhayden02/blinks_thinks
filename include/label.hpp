/***********************************************************************************************
*
*   label.cpp - The library for creating, drawing, and interacting with a simple rectangle.
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

#pragma once

// Source.
#include "entity.hpp"

namespace engine
{
    class label : public entity {
        public:
            label(
                Color line_color,
                Color fill_color,
                Vector2 size,
                int thickness,
                Vector2 position = {0, 0},
                int layer = 0); 

            void update() override;
            void draw() override;

            // Getters and setters.
            void set_scale(float scale) { m_scale = scale; }

            Rectangle get_rectangle() { return m_rectangle; }

        private:
            Color m_line_color;
            
            Color m_fill_color; 

            Vector2 m_size;             // The area the button's rectangle will occupy. Used to
                                        // calculate 'm_rectangle'.

            int m_thickness;            // The thickness of the shape's line.

            Rectangle m_rectangle;      // The rectangle used for most button actions. This is
                                        // calculated from 'm_size', 'm_position', and 'm_scale'. 

            float m_scale;              // What 'm_rectangle' and the label's 'm_fontSize' are
                                        // multiplied by.
    };
}
