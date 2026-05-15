/***********************************************************************************************
* 
*   game.cpp - The library for creating the monolithic game managing object.
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

// Standard library.
#include <unordered_set>
#include <algorithm>

#ifdef PLATFORM_WEB
#include <emscripten.h>
namespace web
{
    EM_JS(int, mouse_in_canvas, (), { return mouse_in_canvas_flag ? 1 : 0; } );
}
#endif

using engine::game;
using engine::audio_manager;
using engine::shader_manager;

game::game()
{
    std::random_device random_generator_seed;
    m_random_generator.seed(random_generator_seed());

    this->m_button_in_hand = nullptr;

    InitWindow(m_w, m_h, m_game_name);
    SetWindowSize(m_w, m_h);
    SetTargetFPS(m_frame_rate);
    SetExitKey(KEY_NULL);
    SetTraceLogLevel(LOG_DEBUG);

    // Initialize managers after window creation.
    audio = new audio_manager();
    shaders = new shader_manager();
}

game::~game()
{
    delete shaders;
    delete audio;
    CloseWindow();
}

void game::run()
{
    while (!WindowShouldClose())
    {
        // ---------------------------------------------------------------------------------- //
        //                                      Update.                                       //
        // ---------------------------------------------------------------------------------- //
        if (m_next_level != nullptr) {
            if (m_current_level != nullptr) {
                delete m_current_level;
            }
            m_current_level = m_next_level;
            m_next_level = nullptr;
        }
        if (m_current_level != nullptr) {
            m_current_level->update();
        }

        audio->update();

        // ---------------------------------------------------------------------------------- //
        //                                       Draw.                                        //
        // ---------------------------------------------------------------------------------- //
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (m_current_level != nullptr) {
            m_current_level->draw();
        }

        EndDrawing();
    }
}

int game::get_random_value(int min, int max)
{
    GAME_ASSERT(max - min > 0, "Invalid range supplied.");
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(m_random_generator);
}

vector<int> game::get_random_sequence(size_t count, int min, int max, vector<int> exclude)
{
    GAME_ASSERT(
        [&]{
            const size_t range_size = max - min + 1;
            return count <= range_size;
        }(),
        "Requested more unique numbers than available range."
    ); 

    std::unordered_set<int> exclusion_set(exclude.begin(), exclude.end());

    vector<int> pool;
    for (int num = min; num <= max; ++num) {
        if (exclusion_set.find(num) == exclusion_set.end()) {
            pool.push_back(num);
        }
    } 

    std::shuffle(pool.begin(), pool.end(), m_random_generator);
    return {pool.begin(), pool.begin() + static_cast<long>(count)};
}

Color game::get_random_color()
{
    return m_bright_colors.at(get_random_value(0, m_bright_colors.size() - 1));
}

vector<Color> game::get_random_color_sequence(size_t count)
{
    GAME_ASSERT(count <= m_bright_colors.size() ,"Requested more unique colors than available maximum."); 

    // Copy game defined constant vector, shuffle it, then return the count asked for.
    vector<Color> palate = m_bright_colors;
    std::shuffle(palate.begin(), palate.end(), m_random_generator);
    return {palate.begin(), palate.begin() + static_cast<long>(count)};
}

bool game::mouse_in_canvas() {
    #ifdef PLATFORM_WEB
    return web::mouse_in_canvas();
    #else
    return IsWindowFocused();
    #endif
}
