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
#include "level_seven.hpp"
#include "level_eight.hpp"
#include "level_lose.hpp"
#include "game.hpp"

using engine::rotation_sin;
using engine::grows_when_hovered;
using engine::grabbable;

level_seven::level_seven()
{
    add_simple_text(
        "level",
        80,
        ORANGE,
        {m_game.get_cw() - 30, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "Feed the hungry number the proper food",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    vector<button*> grouped = add_grouped_buttons(m_choice_count - 2, 1, 8, {7, 9});

    m_button_seven = add_level_num_button(7);
    grouped.push_back(m_button_seven);

    m_button_nine = add_text_button("9", 80, m_game.get_random_color(), {0, 0});
    add_to_group(m_button_nine);
    grouped.push_back(m_button_nine);

    for (button* btn : grouped) {
        btn->add_trait(new grows_when_hovered());
        btn->add_trait(new grabbable());
    }
}

void level_seven::update()
{
    level::update();

    button* button_in_hand = m_game.get_button_in_hand();

    if (button_in_hand != nullptr) {
        for (button* btn : get_buttons()) {
            const bool two_buttons_collided = (
                btn != button_in_hand &&
                CheckCollisionRecs(button_in_hand->get_rectangle(), btn->get_rectangle())
            );

            if (two_buttons_collided) {
                const bool seven_and_nine_collided = (
                    (button_in_hand == m_button_seven && btn == m_button_nine) ||
                    (button_in_hand == m_button_nine && btn == m_button_seven)
                );

                if (seven_and_nine_collided) {
                    m_game.set_next_level(new level_eight());
                }
                else {
                    m_game.set_next_level(new level_lose());
                }
            }
        }
    }
}
