/***********************************************************************************************
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
#include "level_four.hpp"
#include "level_five.hpp"
#include "game.hpp"

using engine::rotation_sin;
using engine::grows_when_hovered;

level_four::level_four()
{
    //
    // Main UI elements (level title, directions).
    //
    add_simple_text(
        "level",
        80,
        ORANGE,
        {m_game.get_cw() - 30,
        m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "How much time do you want for level 5?",
        40,
        RAYWHITE,
        {m_game.get_cw(),
        m_game.get_ch() - 150},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    m_choices = add_grouped_buttons(m_choice_count - 1, m_min_choice, m_max_choice, {});
    m_choices.push_back(add_level_num_button(4));

    for (size_t i = 0; i < m_choices.size(); ++i) {
        m_choices[i]->add_trait(new grows_when_hovered());
    }
}

void level_four::update()
{
    scene::update();

    for (button* btn : get_buttons()) {
        if (btn->is_pressed()) {
            string chosen_time = btn->get_text();
            m_game.set_next_scene(new level_five(chosen_time));
        }
    }
}
