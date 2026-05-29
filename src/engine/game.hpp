/***********************************************************************************************
*
*   game.hpp - The library for creating the monolithic game managing object.
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

// Source.
#include "scene_manager.hpp"
#include "shader_manager.hpp"
#include "audio_manager.hpp"
#include "random_manager.hpp"

// Standard library.
#include <string>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <cmath>

#ifndef NDEBUG
    #define GAME_ASSERT(cond, msg) \
        do { \
            if (!(cond)) { \
                std::cerr << "[GAME_ASSERT] Assertion failed: (" #cond ") in" \
                          << " (" << __FILE__ << ":" << __LINE__ << ")\n" \
                          << "[GAME_ASSERT] Message: " << msg << std::endl; \
                std::abort(); \
            } \
        } while (0)
#else
    #define GAME_ASSERT(cond, msg) ((void)0)
#endif

using std::string;
using std::unordered_map;

namespace engine
{

class button;

class game
{
    public:
        static game& get_instance()
        {
            static game instance;
            return instance;
        }

        game(const game&) = delete;
        game& operator=(const game&) = delete;
        game(game&&) = delete;
        game& operator=(game&&) = delete;

        audio_manager* audio;
        shader_manager* shaders;
        random_manager* random;
        scene_manager* scenes;

        void run();

        bool mouse_in_canvas();

        static bool float_equals(float a, float b, float epsilon = 0.0001f)
        {
            return std::fabs(a - b) < epsilon;
        }

        static const string get_game_version() { return m_game_version; }
        static const string get_game_name() { return m_game_name; }

        static constexpr int get_w() { return m_w; }
        static constexpr int get_h() { return m_h; }
        static constexpr float get_cw() { return m_cw; }
        static constexpr float get_ch() { return m_ch; }

        static constexpr size_t get_frame_rate() { return m_frame_rate; }

        button* get_button_in_hand() { return m_button_in_hand; }
        void set_button_in_hand(button* btn) { m_button_in_hand = btn; }

    private:
        game();
        ~game();

        static constexpr const char* m_game_version = "0.0.9";
        static constexpr const char* m_game_name = "Blink's Thinks";

        static constexpr int m_w = 900;
        static constexpr int m_h = 600;
        static constexpr float m_cw = m_w / 2.0f;
        static constexpr float m_ch = m_h / 2.0f;
        static constexpr size_t m_frame_rate = 60;

        button* m_button_in_hand;
};

} // NAMESPACE ENGINE.
