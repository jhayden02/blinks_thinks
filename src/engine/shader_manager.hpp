/***********************************************************************************************
*
*   shader_manager.hpp - Shader management for the game engine.
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

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

namespace engine
{

class shader_manager
{
    public:
        shader_manager();
        ~shader_manager();

        void begin();
        void append(string shader_name);
        void process();

    private:
        RenderTexture2D m_target_a;
        RenderTexture2D m_target_b;
        unordered_map<string, Shader> m_shaders;
        vector<string> m_shader_queue;
        bool m_in_texture_mode;
};

} // NAMESPACE ENGINE.
