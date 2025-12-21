/***********************************************************************************************
* 
*   game.hpp - The library for creating the monolithic game managing object.
*
*   Copyright (c) 2025 Josh Hayden (@BlinkDynamo)
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
#include "level.hpp"

using engine::level;

// Standard library.
#include <string>
#include <unordered_map>
#include <random>
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
    class game
    {
        // Music mixer class. Handles all music throughout the game.
        private:
            class audio_manager
            {
                public:
                    static audio_manager& get_instance() {
                        static audio_manager instance;
                        return instance;
                    }

                    void update();

                    Sound get_sound_effect(string sound_name) { return m_sound_effects.at(sound_name); }
                    void set_next_music(string track_name, bool looping = true);
                    void shift_pitch(float pitch);

                private:
                    audio_manager();
                    ~audio_manager();

                    // Prevent copy, copy assignment, move, and move assignment.
                    audio_manager(const audio_manager&) = delete;
                    audio_manager& operator=(const audio_manager&) = delete;
                    audio_manager(audio_manager&&) = delete;
                    audio_manager& operator=(audio_manager&&) = delete;

                    // All music tracks loaded on construction and referenced by their track names.
                    unordered_map<string, Music> m_music_tracks;

                    // All sounds effects loaded on construction and referenced by their sound names.
                    unordered_map<string, Sound> m_sound_effects;

                    static constexpr int m_frame_dur_mix = 90;
                    static constexpr int m_frame_dur_pitch = 60;

                    Music* m_current_music;     // The music currently being played.
                    Music* m_next_music;        // The song that should be mixed in.
                    
                    float m_current_pitch;      // The current pitch of the music.
                    float m_next_pitch;         // The pitch that should be shifted to.

                    int m_frame_count_mix;      // The current frame count of the mixing process.
                    int m_frame_count_pitch;    // The current frame in the pitch shifting process.

                    bool m_mixing;              // If two songs are being mixed currently.
                    bool m_shifting;            // If the pitch is being shifted to something.
            };

        public:
            static game& get_instance() {
                static game instance;
                return instance;
            }
 
            // Prevent copy, copy assignment, move, and move assignment.
            game(const game&) = delete;
            game& operator=(const game&) = delete;
            game(game&&) = delete;
            game& operator=(game&&) = delete;

            audio_manager& audio;

            // Methods.
            void run();

            // Returns a random int from a defined range inclusively.
            int get_random_value(int min, int max);

            // Returns a vector of unique values within a given range. Optionally exclude a vector of values.
            vector<int> get_random_sequence(size_t count, int min, int max, vector<int> exclude = {});

            // Returns a random Raylib-defined color from an array of colors that brighten well.
            Color get_random_color();

            // Returns a vector of randomized, unique, Raylib-defined colors of size 'count'.
            vector<Color> get_random_color_sequence(size_t count);

            // Checks if the cursor is inside the canvas.
            bool mouse_in_canvas();

            static bool float_equals(float a, float b, float epsilon = 0.0001f) { return std::fabs(a - b) < epsilon; }

            // Getters and setters.
            static const string get_game_version() { return m_game_version; }
            static const string get_game_name() { return m_game_name; }

            static constexpr int get_w() { return m_w; } 
            static constexpr int get_h() { return m_h; }
            static constexpr float get_cw() { return m_cw; }
            static constexpr float get_ch() { return m_ch; }

            static constexpr size_t get_frame_rate() { return m_frame_rate; }

            level* get_current_level() { return m_current_level; }
            void set_next_level(level* next_level) { m_next_level = next_level; }

            button* get_button_in_hand() { return m_button_in_hand; }
            void set_button_in_hand(button* btn) { m_button_in_hand = btn; }            

        private:
            game();
            ~game();

            // Version.
            static constexpr const char* m_game_version = "0.0.9";
            static constexpr const char* m_game_name = "Blink's Thinks";

            // Resolution and framerate.
            static constexpr int m_w = 900;
            static constexpr int m_h = 600;
            static constexpr float m_cw = m_w / 2.0f;
            static constexpr float m_ch = m_h / 2.0f;
            static constexpr size_t m_frame_rate = 60;

            // The current level being updated, drawn, and interacted with.
            level* m_current_level;
            level* m_next_level;

            // The button currently being grabbed by the user.
            button* m_button_in_hand;

            // The random number engine for the game instance.
            std::default_random_engine m_random_generator;

            // RNG specific constants.
            inline static const vector<Color> m_bright_colors = {GOLD, ORANGE, PINK, RED, LIME, SKYBLUE, PURPLE, VIOLET};
    };
}
