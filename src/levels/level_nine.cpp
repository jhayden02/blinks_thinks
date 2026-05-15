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
#include "level_nine.hpp"
#include "level_ten.hpp"
#include "level_lose.hpp"
#include "game.hpp"

using engine::rotation_sin;
using engine::grows_when_hovered;
using engine::grabbable;

// Standard library.
#include <algorithm>

level_nine::level_nine()
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

    this->m_submit_box = add_entity(
        new box(
            BLACK,
            WHITE,
            {300, 100},
            2,
            {m_game.get_cw(), m_game.get_ch() - 50},
            0
        )
    );

    this->m_submit_button = add_ui_button("Submit");
	m_submit_button->set_position(
		{m_submit_button->get_position().x, m_submit_button->get_position().y + 100}
	);

    m_correct_button_layout.reserve(m_choice_count);

    vector<button*> grouped = add_grouped_buttons(m_choice_count - 1, m_min_choice, m_max_choice, {9});
    grouped.push_back(add_level_num_button(9));

    for (button* btn : grouped) {
        btn->add_trait(new grows_when_hovered());
        btn->add_trait(new grabbable());
        m_correct_button_layout.push_back(btn);
    }

    // Sort greatest-to-least so the correct answer reads left-to-right when assembled.
    sort(
        m_correct_button_layout.begin(),
        m_correct_button_layout.end(),
        [](button* a, button* b) {
            return stoi(a->get_text()) > stoi(b->get_text());
        }
    );
}

void level_nine::update()
{
    scene::update();

    //
    // On submission, add every button inside of the submission box to a vector, then organize
    // their text object's text strings in the same order that they exist spatially. Save this
    // created number to a string and check if it matches 'm_correct_number'.
    //
    if (m_submit_button->is_pressed()) {

        vector<button*> numbers_in_box; // For buttons that are inside of the submission box.
        numbers_in_box.reserve(m_choice_count);

        for (button* btn : get_buttons()) {
            if (CheckCollisionRecs(btn->get_rectangle(), m_submit_box->get_rectangle())) {
                vector<button*>::iterator it = numbers_in_box.begin();
                while (it != numbers_in_box.end() && (*it)->get_position().x <= btn->get_position().x) {
                    ++it;
                }
                numbers_in_box.insert(it, btn);
            }
        }

        // Assume true and override if proven false.
        bool answer_was_chosen = true;
        vector<button*>::iterator choice_it = numbers_in_box.begin();
        vector<button*>::iterator answer_it = m_correct_button_layout.begin();

        if (numbers_in_box.size() != m_correct_button_layout.size()) {
            answer_was_chosen = false;
        }

        while (choice_it != numbers_in_box.end() && answer_it != m_correct_button_layout.end()) {
            if ((*choice_it)->get_text() != (*answer_it)->get_text()) {
                answer_was_chosen = false;
            }
            ++choice_it;
            ++answer_it;
        }

        if (answer_was_chosen) {
            m_game.set_next_scene(new level_ten());
        }
        else {
            m_game.set_next_scene(new level_lose());
        }
    }
}
