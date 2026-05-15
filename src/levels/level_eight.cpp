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
#include "level_eight.hpp"
#include "level_nine.hpp"
#include "level_lose.hpp"
#include "game.hpp"

using engine::rotation_sin;
using engine::grows_when_hovered;

// Standard library.
#include <limits.h>

using std::to_string;

vector<int> level_eight::get_fib_seq(int num_one, int num_two)
{
    vector<int> sequence(m_fib_seq_len);
    int num_three;
    for (size_t i = 0; i != m_fib_seq_len; ++i) {
        sequence[i] = num_one;
        GAME_ASSERT(
            num_one + num_two < INT_MAX,
            "INT_MAX was exceeded when computing a member of the fibbonacci sequence."
        );
        num_three = num_one + num_two;
        num_one = num_two;
        num_two = num_three;
    }
    return sequence;
}

level_eight::level_eight()
{
    add_simple_text(
        "level",
        80,
        ORANGE,
        {m_game.get_cw() - 30, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "What number comes next?",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    int const fib_seq_num_one = m_game.get_random_value(1, 4);
    int const fib_seq_num_two = m_game.get_random_value(5, 9);
    vector<int> const fib_seq = get_fib_seq(fib_seq_num_one, fib_seq_num_two);

    string fib_seq_question_str;
    for (auto it = fib_seq.begin(); it != fib_seq.end() - 1; ++it) {
        fib_seq_question_str += to_string(*it);
        fib_seq_question_str += ", ";
    }
    fib_seq_question_str += "?";

    add_simple_text(
        fib_seq_question_str,
        40,
        YELLOW,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    // All possible Fibbonacci sequences derived from 1-4 and 5-9 have a 2-digit 5th number.
    // This is why we can hard code this range of incorrect, but correct-digit-length values.
    int const min_incorrect = 10;
    int const max_incorrect = 99;
    int const correct_value = fib_seq.back();

    vector<button*> grouped = add_grouped_buttons(
        m_choice_count - 2,
        min_incorrect,
        max_incorrect,
        {correct_value}
    );

    grouped.push_back(add_level_num_button(8));

    m_correct_button = add_text_button(
        to_string(correct_value),
        80,
        m_game.get_random_color(),
        {0, 0}
    );
    add_to_group(m_correct_button);
    grouped.push_back(m_correct_button);

    for (button* btn : grouped) {
        btn->add_trait(new grows_when_hovered());
    }
}

void level_eight::update()
{
    scene::update();

    if (m_correct_button->is_pressed()) {
        m_game.set_next_scene(new level_nine());
    }
    else {
        for (button* btn : get_buttons()) {
            if (btn->is_pressed()) {
                m_game.set_next_scene(new level_lose());
            }
        }
    }
}
