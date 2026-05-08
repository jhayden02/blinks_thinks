/***********************************************************************************************
*
*   audio_manager.hpp - Audio management for the game engine.
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

using std::string;
using std::unordered_map;

namespace engine
{

class audio_manager
{
    public:
        audio_manager();
        ~audio_manager();

        void update();

        Sound* get_sound_effect(string sound_name) { return &m_sound_effects.at(sound_name); }
        void set_next_music(string track_name, bool looping = true);
        void shift_pitch(float pitch);

    private:
        unordered_map<string, Music> m_music_tracks;
        unordered_map<string, Sound> m_sound_effects;

        static constexpr int m_frame_dur_mix = 90;
        static constexpr int m_frame_dur_pitch = 60;

        Music* m_current_music;
        Music* m_next_music;

        float m_current_pitch;
        float m_next_pitch;

        int m_frame_count_mix;
        int m_frame_count_pitch;

        bool m_mixing;
        bool m_shifting;
};

} // NAMESPACE ENGINE.
