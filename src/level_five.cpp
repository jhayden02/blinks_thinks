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
#include "level_five.hpp"
#include "level_six.hpp"
#include "level_lose.hpp"
#include "game.hpp"

using engine::rotation_sin;

using std::to_string;

level_five::level_five(string duration)
{
    this->m_frames_counter = 0;
    this->m_duration = duration;
    this->m_timer = add_text_button(
        m_duration,
        80,
        RED,
        {m_game.get_cw(), m_game.get_ch() - 65}
    );

    //
    // Main UI elements (level title, directions, countdown timer).
    //
    add_simple_text(
        "level",
        80,
        ORANGE,
        {m_game.get_cw() - 30, m_game.get_ch() - 250},
        0
    );

    add_level_num_button(5);

    add_simple_text(
        "Don't touch any numbers for",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    add_simple_text(
        "more seconds",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch()},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    //
    // Button creation.
    //
    const string long_num_one   = "762967340328811348866734234450240332396217777462684390";
    const string long_num_two   = "239427620310921174648449330989407894927458570770003111";
    const string long_num_three = "230459256723665565627118580006023666643111673444567710";

    add_text_button(
        "68",
        120,
        RED,
        {m_game.get_cw() - 270, m_game.get_ch() - 500}
    )
    ->set_speed({0, 12});

    add_text_button(
        long_num_one,
        160,
        RED,
        {m_game.get_cw() - 3050, m_game.get_ch() + 250}
    )
    ->set_speed({3, -1});

    add_text_button(
        long_num_two,
        160,
        RED,
        {m_game.get_cw() + 3050, m_game.get_ch() - 230}
    )
    ->set_speed({-3, 1});

    add_text_button(
        "8043001",
        120,
        RED,
        {m_game.get_cw() - 200, m_game.get_ch() + 1200}
    )
    ->set_speed({0, -5});

    add_text_button(
        "3",
        120,
        RED,
        {m_game.get_cw() + 600, m_game.get_ch()}
    )
    ->set_speed({-1, 0});

    add_text_button(
        "762",
        160,
        RED,
        {m_game.get_cw() - 1300, m_game.get_ch()}
    )
    ->set_speed({2, 0});

    add_text_button(
        "12",
        120,
        RED,
        {m_game.get_cw() + 1800, m_game.get_ch() - 600}
    )
    ->set_speed({-8, 4});

    add_text_button(
        "5000006",
        120,
        RED,
        {m_game.get_cw() + 200, m_game.get_ch() - 3000}
    )
    ->set_speed({0, 7});

    add_text_button(
        "3078",
        80,
        RED,
        {m_game.get_cw() - 250, m_game.get_ch() - 3100}
    )
    ->set_speed({0, 7});

    add_text_button(
        long_num_three,
        150,
        RED,
        {m_game.get_cw() + 4550, m_game.get_ch() - 150}
    )
    ->set_speed({-4, 0});

    add_text_button(
        "7877878447232634",
        150,
        RED,
        {m_game.get_cw(), m_game.get_ch() + 800}
    )
    ->set_speed({0, -1});

    add_text_button(
        "64",
        120,
        RED,
        {m_game.get_cw() + 3000, m_game.get_ch() + 700}
    )
    ->set_speed({-5, -1});
}

void level_five::update()
{
    level::update();

    bool level_lost = false;

    // update the timer once per 60 frames.
    m_frames_counter++;
    if (m_frames_counter == 60) {
        int duration_as_int = stoi(m_duration);
        if (duration_as_int > 0) {
            m_frames_counter = 0;
            --duration_as_int;

            m_duration = to_string(duration_as_int);
            m_timer->get_text_obj()->set_text_str(m_duration);
        }
        else {
            m_game.set_next_level(new level_six());
        }
    }

    // The player will lose if any button is hovered, or if the window (game) becomes unfocused.
    for (button* btn : get_buttons()) {
        if (btn->is_hovered()) {
            level_lost = true;
        }
    }

    if (!m_game.mouse_in_canvas()) {
        level_lost = true;
    }

    if (level_lost) {
        m_game.set_next_level(new level_lose());
    }
}
