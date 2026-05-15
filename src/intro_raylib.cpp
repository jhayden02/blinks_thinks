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
#include "intro_raylib.hpp"
#include "intro_self_credit.hpp"
#include "level_title.hpp"
#include "game.hpp"

intro_raylib::intro_raylib()
{
    this->m_animation = add_entity(new anim_raylib());
    m_game.audio->set_next_music("title_theme");
}

void intro_raylib::update()
{
    level::update();

    if (m_animation->is_finished()) {
        m_game.set_next_level(new intro_self_credit());
    }
    else if (IsKeyPressed(KEY_ENTER)) {
        m_game.set_next_level(new level_title());
    }
}
