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
using engine::random_manager;
using engine::scene_manager;

game::game()
{
    this->m_button_in_hand = nullptr;

    InitWindow(m_w, m_h, m_game_name);
    SetWindowSize(m_w, m_h);
    SetTargetFPS(m_frame_rate);
    SetExitKey(KEY_NULL);
    SetTraceLogLevel(LOG_DEBUG);

    // Initialize managers after window creation.
    audio = new audio_manager();
    shaders = new shader_manager();
    random = new random_manager();
    scenes = new scene_manager();
}

game::~game()
{
    delete scenes;
    delete random;
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
        scenes->update();
        audio->update();

        // ---------------------------------------------------------------------------------- //
        //                                       Draw.                                        //
        // ---------------------------------------------------------------------------------- //
        BeginDrawing();
        ClearBackground(RAYWHITE);

        scenes->draw();

        EndDrawing();
    }
}

bool game::mouse_in_canvas() {
    #ifdef PLATFORM_WEB
    return web::mouse_in_canvas();
    #else
    return IsWindowFocused();
    #endif
}
