/***********************************************************************************************
*
*   button.hpp - The library for creating, drawing, and interacting with buttons.
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
#include "box.hpp"
#include "button_traits.hpp"
#include "text.hpp"

// Standard library.
#include <vector>

using std::vector;

namespace engine
{

class button : public box
{
    public:
        button(
            text* text_obj,
            Color bg_color,
            Rectangle rec,
            int layer,
            Color outline_color = BLACK,
            float outline_size = 2.0f);

        ~button();

        void update() override;
        void draw() override;

        // Checks if the mouse is within the button's rectangle.
        bool is_hovered();

        // Checks if 'is_hovered()' and the mouse being pressed are both true.
        bool is_pressed();

        void add_trait(button_trait* trait) { m_traits.push_back(trait); }

        string get_text() { return m_text_obj->get_text_str(); }

        text* get_text_obj() { return m_text_obj; }

    private:
        // The pointer to the text object of the button. The button handles updating and
        // drawing it's child text within it's own 'update()' and 'draw()' methods.
        text* m_text_obj;

        // What color the text object's text should be when not hovered.
        Color m_default_text_color;

        // What color the text object's text should be drawn in the current frame.
        Color m_current_text_color;

        // What color the button's background should be when not hovered.
        Color m_default_bg_color;

        // The storage container to hold all active traits attached to the button.
        vector<button_trait*> m_traits;

        // The factor by which the button is brightened when hovered.
        static constexpr float m_brighten_factor = 2.0f;

        // Return a brightened version of a color, depending on m_brighten_factor.
        Color brighten_color(Color color);
};

} // NAMESPACE ENGINE.
