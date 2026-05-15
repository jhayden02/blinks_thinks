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
#include "level_10.hpp"
#include "level_win.hpp"
#include "level_lose.hpp"
#include "game.hpp"

using engine::box;
using engine::rotation_sin;
using engine::grows_when_hovered;
using engine::grabbable;

// Standard library.
#include <algorithm>

level_10::level_10()
{
    add_simple_text(
        "level",
        80,
        ORANGE,
        {m_game.get_cw() - 30, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "Put the largest number in the box",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_trait(new rotation_sin(4.0f, 1.5f));

    Vector2 const submit_box_position = {m_game.get_cw(), m_game.get_ch() - 50};
    this->m_submit_box = add_entity(
        new button(
            new text("", 80, BLACK, submit_box_position, 0, {0, 0, 0, 0}, 0.0f),
            WHITE,
            {submit_box_position.x - 75.0f, submit_box_position.y - 50.0f, 150.0f, 100.0f},
            0
        )
    );
    m_submit_box->add_trait(new grabbable());

    this->m_submit_button = add_ui_button("Submit");
	m_submit_button->set_position(
		{m_submit_button->get_position().x, m_submit_button->get_position().y + 100}
	);

    vector<button*> grouped = add_grouped_buttons(m_choice_count - 1, m_min_choice, m_max_choice, {10});
    grouped.push_back(add_level_num_button(10));

    sort(
        grouped.begin(),
        grouped.end(),
        [](button* a, button* b) {
            return stoi(a->get_text()) > stoi(b->get_text());
        }
    );

    m_holdable_number = grouped[m_game.get_random_value(0, grouped.size() - 1)];
    m_holdable_number->add_trait(new grows_when_hovered());
    m_holdable_number->add_trait(new grabbable());

    // The largest creatable number always includes the level number as the second piece.
    // If the level number is holdable, you move it to the right of the second largest
    // number. If it's not holdable, you move the holdable number to the left of it.
    if (m_holdable_number->get_text() == "10") {
        m_correct_button_layout.push_back(grouped[1]);
        m_correct_button_layout.push_back(grouped[0]);
    } else {
        m_correct_button_layout.push_back(m_holdable_number);
        m_correct_button_layout.push_back(grouped[0]);
    }

    // Freeze every non-holdable button with an ice-cube overlay to show that they cannot be moved.
    vector<button*> iced = grouped;
    for (button* btn : iced) {
        if (btn == m_holdable_number) {
            continue;
        }
        int const ice_cube_padding = 32;
        Vector2 const ice_cube_size = {
            btn->get_rectangle().width + ice_cube_padding,
            btn->get_rectangle().height + ice_cube_padding
        };
        Vector2 ice_cube_position = btn->get_position();
        ice_cube_position.x += 4;   // Accomodate for the shadow offset of the text.
        add_entity(
            new box(
                {255, 255, 255, 75},
                {75, 150, 255, 75},
                ice_cube_size,
                4,
                ice_cube_position,
                1
            )
        );
    }
}

void level_10::update()
{
    scene::update();

    //
    // On submission, add every button inside of the submission box to a vector, then organize
    // their text object's text strings in the same order that they exist spatially. Save this
    // created number to a string and check if it matches 'm_correct_number'.
    //
    if (m_submit_button->is_pressed()) {

        vector<button*> numbers_in_box; // For buttons that are inside of the submission box.
        numbers_in_box.reserve(m_max_submit_box_numbers);

        for (button* btn : get_buttons()) {
            if (btn == m_submit_button || btn == m_submit_box) {
                continue;
            }
            if (CheckCollisionRecs(btn->get_rectangle(), m_submit_box->get_rectangle())) {
                vector<button*>::iterator it = numbers_in_box.begin();
                while (it != numbers_in_box.end() && (*it)->get_position().x <= btn->get_position().x) {
                    ++it;
                }
                numbers_in_box.insert(it, btn);
            }
        }

        if (numbers_in_box == m_correct_button_layout) {
            m_game.set_next_scene(new level_win());
        }
        else {
            m_game.set_next_scene(new level_lose());
        }
    }
}
