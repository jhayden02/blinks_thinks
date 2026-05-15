/***********************************************************************************************
* 
*   shader_manager.cpp - The library for the shader manager module of game object.
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
#include "shader_manager.hpp"

// Standard library.
#include <algorithm>

using engine::game;
using engine::shader_manager;

shader_manager::shader_manager()
{
    m_target_a = LoadRenderTexture(game::get_w(), game::get_h());
    m_target_b = LoadRenderTexture(game::get_w(), game::get_h());

    m_shaders.emplace("blur", LoadShader(0, "res/shaders/blur.frag"));
    m_shaders.emplace("vignette", LoadShader(0, "res/shaders/vignette.frag"));

    int resolution_loc = GetShaderLocation(m_shaders.at("vignette"), "resolution");
    float resolution[2] = {static_cast<float>(game::get_w()), static_cast<float>(game::get_h())};
    SetShaderValue(m_shaders.at("vignette"), resolution_loc, resolution, SHADER_UNIFORM_VEC2);

    m_in_texture_mode = false;
}

shader_manager::~shader_manager()
{
    UnloadRenderTexture(m_target_a);
    UnloadRenderTexture(m_target_b);

    for (const auto& [name, shader] : m_shaders) {
        UnloadShader(shader);
    }
}

void engine::shader_manager::begin()
{
    BeginTextureMode(m_target_a);
    m_in_texture_mode = true;
}

void engine::shader_manager::append(string shader_name)
{
    m_shader_queue.push_back(shader_name);
}

void engine::shader_manager::process()
{
    if (m_in_texture_mode) {
        EndTextureMode();
        m_in_texture_mode = false;
    }

    if (m_shader_queue.empty()) {
        DrawTextureRec(
            m_target_a.texture,
            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(m_target_a.texture.width),
                static_cast<float>(-m_target_a.texture.height)
            },
            Vector2{0.0f, 0.0f},
            WHITE
        );
        return;
    }

    RenderTexture2D* source = &m_target_a;
    RenderTexture2D* dest = &m_target_b;

    for (size_t i = 0; i < m_shader_queue.size(); ++i) {
        bool is_last = (i == m_shader_queue.size() - 1);

        if (!is_last) {
            BeginTextureMode(*dest);
        }

        BeginShaderMode(m_shaders.at(m_shader_queue[i]));

        DrawTextureRec(
            source->texture,
            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(source->texture.width),
                static_cast<float>(-source->texture.height)
            },
            Vector2{0.0f, 0.0f},
            WHITE
        );

        EndShaderMode();

        if (!is_last) {
            EndTextureMode();
            std::swap(source, dest);
        }
    }

    m_shader_queue.clear();
}
