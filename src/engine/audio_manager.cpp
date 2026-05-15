/***********************************************************************************************
* 
*   audio_manager.cpp - The library for creating the audio manager module of the game object.
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
#include "audio_manager.hpp"

using engine::audio_manager;

audio_manager::audio_manager()
{
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(16384);

    m_current_music = nullptr;
    m_next_music = nullptr;
    m_current_pitch = 1.0f;
    m_next_pitch = 1.0f;
    m_frame_count_mix = 0;
    m_frame_count_pitch = 0;
    m_mixing = false;
    m_shifting = false;

    m_music_tracks.emplace("title_theme", LoadMusicStream("res/music/title_theme.ogg"));
    m_music_tracks.emplace("win_theme", LoadMusicStream("res/music/win_theme.ogg"));
    m_music_tracks.emplace("no_stopping_now", LoadMusicStream("res/music/no_stopping_now.ogg"));

    m_sound_effects.emplace("click", LoadSound("res/sfx/click.ogg"));
    SetSoundVolume(m_sound_effects.at("click"), 0.22f);

    m_sound_effects.emplace("grab", LoadSound("res/sfx/grab.ogg"));
    SetSoundVolume(m_sound_effects.at("grab"), 0.40f);
}

audio_manager::~audio_manager()
{
    for (const auto& [name, music] : m_music_tracks) {
        UnloadMusicStream(music);
    }

    for (const auto& [name, sound] : m_sound_effects) {
        UnloadSound(sound);
    }

    CloseAudioDevice();
}

void audio_manager::update()
{
    if (m_mixing) {

        // Fade out the current track.
        if (m_current_music != nullptr) {
            SetMusicVolume(
                *m_current_music,
                (m_frame_dur_mix - m_frame_count_mix) / static_cast<float>(m_frame_dur_mix)
            );
        }
        
        // Fade in the next track.
        if (m_next_music != nullptr) {
            SetMusicVolume(
                *m_next_music,
                m_frame_count_mix / static_cast<float>(m_frame_dur_mix)
            );
        }
        ++m_frame_count_mix;

        // If the frame count has exceeded the duration, stop the current music (mixed out track)
        // and set the current music to the next music. Mixing complete.
        if (m_frame_count_mix > m_frame_dur_mix) {
            if (m_current_music != nullptr) { StopMusicStream(*m_current_music); }
            m_current_music = m_next_music;
            m_mixing = false;
        }
    } 

    if (m_shifting) { 
        float t = m_frame_count_pitch / static_cast<float>(m_frame_dur_pitch);
        float new_pitch = m_current_pitch + t * (m_next_pitch - m_current_pitch);

        if (m_current_music != nullptr) {
            SetMusicPitch(*m_current_music, new_pitch);
        }

        if (m_next_music != nullptr) {
            SetMusicPitch(*m_next_music, new_pitch);
        }

        ++m_frame_count_pitch;

        if (m_frame_count_pitch > m_frame_dur_pitch) {
            m_current_pitch = m_next_pitch;
            m_shifting = false;
        }
    }

    // Update both of the music streams if they are properly set.
    if (m_current_music != nullptr) {
        UpdateMusicStream(*m_current_music);
    }

    if (m_next_music != nullptr) {
        UpdateMusicStream(*m_next_music);
    }
} 

void audio_manager::set_next_music(string track_name, bool looping)
{
    Music* next_music = &m_music_tracks.at(track_name);
 
    if (next_music == m_current_music) {
        TraceLog(LOG_DEBUG, "[%s] Track requested is already playing. No change.", __PRETTY_FUNCTION__);
        return;
    }
    else if (m_mixing) {
        TraceLog(LOG_DEBUG, "[%s] Track requested while mixing is active. No change.", __PRETTY_FUNCTION__);
        return;
    }

    next_music->looping = looping;
  
    m_mixing = true;
    m_frame_count_mix = 0;
    m_next_music = next_music;
    PlayMusicStream(*m_next_music);
    SetMusicVolume(*m_next_music, 0.0f);
}

void audio_manager::shift_pitch(float pitch)
{
    if (m_current_music == nullptr) return;

    m_next_pitch = pitch;
    m_frame_count_pitch = 0;
    m_shifting = true;
}
