/***********************************************************************************************
*
*   scene_manager.hpp - Scene flow management for the game engine.
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

#pragma once

#include <functional>
#include <vector>
#include <cstddef>

using std::function;
using std::vector;
using std::size_t;

namespace engine
{

class scene;

class scene_manager
{
    public:
        scene_manager();
        ~scene_manager();

        scene_manager(const scene_manager&) = delete;
        scene_manager& operator=(const scene_manager&) = delete;
        scene_manager(scene_manager&&) = delete;
        scene_manager& operator=(scene_manager&&) = delete;

        // Performs the queued scene swap and ticks the current scene.
        void update();

        // Draws the current scene.
        void draw();

        // Advances to the next scene in m_advance.
        void advance();

        // Queues the lose scene without changing the advance index.
        void lose();

        // Jumps to the title scene in m_advance.
        void goto_title();

        // Jumps to the restart-point scene in m_advance.
        void goto_restart_point();

    private:
        vector<function<scene*()>> m_advance;
        function<scene*()> m_lose;
        size_t m_index;
        size_t m_title_index;
        size_t m_restart_point_index;
        scene* m_current_scene;
        scene* m_next_scene;
};

} // NAMESPACE ENGINE.
