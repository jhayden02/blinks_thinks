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
#include "level_02.hpp"
#include "game.hpp"

using engine::rotation_sin;
using engine::grows_when_hovered;

// Standard library.
#include <algorithm>

level_02::level_02()
{
    add_simple_text(
        "level",
        80,
        ORANGE,
        {m_game.get_cw() - 30, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "What is the smallest number?",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    m_choices = add_grouped_buttons(m_choice_count - 1, m_min_choice, m_max_choice, {2});
    m_choices.push_back(add_level_num_button(2));

    for (button* btn : m_choices) {
        btn->add_trait(new grows_when_hovered());
    }

    m_correct_button = *std::min_element(
        m_choices.begin(),
        m_choices.end(),
        [](button* a, button* b) {
            return stoi(a->get_text()) < stoi(b->get_text());
        }
    );
}

void level_02::update()
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
