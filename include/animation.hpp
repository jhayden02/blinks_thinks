/***********************************************************************************************
*
*   animation.hpp - The library for drawing animations.
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
#include "entity.hpp"

// Standard library.
#include <string>

using std::string;

namespace engine
{
    class anim_raylib : public entity
    {
        public:
            anim_raylib();
            bool is_finished();
            void update() override;
            void draw() override;

        private:
            // Position.
            int m_logo_position_x;
            int m_logo_position_y;

            // Size.
            int m_top_side_rec_width;
            int m_left_side_rec_height;
            int m_bottom_side_rec_width;
            int m_right_side_rec_height;

            // Counters and state machines.
            int m_frame_counter;
            int m_letters_count;
            int m_state;

            float m_alpha;
    };

    class anim_self_credit : public entity
    {
        public:
            anim_self_credit();
            bool is_finished();
            void draw() override;
            void update() override;

        private:
            // Counters.
            int m_frame_counter;
            int m_letters_count;
           
            // state of the animation.
            enum class state {
                LETTERS_ADDING,
                LETTERS_REMOVING,
                CURSOR_BLINKING,
                BLANK_SCREEN_DELAY,
                FINISHED
            };
            state m_state;

            // text.
            string m_text;
            int m_font_size;
            float m_spacing;
            Font m_font;
            Vector2 m_text_position;

            // Colors.
            Color m_bg_color;
    };
}
