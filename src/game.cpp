/***********************************************************************************************
* 
*   game.cpp - The library for creating the monolithic game managing object.
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

// Source.
#include "game.hpp"

using engine::game;

// Standard library.
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <climits>

#ifdef PLATFORM_WEB
#include <emscripten.h>
namespace web
{
    EM_JS(int, mouse_in_canvas, (), { return mouse_in_canvas_flag ? 1 : 0; } );
}
#endif

game::game()
    : audio(audio_manager::get_instance())
{
    // Class initialization.
    std::random_device random_generator_seed;
    m_random_generator.seed(random_generator_seed());

    this->m_button_in_hand = nullptr;

    // Window, Screen, and FPS.
    InitWindow(m_w, m_h, m_game_name);
    SetWindowSize(m_w, m_h); // This fixes some Wayland bugs related to window size.
    SetTargetFPS(m_frame_rate); 

    // Remove the ESC key as a close command.
    SetExitKey(KEY_NULL);

    // Set the tracelog level.
    SetTraceLogLevel(LOG_DEBUG);

    // Load background blur effect.
    m_background_target = LoadRenderTexture(m_w, m_h);
    m_blur_shader = LoadShader(0, "res/shaders/blur.frag");
}

game::~game()
{
    UnloadRenderTexture(m_background_target);
    UnloadShader(m_blur_shader);
    CloseWindow();
}

game::audio_manager::audio_manager()
{
    // Setup the audio device.
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(16384);
    
    // Initialize variables.
    m_current_music = nullptr;
    m_next_music = nullptr;
    m_current_pitch = 1.0f;
    m_next_pitch = 1.0f;
    m_frame_count_mix = 0;
    m_frame_count_pitch = 0;
    m_mixing = false;
    m_shifting = false;

    // Load all music tracks into 'm_music_tracks'.
    m_music_tracks.emplace("title_theme", LoadMusicStream("res/music/title_theme.ogg"));
    m_music_tracks.emplace("win_theme", LoadMusicStream("res/music/win_theme.ogg")); 
    m_music_tracks.emplace("no_stopping_now", LoadMusicStream("res/music/no_stopping_now.ogg")); 

    // Load all sound effects into 'm_sound_effects' with needed adjustments.
    m_sound_effects.emplace("click", LoadSound("res/sfx/click.ogg"));
    SetSoundVolume(m_sound_effects.at("click"), 0.22f);

    m_sound_effects.emplace("grab", LoadSound("res/sfx/grab.ogg"));
    SetSoundVolume(m_sound_effects.at("grab"), 0.40f);
}

game::audio_manager::~audio_manager()
{
    // Unload all the music tracks.
    for (const auto& [name, music] : m_music_tracks) {
        UnloadMusicStream(music);
    }

    // Unload all the sounds.
    for (const auto& [name, sound] : m_sound_effects) {
        UnloadSound(sound);
    }

    CloseAudioDevice();
}

void game::audio_manager::update()
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

void game::audio_manager::set_next_music(string track_name, bool looping)
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

void game::audio_manager::shift_pitch(float pitch) {
    if (m_current_music == nullptr) return;

    m_next_pitch = pitch;
    m_frame_count_pitch = 0;
    m_shifting = true;
}
    
void game::run()
{
    while (!WindowShouldClose())
    {
        // ---------------------------------------------------------------------------------- //
        //                                      update.                                       //
        // ---------------------------------------------------------------------------------- //
        if (m_next_level != nullptr) {
            if (m_current_level != nullptr) { delete m_current_level; }
            m_current_level = m_next_level;
            m_next_level = nullptr;
        }
        
        if (m_current_level != nullptr) { m_current_level->update(); }

        audio.update();

        // ---------------------------------------------------------------------------------- //
        //                                       draw.                                        //
        // ---------------------------------------------------------------------------------- //
        if (m_current_level != nullptr) {
            BeginTextureMode(m_background_target);
                ClearBackground(RAYWHITE);
                m_current_level->draw_layers(INT_MIN, -1);
            EndTextureMode();

            BeginDrawing();
                ClearBackground(BLACK);
                BeginShaderMode(m_blur_shader);
                    DrawTextureRec(
                        m_background_target.texture,
                        {0.0f, 0.0f, static_cast<float>(m_w), -static_cast<float>(m_h)},
                        {0.0f, 0.0f},
                        WHITE
                    );
                EndShaderMode();
                m_current_level->draw_layers(0, INT_MAX);
            EndDrawing();
        } else {
            BeginDrawing();
                ClearBackground(RAYWHITE);
                if (m_current_level != nullptr) m_current_level->draw();
            EndDrawing();
        }
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
