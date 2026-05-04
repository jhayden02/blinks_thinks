/***********************************************************************************************
* 
*   text.hpp - The library for creating and drawing formatted text.
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
#include "text_traits.hpp"

// Standard library.
#include <string>
#include <vector>

using std::string;
using std::vector;

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
            float outline_size = 2.0f
        );

        void update() override;
        void draw() override;

        void add_trait(text_trait* trait) { m_traits.push_back(trait); }

        float get_rotation() { return m_rotation; }
        void set_rotation(float rotation) { m_rotation = rotation; }

        string get_text_str() { return m_text_str; }
        void set_text_str(string text_str) { m_text_str = text_str; }

        Color get_text_color() { return m_text_color; }
        void set_text_color(Color text_color) { m_text_color = text_color; }

        Color get_outline_color() { return m_outline_color; }
        void set_outline_color(Color outline_color) { m_outline_color = outline_color; }

        float get_outline_size() { return m_outline_size; }
        void set_outline_size(float outline_size) { m_outline_size = outline_size; }

        Rectangle get_rec() { return m_rec; }

        int get_font_size() { return m_font_size; }
        void set_font_size(int font_size) { m_font_size = font_size; }

        void set_scale(float scale) { m_scale = scale; }

    private:
        Font m_font;

        string m_text_str;

        float m_scale;

        float m_font_size;

        Color m_text_color;

        Color m_outline_color;

        int m_letter_spacing;

        Rectangle m_rec;

        Vector2 m_origin;

        float m_outline_size;

        float m_rotation;

        // The storage container to hold all active traits attached to the text.
        vector<text_trait*> m_traits;
};

} // NAMESPACE ENGINE.
