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
#include "level_06.hpp"
#include "game.hpp"

using engine::rotation_sin;
using engine::grows_when_hovered;

level_06::level_06()
{
    this->m_frames_counter = 0;

    this->m_correct_button = add_text_button(
        "3",
        80,
        m_game.random->get_color(),
        {-200, m_game.get_ch() + 85}
    );
    m_correct_button->set_speed({20, 0});

    add_simple_text(
        "level",
        80,
        ORANGE,
        {m_game.get_cw() - 30, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "Click the number 3",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    vector<button*> grouped = add_grouped_buttons(
        m_choice_count - 1,
        m_min_choice,
        m_max_choice,
        {3, 6}
    );
    grouped.push_back(add_level_num_button(6));

    for (button* btn : grouped) {
        btn->add_trait(new grows_when_hovered());
    }
}

void level_06::update()
{
    scene::update();


    if (m_correct_button->get_position().x >= m_game.get_cw() && m_frames_counter < 60) {
        m_correct_button->set_speed({0, 0});
        ++m_frames_counter;
    }

    if (m_frames_counter == 60) {
        m_correct_button->set_speed({20, 0});
    }

    for (button* btn : get_entities<button>()) {
        if (btn->is_pressed()) {
            if (btn == m_correct_button) {
                m_game.scenes->advance();
            }
            else {
                m_game.scenes->lose();
            }
        }
    }

    // Check if the number '3' has gone off of the screen to the right. If so, the player loses.
    if (m_correct_button->get_position().x > m_game.get_w() + 80) {
        m_game.scenes->lose();
    }

}
