/***********************************************************************************************
* 
*   text.hpp - The library for creating and drawing formatted text.
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

// Standard library.
#include <string>

using std::string;

namespace engine
{
    class text : public entity
    {
        public:
            text(
                string text_str,
                float font_size,
                Color text_color,
                Vector2 position = {0, 0},
                int layer = 0,
                Color outline_color = BLACK,
                float outline_size = 2.0f); 

            void update() override;
            void draw() override;

            void add_anim_rotate(float rotation, float speed, float depth) {
                m_rotation = rotation;
                m_rotation_speed = speed;
                m_rotation_depth = depth;
            }

            // Getters and setters.
            string get_text_str() { return m_text_str; }
            void set_text_str(string text_str) { m_text_str = text_str; }

            Color get_text_color() { return m_text_color; }
            void set_text_color(Color text_color) { m_text_color = text_color; }

            Color get_outline_color() { return m_outline_color; }
            void set_outline_color(Color outline_color) { m_outline_color = outline_color; }

            float get_outline_size() { return m_outline_size; }
            void set_outline_size(float outline_size) { m_outline_size = outline_size; }

            Vector2 get_text_dim() { return m_text_dim; }
            void set_text_dim(Vector2 text_dim) { m_text_dim = text_dim; }

            int get_font_size() { return m_base_font_size; }
            void set_font_size(int font_size) { m_base_font_size = font_size; }

            void set_scale(float scale) { m_scale = scale; }

        private:
            string m_text_str;              // The text that will be displayed.
                                            
            float m_scale;                  // The scaling factor that should be applied to the
                                            // original font size.
                                            
            float m_base_font_size;         // The size that the text is initially (unscaled).
                                            
            float m_scaled_font_size;       // The size that the text is after scaling is applied.
                                            
            Color m_text_color;             // The color that the text will be displayed.

            Color m_outline_color;          // The color of the outline drawn around the text.
                                            
            int m_letter_spacing;           // The pixels of spacing in between letters.
                                            
            Vector2 m_text_dim;             // The text's dimensions. Calculated after font size,
                                            // letter spacing, and scaling have all been applied.
                                            
            Vector2 m_origin;               // The horizontal and vertical center of the text.
                                            // Calculated after font size, letter spacing, and
                                            // scaling have all been applied.

            float m_outline_size;           // The thickness of the outline in pixels.

            // Rotation algorithm: m_rotation = sin(GetTime() * m_rotation_speed) * m_rotation_depth

            float m_rotation;               // The rotation of the text (default is 0.0f).
            float m_rotation_speed;         // The speed of rotation (default is 0.0f).
            float m_rotation_depth;         // The depth of rotation (default is 0.0f).
    };
}
