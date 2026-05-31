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
#include "level_03.hpp"
#include "game.hpp"

using engine::rotation_sin;
using engine::grows_when_hovered;

level_03::level_03()
{
    add_simple_text(
        "level",
        80,
        ORANGE,
        {m_game.get_cw() - 30, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "What is the tallest number?",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    m_choices = add_grouped_buttons(m_choice_count - 1, m_min_choice, m_max_choice, {3});
    m_choices.push_back(add_level_num_button(3));

    int const correct_index = m_game.random->get_int(0, m_choice_count - 1);
    for (size_t i = 0; i < m_choices.size(); ++i) {
        if (static_cast<int>(i) == correct_index) {
            m_correct_button = m_choices[i];
            m_correct_button->add_trait(new grows_when_hovered(20, 2.5f));
        }
        else {
            m_choices[i]->add_trait(new grows_when_hovered());
        }
    }
}

void level_03::update()
{
    scene::update();

    if (m_correct_button->is_pressed()) {
        m_game.scenes->advance();
    }
    else {
        for (button* btn : get_entities<button>()) {
            if (btn->is_pressed()) {
                m_game.scenes->lose();
            }
        }
    }
}
