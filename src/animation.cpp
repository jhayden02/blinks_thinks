/***********************************************************************************************
*
*   animation.cpp - The library for drawing animations.
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
#include "animation.hpp"
#include "game.hpp"

using engine::game;
using engine::anim_raylib;
using engine::anim_self_credit;

// ------------------------------------------------------------------------------------------ //
//                                   Raylib splash screen.                                    //
// ------------------------------------------------------------------------------------------ //
anim_raylib::anim_raylib()
    :
    entity({0, 0}, 0), // Both of these are unused. Set default values of 0.
    m_logo_position_x((game::get_cw()) - 128),
    m_logo_position_y((game::get_ch()) - 128),
    m_top_side_rec_width(16),
    m_left_side_rec_height(16),
    m_bottom_side_rec_width(16),
    m_right_side_rec_height(16),
    m_frame_counter(0),
    m_letters_count(0),
    m_state(0),
    m_alpha(1.0f)
{}

bool anim_raylib::is_finished()
{
    return (m_state == 4);
}

void anim_raylib::update()
/***********************************************************************************************
*
*   Original animation courtesy of Ramon Santamaria (@raysan5)
*
*   This code has been changed from the original source code found here:
*       
*   https://github.com/raysan5/raylib/examples/shapes/shapes_logo_raylib_anim.c
*
*   Example originally created with raylib 2.5, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2025 Ramon Santamaria (@raysan5)
*
***********************************************************************************************/
{
    switch (m_state)
    {
        case (0): {
            m_frame_counter++;

            if (m_frame_counter == 120)
            {
                m_state = 1;
                m_frame_counter = 0;      // Reset counter... will be used later...
            }
        } break;

        case (1): {
            m_top_side_rec_width += 4;
            m_left_side_rec_height += 4;

            if (m_top_side_rec_width == 256) m_state = 2;
        } break;

        case (2): {
        m_bottom_side_rec_width += 4;
        m_right_side_rec_height += 4;

        if (m_bottom_side_rec_width == 256) m_state = 3;
        } break;

        case (3): { 
            m_frame_counter++;

            if (m_frame_counter/12)
            {
                m_letters_count++;
                m_frame_counter = 0;
            }

            if (m_letters_count >= 10)
            {
                m_alpha -= 0.02f;

                if (m_alpha <= 0.0f)
                {
                    m_alpha = 0.0f;
                    m_state = 4;
                }
            }
        } break;

        case (4): {} break;
    }
}

void anim_raylib::draw()
/***********************************************************************************************
*
*   Original animation courtesy of Ramon Santamaria (@raysan5)
*
*   This code has been changed from the original source code found here:
*       
*   https://github.com/raysan5/raylib/examples/shapes/shapes_logo_raylib_anim.c
*
*   Example originally created with raylib 2.5, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2025 Ramon Santamaria (@raysan5)
*
***********************************************************************************************/
{
    ClearBackground(RAYWHITE);
    
    switch (m_state)
    {
        case (0): {
            if ((m_frame_counter / 15) % 2) {
                DrawRectangle(m_logo_position_x, m_logo_position_y, 16, 16, BLACK);
            }
        } break;

        case (1): {
            DrawRectangle(m_logo_position_x, m_logo_position_y, m_top_side_rec_width, 16, BLACK);
            DrawRectangle(m_logo_position_x, m_logo_position_y, 16, m_left_side_rec_height, BLACK);
        } break;

        case (2): {
            DrawRectangle(m_logo_position_x, m_logo_position_y, m_top_side_rec_width, 16, BLACK);
            DrawRectangle(m_logo_position_x, m_logo_position_y, 16, m_left_side_rec_height, BLACK);

            DrawRectangle(m_logo_position_x + 240, m_logo_position_y, 16, m_right_side_rec_height, BLACK);
            DrawRectangle(m_logo_position_x, m_logo_position_y + 240, m_bottom_side_rec_width, 16, BLACK);
        } break;

        case (3): {
            DrawRectangle(
                m_logo_position_x,
                m_logo_position_y,
                m_top_side_rec_width,
                16,
                Fade(BLACK, m_alpha));

            DrawRectangle(
                m_logo_position_x,
                m_logo_position_y + 16,
                16,
                m_left_side_rec_height - 32,
                Fade(BLACK, m_alpha));

            DrawRectangle(
                m_logo_position_x + 240,
                m_logo_position_y + 16,
                16,
                m_right_side_rec_height - 32,
                Fade(BLACK, m_alpha));

            DrawRectangle(
                m_logo_position_x,
                m_logo_position_y + 240,
                m_bottom_side_rec_width, 16,
                Fade(BLACK, m_alpha));

            DrawRectangle(
                GetScreenWidth()/2 - 112,
                GetScreenHeight()/2 - 112,
                224,
                224,
                Fade(RAYWHITE, m_alpha));

            DrawText(
                TextSubtext("raylib", 0, m_letters_count),
                game::get_cw() - 44,
                game::get_ch() + 48,
                50,
                Fade(BLACK, m_alpha));

        } break;

        case (4): {
            return;
        } break;
    }
}

// ------------------------------------------------------------------------------------------ //
//                                 Self credit splash screen.                                 //
// ------------------------------------------------------------------------------------------ //
anim_self_credit::anim_self_credit()
    : 
    entity({0, 0}, 0), // Both of these are unused. Set default values of 0.
    m_frame_counter(0),
    m_letters_count(0),
    m_state(state::LETTERS_ADDING),
    m_text("A game by Josh Hayden"),
    m_font_size(40),
    m_spacing(4.0f),
    m_font(GetFontDefault()),

    m_text_position({ 
        static_cast<float>(game::get_cw() - 240), 
        static_cast<float>(game::get_ch())
    }),

    m_bg_color({ 30, 30, 30, 255})
{}

bool anim_self_credit::is_finished()
{
    return (m_state == state::FINISHED);
}

void anim_self_credit::update()
{ 
    m_frame_counter++;

    switch (m_state)
    {
        // Letters being added on every 3 frames.
        case state::LETTERS_ADDING: {
            if (m_letters_count < 21) {
                if (m_frame_counter / 3) {
                    m_letters_count++;
                    m_frame_counter = 0;
                }
            }
            // If done spelling, wait for 1 second, then move to m_state 1.
            else if (m_frame_counter / 60) {  
                    m_state = state::LETTERS_REMOVING; 
                    m_frame_counter = 0;
            }
        } break;

        // Letters being removed on every 3 frames.
        case state::LETTERS_REMOVING: {
            if (m_letters_count > 0) {
                if (m_frame_counter / 3) {
                    m_letters_count--;
                    m_frame_counter = 0;
                }
            }
            // If done removing letters, wait for 1 second, then move to m_state 2.
            else if (m_frame_counter / 60) {
                    m_state = state::CURSOR_BLINKING; 
            }
        } break;

        case state::CURSOR_BLINKING: {
            m_state = state::BLANK_SCREEN_DELAY; 
        } break;

        case state::BLANK_SCREEN_DELAY: {
            m_state = state::FINISHED;
        } break;

        case state::FINISHED: {} break;
    }
}

void anim_self_credit::draw()
{ 
    // draw a background rectangle.
    DrawRectangle(game::get_cw() - 300, game::get_ch() - 30, 600, 100, m_bg_color);
    switch (m_state)
    {
        // Letters being added on every 2 frames.
        case state::LETTERS_ADDING: 
        case state::LETTERS_REMOVING: { 
            float x = m_text_position.x;
            float y = m_text_position.y;

            // draw every character in 'text' sequentially, drawing a terminal cursor on the
            // last letter.
            for (int i = 0; i < m_letters_count; i++) {
                char c = m_text.c_str()[i];
                char s[2] = { c, '\0' };

                Vector2 char_size = MeasureTextEx(m_font, s, m_font_size, m_spacing);

                bool is_last = (i == m_letters_count - 1);

                if (is_last) {
                    DrawRectangle(
                        static_cast<int>(x), 
                        static_cast<int>(y), 
                        static_cast<int>(char_size.x), 
                        m_font_size, 
                        DARKBLUE
                    );
                }

                DrawTextEx(m_font, s, { x, y }, m_font_size, m_spacing, is_last ? WHITE : SKYBLUE);
                x += char_size.x + m_spacing;
            }
        } break;

        case state::CURSOR_BLINKING:
        case state::BLANK_SCREEN_DELAY: 
        case state::FINISHED: {
            return;
        } break;
    }
}
