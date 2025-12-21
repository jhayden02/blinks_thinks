/***********************************************************************************************
* 
*   game_levels.hpp - The implementation of the level class. All levels used in the game are
*                     defined here.
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
using engine::label;
using engine::button;
using engine::anim_raylib;
using engine::anim_self_credit;

// Standard library.
#include <string>

using std::string;

class intro_raylib : public engine::level
{
    public:
        intro_raylib();
        void update() override;

    private:
        anim_raylib* m_animation;
};

class intro_self_credit : public engine::level
{
    public:
        intro_self_credit();
        void update() override;

    private:
        anim_self_credit* m_animation;
};

class level_title : public engine::level
{
    public:
        level_title();
        void update() override;

    private:

        class game_title_text
        {
            public:
                text* text_obj;
 
                static constexpr float initial_rotation = 0.0;
                static constexpr float rotation_speed = 5.0;
                static constexpr float rotation_depth = 2.5;

                static constexpr Color text_color = { 75, 255, 205, 255 };
        };
        game_title_text m_game_title_text;

        button* m_play_button;
};

class level_lose : public engine::level
{
    public:
        level_lose();
        void update() override;

    private:
        button* m_restart_button;
};

class level_win : public engine::level
{
    public:
        level_win();
        void update() override;

    private:
        button* m_title_screen_button;
};

class intro_section_one : public engine::level
{
    public:
        intro_section_one();
        void update() override;

    private:
        int m_frames_counter;
};

class level_one : public engine::level
{
    public:
        level_one();
        void update() override;

    private:
        static constexpr int m_choice_count = 5, m_min_choice = 1, m_max_choice = 25; 
        button* m_correct_button;
};

class level_two : public engine::level
{
    public:
        level_two(); 
        void update() override;

    private:
        static constexpr int m_choice_count = 5, m_min_choice = 1, m_max_choice = 25; 
        button* m_correct_button;
};

class level_three : public engine::level
{
    public:
        level_three(); 
        void update() override;

    private:
        static constexpr int m_choice_count = 5, m_min_choice = 1, m_max_choice = 25; 

        button* m_correct_button; 
};

class level_four : public engine::level
{
    public:
        level_four(); 
        void update() override;
};

class level_five : public engine::level
{
    public:
        level_five(string duration); 
        void update() override;

    private:
        int m_frames_counter;

        string m_duration; 
        button* m_timer;
};

class level_six : public engine::level
{
    public:
        level_six();
        void update() override;

    private:
        static constexpr int m_choice_count = 5, m_min_choice = 1, m_max_choice = 25;  
        int m_frames_counter;
        button* m_correct_button;
};

class level_seven : public engine::level
{
    public:
        level_seven();
        void update() override;

    private:
        button* m_button_seven;
        button* m_button_nine;
        static constexpr int m_choice_count = 5;
};

class level_eight : public engine::level
{
    public:
        level_eight();
        void update() override;

    private:
        static constexpr int m_choice_count = 6, m_fib_seq_len = 5;

        // Will return a fibbonacci sequence of length 'length', started by 'num_one' + 'num_two',
        // exiting if the returned length results in a number larger than an int can store.
        vector<int> get_fib_seq(int num_one, int num_two); 

        button* m_correct_button;
};

class level_nine : public engine::level
{
    public:
        level_nine();
        void update() override;

    private:
        label* m_submit_box;
        button* m_submit_button;
        static constexpr int m_choice_count = 5;
        vector<button*> m_correct_button_layout;
};

class level_ten : public engine::level
{
    public:
        level_ten();
        void update() override;

    private:
        button* m_submit_box;
        button* m_holdable_number;
        button* m_submit_button;
        static constexpr int m_choice_count = 5;
        vector<button*> m_correct_button_layout;
};
