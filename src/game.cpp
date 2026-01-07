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

using engine::game;

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

game::shader_manager::shader_manager()
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

game::shader_manager::~shader_manager()
{
    UnloadRenderTexture(m_target_a);
    UnloadRenderTexture(m_target_b);

    for (const auto& [name, shader] : m_shaders) {
        UnloadShader(shader);
    }
}

void game::shader_manager::begin()
{
    BeginTextureMode(m_target_a);
    m_in_texture_mode = true;
}

void game::shader_manager::append(string shader_name)
{
    m_shader_queue.push_back(shader_name);
}

void game::shader_manager::process()
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
    for (const auto& [name, music] : m_music_tracks) {
        UnloadMusicStream(music);
    }

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
