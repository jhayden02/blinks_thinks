/***********************************************************************************************
*
*   level.cpp - The library for creating and drawing formatted text.
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
#include "game.hpp"
#include "level.hpp"
#include "text.hpp"
#include "button.hpp"
#include "overlay.hpp"

using engine::game;
using engine::level;
using engine::text;
using engine::button;
using engine::overlay;

const Vector2 level::m_grouped_buttons_center = {game::get_cw(), game::get_ch() + 75.0f};

level::level()
    :
    m_game(game::get_instance()),
    m_entities{},
    m_buttons{},
    m_grouped_buttons{}
{
    add_entity(
        new background(
            { 145, 145, 145, 255 },
            { 180, 180, 180, 255 },
            50
        )
    );
}

level::~level()
{
    for (entity* ent : m_entities) {
        delete ent;
    }
    m_entities.clear();
    m_buttons.clear();
    m_grouped_buttons.clear();
}

void level::update()
{
    for (const auto& ent : m_entities) {
        ent->update();
    }
}

void level::draw()
{
    for (const auto& ent : m_entities) {
        ent->draw();
    }
}

// Create a simple text with a black outline.
text* level::add_simple_text(
	string text_str,
	float font_size,
	Color text_color,
    Vector2 position,
	int layer
) {
    text* const text_obj = new text(text_str, font_size, text_color, position, layer);
    add_entity(text_obj);
    return text_obj;
}

// Make a clickable UI button with dynamic text and background color at a fixed location.
button* level::add_ui_button(string text_str)
{
    constexpr Vector2 position = {
        game::get_cw(),
        game::get_ch() + 100
    };
    constexpr int layer = 1;
    text* const text_obj = new text(text_str, 40, WHITE, position, layer, BLACK, 2.0f);
    button* const btn = new button(
        text_obj,
        DARKGRAY,
        {position.x - 90.0f, position.y - 30.0f, 180.0f, 60.0f},
        layer
    );
    btn->add_trait(new grows_when_hovered());
    btn->set_sfx_press(m_game.audio->get_sound_effect("click"));
    add_entity(btn);
    return btn;
}

// Make clickable text by creating an invisible button in the shape and size of the text.
button* level::add_text_button(string text_str, int font_size, Color text_color, Vector2 position)
{
    constexpr int layer = 1;
    text* const text_obj = new text(text_str, font_size, text_color, position, layer);
    Rectangle const text_rec = text_obj->get_rec();
    Rectangle const btn_rec = {
        position.x - text_rec.width / 2.0f,
        position.y - text_rec.height / 2.0f,
        text_rec.width,
        text_rec.height
    };
    button* const btn = new button(text_obj, {0, 0, 0, 0}, btn_rec, layer, {0, 0, 0, 0}, 0);
    btn->set_sfx_press(m_game.audio->get_sound_effect("grab"));
    add_entity(btn);
    return btn;
}

button* level::add_level_num_button(int num)
{
    Vector2 const position = {m_game.get_cw() + 118.0f, m_game.get_ch() - 250.0f};
    return add_text_button(std::to_string(num), 80, ORANGE, position);
}

vector<button*> level::add_grouped_buttons(
    size_t count,
    int min_value,
    int max_value,
    const vector<int>& excluded_values
) {
    GAME_ASSERT(count > 0, "add_grouped_buttons called with count == 0.");
    GAME_ASSERT(
        min_value <= max_value,
        "add_grouped_buttons called with min_value > max_value."
    );
    GAME_ASSERT(
        static_cast<size_t>(max_value - min_value + 1) > excluded_values.size(),
        "add_grouped_buttons: excluded_values covers or exceeds the entire value range."
    );
    GAME_ASSERT(
        static_cast<size_t>(max_value - min_value + 1) - excluded_values.size() >= count,
        "add_grouped_buttons: not enough unique values in range to satisfy count."
    );
    GAME_ASSERT(
        m_grouped_buttons.size() + count <= m_grouped_buttons_max,
        "add_grouped_buttons: adding count would exceed m_grouped_buttons_max."
    );

    vector<int> const values = m_game.get_random_sequence(
        count, min_value, max_value, excluded_values
    );
    vector<Color> const colors = m_game.get_random_color_sequence(count);

    vector<button*> created;
    created.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        button* const btn = add_text_button(std::to_string(values[i]), 80, colors[i], {0, 0});
        m_grouped_buttons.push_back(btn);
        created.push_back(btn);
    }

    rearrange_group();
    return created;
}

void level::add_to_group(button* btn)
{
    GAME_ASSERT(btn != nullptr, "add_to_group called with nullptr.");
    GAME_ASSERT(
        m_grouped_buttons.size() + 1 <= m_grouped_buttons_max,
        "add_to_group: adding a button would exceed m_grouped_buttons_max."
    );
    m_grouped_buttons.push_back(btn);
    rearrange_group();
}

void level::rearrange_group()
{
    size_t const n = m_grouped_buttons.size();
    if (n == 0) {
        return;
    }
    GAME_ASSERT(
        n <= m_grouped_buttons_max,
        "rearrange_group: m_grouped_buttons size exceeds m_grouped_buttons_max."
    );

    // A single row of N buttons centered horizontally on m_grouped_buttons_center.
    float const first_col_x
        = m_grouped_buttons_center.x
        - (static_cast<float>(n - 1) * m_grouped_buttons_col_spacing / 2.0f);

    for (size_t i = 0; i < n; ++i) {
        Vector2 const position = {
            first_col_x + static_cast<float>(i) * m_grouped_buttons_col_spacing,
            m_grouped_buttons_center.y
        };
        m_grouped_buttons[i]->set_position(position);
    }
}
