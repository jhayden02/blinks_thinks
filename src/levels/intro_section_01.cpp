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
#include "intro_section_01.hpp"
#include "game.hpp"

intro_section_01::intro_section_01()
{
    this->m_frames_counter = 0;

    add_simple_text(
        "Levels 1-10: Numbers",
        60,
        GREEN,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    );

    // Set the music track, and set the pitch make to normal if it's not.
    m_game.audio->set_next_music("no_stopping_now");
    m_game.audio->shift_pitch(1.0f);
}

void intro_section_01::update()
{
    scene::update();

    ++m_frames_counter;

    if (m_frames_counter == 3 * m_game.get_frame_rate()) {
        m_game.scenes->advance();
    }
}
