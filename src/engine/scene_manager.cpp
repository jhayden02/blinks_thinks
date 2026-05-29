/***********************************************************************************************
*
*   scene_manager.cpp - Scene flow management for the game engine.
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
#include "scene_manager.hpp"
#include "scene.hpp"
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

using engine::scene;
using engine::scene_manager;

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
