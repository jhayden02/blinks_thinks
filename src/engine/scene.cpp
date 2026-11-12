/***********************************************************************************************
*
*   scene.cpp - The library for creating and drawing formatted text.
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
#include "scene.hpp"
#include "text.hpp"
#include "button.hpp"
#include "overlay.hpp"

// Levels.
#include "intro_raylib.hpp"
#include "intro_self_credit.hpp"
#include "level_title.hpp"
#include "intro_section_01.hpp"
#include "level_01.hpp"
#include "level_02.hpp"
#include "level_03.hpp"
#include "level_04.hpp"
#include "level_05.hpp"
#include "level_06.hpp"
#include "level_07.hpp"
#include "level_08.hpp"
#include "level_09.hpp"
#include "level_10.hpp"
#include "level_win.hpp"
#include "level_lose.hpp"

using engine::game;
using engine::scene;
using engine::scene_manager;
using engine::text;
using engine::button;
using engine::overlay;

// ------------------------------------------------------------------------------------------ //
//                                           scene                                            //
// ------------------------------------------------------------------------------------------ //
const Vector2 scene::m_grouped_buttons_center = {game::get_x_center(), game::get_y_center() + 75.0f};

scene::scene()
    :
    m_game(game::get_instance()),
    m_entities{},
    m_grouped_buttons{}
{
    this->m_button_in_hand = nullptr;
    add_entity(
        new background(
            { 145, 145, 145, 255 },
            { 180, 180, 180, 255 },
            50
        )
    );
}

scene::~scene()
{
    for (entity* ent : m_entities) {
        delete ent;
    }
    m_entities.clear();
    m_grouped_buttons.clear();
}

void scene::update()
{
    for (const auto& ent : m_entities) {
        ent->update();
    }
}

void scene::draw()
{
    for (const auto& ent : m_entities) {
        ent->draw();
    }
}

// Create a simple text with a black outline.
text* scene::add_simple_text(
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
button* scene::add_ui_button(string text_str)
{
    constexpr Vector2 position = {
        game::get_x_center(),
        game::get_y_center() + 100
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
    btn->add_trait(new sfx_press(m_game.audio->get_sound_effect("click")));
    add_entity(btn);
    return btn;
}

// Make clickable text by creating an invisible button in the shape and size of the text.
button* scene::add_text_button(string text_str, int font_size, Color text_color, Vector2 position)
{
    constexpr int layer = 1;
    text* const text_obj = new text(text_str, font_size, text_color, position, layer);
    const Rectangle btn_rec = text_obj->get_rec();
    button* const btn = new button(text_obj, {0, 0, 0, 0}, btn_rec, layer, {0, 0, 0, 0}, 0);
    btn->add_trait(new sfx_press(m_game.audio->get_sound_effect("click")));
    add_entity(btn);
    return btn;
}

button* scene::add_level_num_button(int num)
{
    const Vector2 position = {m_game.get_x_center() + 118.0f, m_game.get_y_center() - 250.0f};
    return add_text_button(std::to_string(num), 80, ORANGE, position);
}

vector<button*> scene::add_grouped_buttons(
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

    const vector<int> values = m_game.random->get_int_sequence(
        count, min_value, max_value, excluded_values
    );
    const vector<Color> colors = m_game.random->get_color_sequence(count);

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

void scene::add_to_group(button* btn)
{
    GAME_ASSERT(btn != nullptr, "add_to_group called with nullptr.");
    GAME_ASSERT(
        m_grouped_buttons.size() + 1 <= m_grouped_buttons_max,
        "add_to_group: adding a button would exceed m_grouped_buttons_max."
    );
    m_grouped_buttons.push_back(btn);
    rearrange_group();
}

void scene::rearrange_group()
{
    const size_t n = m_grouped_buttons.size();
    if (n == 0) {
        return;
    }
    GAME_ASSERT(
        n <= m_grouped_buttons_max,
        "rearrange_group: m_grouped_buttons size exceeds m_grouped_buttons_max."
    );

    // A single row of N buttons centered horizontally on m_grouped_buttons_center.
    const float width = static_cast<float>(n - 1) * m_grouped_buttons_spacing;
    const float start = m_grouped_buttons_center.x - width / 2.0f;

    for (size_t i = 0; i < n; ++i) {
        const Vector2 position = {
            start + static_cast<float>(i) * m_grouped_buttons_spacing,
            m_grouped_buttons_center.y
        };
        m_grouped_buttons[i]->set_position(position);
    }
}

// ------------------------------------------------------------------------------------------ //
//                                       scene_manager                                        //
// ------------------------------------------------------------------------------------------ //
scene_manager::scene_manager()
    :
    m_advance{
        []() -> scene* { return new intro_raylib(); },
        []() -> scene* { return new intro_self_credit(); },
        []() -> scene* { return new level_title(); },
        []() -> scene* { return new intro_section_01(); },
        []() -> scene* { return new level_01(); },
        []() -> scene* { return new level_02(); },
        []() -> scene* { return new level_03(); },
        []() -> scene* { return new level_04(); },
        []() -> scene* { return new level_05(); },
        []() -> scene* { return new level_06(); },
        []() -> scene* { return new level_07(); },
        []() -> scene* { return new level_08(); },
        []() -> scene* { return new level_09(); },
        []() -> scene* { return new level_10(); },
        []() -> scene* { return new level_win(); }
    },
    m_lose([]() -> scene* { return new level_lose(); }),
    m_index(0),
    m_title_index(2),
    m_restart_point_index(3),
    m_current_scene(nullptr),
    m_next_scene(nullptr)
{
}

scene_manager::~scene_manager()
{
    delete m_current_scene;
    delete m_next_scene;
}

void scene_manager::update()
{
    // Seed the first scene on the initial tick to avoid recursing back into
    // game::get_instance() during the game constructor.
    if (m_current_scene == nullptr && m_next_scene == nullptr) {
        m_next_scene = m_advance[m_index]();
    }
    if (m_next_scene != nullptr) {
        delete m_current_scene;
        m_current_scene = m_next_scene;
        m_next_scene = nullptr;
    }
    if (m_current_scene != nullptr) {
        m_current_scene->update();
    }
}

void scene_manager::draw()
{
    if (m_current_scene != nullptr) {
        m_current_scene->draw();
    }
}

void scene_manager::advance()
{
    ++m_index;
    m_next_scene = m_advance[m_index]();
}

void scene_manager::lose()
{
    m_next_scene = m_lose();
}

void scene_manager::goto_title()
{
    m_index = m_title_index;
    m_next_scene = m_advance[m_index]();
}

void scene_manager::goto_restart_point()
{
    m_index = m_restart_point_index;
    m_next_scene = m_advance[m_index]();
}
