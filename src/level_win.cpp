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
#include "level_win.hpp"
#include "level_title.hpp"
#include "game.hpp"

using engine::rotation_sin;

level_win::level_win()
{
    m_game.audio->set_next_music("win_theme", false);

    this->m_title_screen_button = add_ui_button("Title");
    add_simple_text(
        "You win!",
        100,
        GREEN,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    )
    ->add_trait(new rotation_sin(5.0f, 2.5f));
}

void level_win::update()
{
    level::update();

    if (m_title_screen_button->is_pressed()) {
        m_game.set_next_level(new level_title());
    }
}
