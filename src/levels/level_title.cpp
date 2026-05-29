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
#include "level_title.hpp"
#include "game.hpp"

using engine::game;
using engine::rotation_sin;

level_title::level_title()
{
    this->m_game_title_text.text_obj = add_simple_text(
        "Blink's Thinks",
        100,
        m_game_title_text.text_color,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    );
    this->m_play_button = add_ui_button("Play");
    this->m_game_title_text.text_obj->add_trait(new rotation_sin(5.0f, 2.5f));

    string version_and_build_display_str  = ("v" + game::get_game_version());
    float version_and_build_display_spacing;
    #ifndef NDEBUG
        version_and_build_display_str += " debug";
        version_and_build_display_spacing = 74.0f;
    #else
        version_and_build_display_str += " release";
        version_and_build_display_spacing = 84.0f;
    #endif

    // Display the current version number in the bottom right.
    add_entity(
        new text(
            version_and_build_display_str,
            20,
            RAYWHITE,
            {game::get_w() - version_and_build_display_spacing, 20},
            1000,
            {0, 0, 0, 0},
            0.0f
        )
    );

    m_game.audio->set_next_music("title_theme");
}

void level_title::update()
{
    scene::update();

    if (m_play_button->is_pressed()) {
        m_game.scenes->advance();
    }
}
