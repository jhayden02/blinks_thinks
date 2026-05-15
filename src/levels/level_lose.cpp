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
#include "level_lose.hpp"
#include "intro_section_01.hpp"
#include "game.hpp"

using engine::rotation_sin;

level_lose::level_lose()
{
    this->m_restart_button = add_ui_button("Restart");
    add_simple_text(
        "game over!",
        100,
        RED,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    )
    ->add_trait(new rotation_sin(5.0f, 2.5f));

    // Pitch the music down.
    m_game.audio->shift_pitch(0.8f);
}

void level_lose::update()
{
    scene::update();

    if (m_restart_button->is_pressed()) {
        m_game.set_next_scene(new intro_section_01());
    }
}
