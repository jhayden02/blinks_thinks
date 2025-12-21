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

// Source.
#include "game.hpp"
#include "game_levels.hpp"

using engine::game;
using engine::entity;
using engine::grows_when_hovered;
using engine::grabbable;

// Standard library.
#include <iterator>
#include <algorithm>
#include <limits.h>

using std::to_string;

// ------------------------------------------------------------------------------------------ //
//                                     Raylib animation.                                      //
// ------------------------------------------------------------------------------------------ //
intro_raylib::intro_raylib()
{
    this->m_animation = add_entity(new anim_raylib());
    m_game.audio.set_next_music("title_theme"); 
}

void intro_raylib::update() {
    level::update(); 

    if (m_animation->is_finished()) {
        m_game.set_next_level(new intro_self_credit());
    }
    else if (IsKeyPressed(KEY_ENTER)) {
        m_game.set_next_level(new level_title());
    }
}

// ------------------------------------------------------------------------------------------ //
//                                   Self credit animation.                                   //
// ------------------------------------------------------------------------------------------ //
intro_self_credit::intro_self_credit()
{
    this->m_animation = add_entity(new anim_self_credit());
}

void intro_self_credit::update()
{
    level::update();

    if (m_animation->is_finished() || IsKeyPressed(KEY_ENTER)) {
        m_game.set_next_level(new level_title());
    }
}

// ------------------------------------------------------------------------------------------ //
//                                       Title screen.                                        //
// ------------------------------------------------------------------------------------------ //
level_title::level_title()
{
    this->m_game_title_text.text_obj = add_simple_text(
        "Blink's Thinks",
        100,
        m_game_title_text.text_color,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    );
    this->m_play_button = add_ui_button("Play");
    this->m_game_title_text.text_obj->add_anim_rotate(0.0f, 5.0f, 2.5f);
    
    string version_and_build_display_str  = ("v" + game::get_game_version());
    float version_and_build_display_spacing;
    #ifndef NDEBUG
        version_and_build_display_str += " debug";
        version_and_build_display_spacing = 74.0f;
    #else
        version_and_build_display_str += " release";
        version_and_build_display_spacing = 84.0f;
    #endif

    // Display the current version number in the bottom right.
    add_entity(
        new text(
            version_and_build_display_str,
            20,
            RAYWHITE,
            {0, 0, 0, 0},
            {game::get_w() - version_and_build_display_spacing, 20},
            1000
        )
    );

    m_game.audio.set_next_music("title_theme");
}

void level_title::update()
{
    level::update(); 
    
    if (m_play_button->is_pressed()) {
        m_game.set_next_level(new intro_section_one());
    }
}

// ------------------------------------------------------------------------------------------ //
//                                        Lose screen.                                        //
// ------------------------------------------------------------------------------------------ //
level_lose::level_lose()
{
    this->m_restart_button = add_ui_button("Restart");
    add_simple_text(
        "game over!",
        100,
        RED,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    )
    ->add_anim_rotate(0.0f, 5.0f, 2.5f);

    // Pitch the music down.
    m_game.audio.shift_pitch(0.8f);
}

void level_lose::update()
{
    level::update();

    if (m_restart_button->is_pressed()) {
        m_game.set_next_level(new intro_section_one());
    }
}

// ------------------------------------------------------------------------------------------ //
//                                        Win screen.                                         //
// ------------------------------------------------------------------------------------------ //
level_win::level_win()
{
    m_game.audio.set_next_music("win_theme", false);  

    this->m_title_screen_button = add_ui_button("Title");
    add_simple_text(
        "You win!",
        100,
        GREEN,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    )
    ->add_anim_rotate(0.0f, 5.0f, 2.5f);
}

void level_win::update()
{
    level::update();

    if (m_title_screen_button->is_pressed()) {
        m_game.set_next_level(new level_title());
    }
}

// ------------------------------------------------------------------------------------------ //
//                               Section Introduction: Numbers.                               //
// ------------------------------------------------------------------------------------------ //
intro_section_one::intro_section_one()
{
    this->m_frames_counter = 0;

    add_simple_text(
        "Levels 1-10: Numbers",
        60,
        GREEN,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    );

    // Set the music track, and set the pitch make to normal if it's not.
    m_game.audio.set_next_music("no_stopping_now");
    m_game.audio.shift_pitch(1.0f);
}

void intro_section_one::update()
{
    level::update();

    ++m_frames_counter;

    if (m_frames_counter == 3 * m_game.get_frame_rate()) {
        m_game.set_next_level(new level_one());
    }
}

// ------------------------------------------------------------------------------------------ //
//                                          level 1.                                          //
// ------------------------------------------------------------------------------------------ //
level_one::level_one() 
{
    add_simple_text(
        "level  ",
        80,
        ORANGE,
        {m_game.get_cw() - 4, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "What is the largest number?",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);

    //
    // Button creation.
    //
    vector<button*> choices(m_choice_count);

    vector<Vector2> button_positions = {
        {m_game.get_cw() + 122, m_game.get_ch() - 250},
        {m_game.get_cw() - 275, m_game.get_ch()},
        {m_game.get_cw() - 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 275, m_game.get_ch()}
    };

    // Get a sequence of 4 random numbers, then insert the level number (1) at the beginning.
    vector<int> button_values = m_game.get_random_sequence(4, m_min_choice, m_max_choice, {1});
    button_values.insert(button_values.begin(), 1);

    // Get a sequence of 4 random colors, then insert the level number's color (GOLD) at the beginning.
    vector<Color> button_colors = m_game.get_random_color_sequence(4);
    button_colors.insert(button_colors.begin(), ORANGE);
    
    // Ensure all the vectors are constructed to the same proper size.
    GAME_ASSERT(
        (button_positions.size() == m_choice_count) &&
        (button_values.size() == m_choice_count) &&
        (button_colors.size() == m_choice_count),
        "Not all button construction vectors are of the class-defined size (m_choice_count)."
    );

    // Get 3 iterators for each of these vectors, and *it++ them throughout the loop.
    vector<Vector2>::iterator positions_it = button_positions.begin();
    vector<int>::iterator values_it = button_values.begin();
    vector<Color>::iterator colors_it = button_colors.begin();
   
    // Construct a vector of buttons from which we will choose a correct answer. 
    vector<button*> choosable_buttons(m_choice_count);

    for (size_t loop_count = 0; loop_count != m_choice_count; ++loop_count) {
        button* btn = add_text_button(
            to_string(*values_it++),
            80,
            *colors_it++,
            *positions_it++
        ); 
		btn->add_trait(new grows_when_hovered());
		choosable_buttons[loop_count] = btn;
    }

    // Set 'm_correct_button' to the choice with the smallest value.
    m_correct_button = *std::max_element(choosable_buttons.begin(), choosable_buttons.end(),
        [](button* a, button* b) {
            return (stoi(a->get_text()) < stoi(b->get_text()));
        }
    ); 
}

void level_one::update()
{
    level::update();
    
    if (m_correct_button->is_pressed()) {
        m_game.set_next_level(new level_two());
    }
    else {
        for (button* btn : get_buttons()) {
            if (btn->is_pressed()) {
                m_game.set_next_level(new level_lose());
            }
        }
    }
}

// ------------------------------------------------------------------------------------------ //
//                                          level 2.                                          //
// ------------------------------------------------------------------------------------------ //
level_two::level_two()
{
    add_simple_text(
        "level  ",
        80,
        ORANGE,
        {m_game.get_cw() - 4, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "What is the smallest number?",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);


    //
    // Button creation.    
    //
    vector<button*> choices(m_choice_count);

    vector<Vector2> button_positions = {
        {m_game.get_cw() + 122, m_game.get_ch() - 250},
        {m_game.get_cw() - 275, m_game.get_ch()},
        {m_game.get_cw() - 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 275, m_game.get_ch()}
    };

    // Get a sequence of 4 random numbers, then insert the level number (2) at the beginning.
    vector<int> button_values = m_game.get_random_sequence(4, m_min_choice, m_max_choice, {2});
    button_values.insert(button_values.begin(), 2);

    // Get a sequence of 4 random colors, then insert the level number's color at the beginning.
    vector<Color> button_colors = m_game.get_random_color_sequence(4);
    button_colors.insert(button_colors.begin(), ORANGE);
    
    // Ensure all the vectors are constructed to the same proper size.
    GAME_ASSERT(
        (button_positions.size() == m_choice_count) &&
        (button_values.size() == m_choice_count) &&
        (button_colors.size() == m_choice_count),
        "Not all button construction vectors are of the class-defined size (m_choice_count)."
    );

    // Get 3 iterators for each of these vectors, and *it++ them throughout the loop.
    vector<Vector2>::iterator positions_it = button_positions.begin();
    vector<int>::iterator values_it = button_values.begin();
    vector<Color>::iterator colors_it = button_colors.begin();
   
    // Construct a vector of buttons from which we will choose a correct answer. 
    vector<button*> choosable_buttons(m_choice_count);

    for (size_t loop_count = 0; loop_count != m_choice_count; ++loop_count) {
        button* btn = add_text_button(
            to_string(*values_it++),
            80,
            *colors_it++,
            *positions_it++
        ); 
		btn->add_trait(new grows_when_hovered());
		choosable_buttons[loop_count] = btn;
    }

    // Set 'm_correct_button' to the choice with the smallest value.
    m_correct_button = *std::min_element(choosable_buttons.begin(), choosable_buttons.end(),
        [](button* a, button* b) {
            return (stoi(a->get_text()) < stoi(b->get_text()));
        }
    );
}

void level_two::update()
{
    level::update();

    if (m_correct_button->is_pressed()) {
        m_game.set_next_level(new level_three());
    }
    else {
        for (button* btn : get_buttons()) {
            if (btn->is_pressed()) {
                m_game.set_next_level(new level_lose());
            }
        }
    }
}

// ------------------------------------------------------------------------------------------ //
//                                          level 3.                                          //
// ------------------------------------------------------------------------------------------ //
level_three::level_three() 
{
    //
    // Main UI elements (level title, directions).
    //
    add_simple_text("level  ", 80, ORANGE, {m_game.get_cw(), m_game.get_ch() - 250}, 0);

    add_simple_text("What is the tallest number?", 40, RAYWHITE, {m_game.get_cw(), m_game.get_ch() - 150}, 0)
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);
    
    //
    // Button creation.
    //
    vector<button*> choices(m_choice_count);

    vector<Vector2> button_positions = {
        {m_game.get_cw() + 122, m_game.get_ch() - 250},
        {m_game.get_cw() - 275, m_game.get_ch()},
        {m_game.get_cw() - 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 275, m_game.get_ch()}
    };

    // Get a sequence of 4 random numbers, then insert the level number (3) at the beginning.
    vector<int> button_values = m_game.get_random_sequence(4, m_min_choice, m_max_choice, {3});
    button_values.insert(button_values.begin(), 3);

    // Get a sequence of 4 random colors, then insert the level number's color (GOLD) at the beginning.
    vector<Color> button_colors = m_game.get_random_color_sequence(4);
    button_colors.insert(button_colors.begin(), ORANGE);
    
    // Ensure all the vectors are constructed to the same proper size.
    GAME_ASSERT(
        (button_positions.size() == m_choice_count) &&
        (button_values.size() == m_choice_count) &&
        (button_colors.size() == m_choice_count),
        "Not all button construction vectors are of the class-defined size (m_choice_count)."
    );

    // Get 3 iterators for each of these vectors, and *it++ them throughout the loop.
    vector<Vector2>::iterator positions_it = button_positions.begin();
    vector<int>::iterator values_it = button_values.begin();
    vector<Color>::iterator colors_it = button_colors.begin();
   
    // Choose a random iteration to be the correct 'growing' number.
    size_t chosen_loop_count = m_game.get_random_value(0, m_choice_count - 1);

    for (size_t loop_count = 0; loop_count != m_choice_count; ++loop_count) {
        button* btn = add_text_button(
            to_string(*values_it++),
            80,
            *colors_it++,
            *positions_it++
        ); 

		if (loop_count == chosen_loop_count) {
			m_correct_button = btn;
			m_correct_button->add_trait(new grows_when_hovered(20, 2.5f));
		}
		else {
			btn->add_trait(new grows_when_hovered());
		}
    }	
}

void level_three::update()
{
    level::update();

    if (m_correct_button->is_pressed()) {
        m_game.set_next_level(new level_four());
    }
    else {
        for (button* btn : get_buttons()) {
            if (btn->is_pressed()) {
                m_game.set_next_level(new level_lose());
            }
        }
    }
}

// ------------------------------------------------------------------------------------------ //
//                                          level 4.                                          //
// ------------------------------------------------------------------------------------------ //
level_four::level_four()
{
    //
    // Main UI elements (level title, directions).
    //
    add_simple_text(
        "level 4",
        80,
        ORANGE,
        {m_game.get_cw(),
        m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "How much time do you want for level 5?",
        40,
        RAYWHITE,
        {m_game.get_cw(),
        m_game.get_ch() - 150},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);

    add_text_button(
        "2 seconds",
        60,
        m_game.get_random_color(),
        {m_game.get_cw(),
        m_game.get_ch() - 50}
    );

    add_text_button(
        "5 seconds",
        60,
        m_game.get_random_color(),
        {m_game.get_cw(),
        m_game.get_ch() + 50}
    );

    add_text_button(
        "10 seconds",
        60,
        m_game.get_random_color(),
        {m_game.get_cw(),
        m_game.get_ch() + 150}
    );

    for (button* btn : get_buttons()) {
        btn->add_trait(new grows_when_hovered());
    }
}

void level_four::update()
{
    level::update();

    for (button* btn : get_buttons()) {
        if (btn->is_pressed()) {
            string chosen_time = btn->get_text();
            chosen_time.erase(chosen_time.find(" seconds"), chosen_time.length());
            m_game.set_next_level(new level_five(chosen_time));
        }
    }
}

// ------------------------------------------------------------------------------------------ //
//                                          level 5.                                          //
// ------------------------------------------------------------------------------------------ //
level_five::level_five(string duration)
{
    this->m_frames_counter = 0;
    this->m_duration = duration;
    this->m_timer = add_text_button(
        m_duration,
        80,
        RED,
        {m_game.get_cw(), m_game.get_ch() - 65}
    );

    //
    // Main UI elements (level title, directions, countdown timer).
    //
    add_simple_text(
        "level  ",
        80,
        ORANGE,
        {m_game.get_cw() - 4, m_game.get_ch() - 250},
        0
    );

    add_text_button(
        "5",
        80,
        ORANGE,
        {m_game.get_cw() + 122, m_game.get_ch() - 250}
    );

    add_simple_text(
        "Don't touch any numbers for",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);

    add_simple_text(
        "more seconds",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch()},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);

    //
    // Button creation.
    //
    const string long_num_one   = "762967340328811348866734234450240332396217777462684390";
    const string long_num_two   = "239427620310921174648449330989407894927458570770003111";
    const string long_num_three = "230459256723665565627118580006023666643111673444567710";

    add_text_button(
        "68",
        120,
        RED,
        {m_game.get_cw() - 270, m_game.get_ch() - 500}
    )
    ->set_speed({0, 12});

    add_text_button(
        long_num_one,
        160,
        RED,
        {m_game.get_cw() - 3050, m_game.get_ch() + 250}
    )
    ->set_speed({3, -1});

    add_text_button(
        long_num_two,
        160,
        RED,
        {m_game.get_cw() + 3050, m_game.get_ch() - 230}
    )
    ->set_speed({-3, 1});

    add_text_button(
        "8043001",
        120,
        RED,
        {m_game.get_cw() - 200, m_game.get_ch() + 1200}
    )
    ->set_speed({0, -5});

    add_text_button(
        "3",
        120,
        RED,
        {m_game.get_cw() + 600, m_game.get_ch()}
    )
    ->set_speed({-1, 0});

    add_text_button(
        "762",
        160,
        RED,
        {m_game.get_cw() - 1300, m_game.get_ch()}
    )
    ->set_speed({2, 0});

    add_text_button(
        "12",
        120,
        RED,
        {m_game.get_cw() + 1800, m_game.get_ch() - 600}
    )
    ->set_speed({-8, 4});

    add_text_button(
        "5000006",
        120,
        RED,
        {m_game.get_cw() + 200, m_game.get_ch() - 3000}
    )
    ->set_speed({0, 7});

    add_text_button(
        "3078",
        80,
        RED,
        {m_game.get_cw() - 250, m_game.get_ch() - 3100}
    )
    ->set_speed({0, 7});

    add_text_button(
        long_num_three,
        150,
        RED,
        {m_game.get_cw() + 4550, m_game.get_ch() - 150}
    )
    ->set_speed({-4, 0});

    add_text_button(
        "7877878447232634",
        150,
        RED,
        {m_game.get_cw(), m_game.get_ch() + 800}
    )
    ->set_speed({0, -1});

    add_text_button(
        "64",
        120,
        RED,
        {m_game.get_cw() + 3000, m_game.get_ch() + 700}
    )
    ->set_speed({-5, -1});
}

void level_five::update()
{
    level::update();

    bool level_lost = false;

    // update the timer once per 60 frames.
    m_frames_counter++; 
    if (m_frames_counter == 60) {
        int duration_as_int = stoi(m_duration);
        if (duration_as_int > 0) {
            m_frames_counter = 0;
            --duration_as_int;
            
            m_duration = to_string(duration_as_int);
            m_timer->get_text_obj()->set_text_str(m_duration);
        }
        else {
            m_game.set_next_level(new level_six());
        }
    }

    // The player will lose if any button is hovered, or if the window (game) becomes unfocused.
    for (button* btn : get_buttons()) {
        if (btn->is_hovered()) {
            level_lost = true;    
        }
    }

    if (!m_game.mouse_in_canvas()) {
        level_lost = true;
    }

    if (level_lost) {
        m_game.set_next_level(new level_lose());
    }
}

// ------------------------------------------------------------------------------------------ //
//                                          level 6.                                          //
// ------------------------------------------------------------------------------------------ //
level_six::level_six()
{
    this->m_frames_counter = 0;

    //
    // Main UI elements (level title, directions).
    //
    this->m_correct_button = add_text_button(
        "3",
        80,
        m_game.get_random_color(),
        {-200, m_game.get_ch() + 85}
    );
    m_correct_button->set_speed({20, 0});

    add_simple_text(
        "level  ",
        80,
        ORANGE,
        {m_game.get_cw() - 4, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "Click the number 3",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);
    
    //
    // Button creation.
    //
    vector<Vector2> button_positions = {
        {m_game.get_cw() + 122, m_game.get_ch() - 250},
        {m_game.get_cw() - 275, m_game.get_ch()},
        {m_game.get_cw() - 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 275, m_game.get_ch()}
    };

    // Get a sequence of 4 random numbers, then insert the level number (6) at the beginning.
    vector<int> button_values = m_game.get_random_sequence(m_choice_count - 1, m_min_choice, m_max_choice, {6});
    button_values.insert(button_values.begin(), 6);

    // Get a sequence of 4 random colors, then insert the level number's color (ORANGE) at the beginning.
    vector<Color> button_colors = m_game.get_random_color_sequence(m_choice_count - 1);
    button_colors.insert(button_colors.begin(), ORANGE);
    
    // Ensure all the vectors are constructed to the same proper size.
    GAME_ASSERT(
        (button_positions.size() == m_choice_count) &&
        (button_values.size() == m_choice_count) &&
        (button_colors.size() == m_choice_count),
        "Not all button construction vectors are of the class-defined size (m_choice_count)."
    );

    // Get 3 iterators for each of these vectors, and *it++ them throughout the loop.
    vector<Vector2>::iterator positions_it = button_positions.begin();
    vector<int>::iterator values_it = button_values.begin();
    vector<Color>::iterator colors_it = button_colors.begin();
   
    for (size_t loop_count = 0; loop_count != m_choice_count; ++loop_count) {
        add_text_button(
            to_string(*values_it++),
            80,
            *colors_it++,
            *positions_it++
        ); 
    } 

	for (button* btn : get_buttons()) {
		btn->add_trait(new grows_when_hovered());
	}
}

void level_six::update()
{
    level::update();


    if (m_correct_button->get_position().x >= m_game.get_cw() && m_frames_counter < 60) {
        m_correct_button->set_speed({0, 0});
        ++m_frames_counter;
    }

    if (m_frames_counter == 60) {
        m_correct_button->set_speed({20, 0});
    }

    for (button* btn : get_buttons()) {
        if (btn->is_pressed()) {
            if (btn == m_correct_button) {
                m_game.set_next_level(new level_seven());
            }
            else {
                m_game.set_next_level(new level_lose());
            }
        }
    }

    // Check if the number '3' has gone off of the screen to the right. If so, the player loses.
    if (m_correct_button->get_position().x > m_game.get_w() + 80) {
        m_game.set_next_level(new level_lose());
    }

}

// ------------------------------------------------------------------------------------------ //
//                                          level 7.                                          //
// ------------------------------------------------------------------------------------------ //
level_seven::level_seven()
{
    //
    // Main UI elements (level title, directions).
    //
    add_simple_text(
        "level  ",
        80,
        ORANGE,
        {m_game.get_cw() - 4, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "Feed the hungry number the proper food",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);

    //
    // Button creation.
    //
    vector<Vector2> button_positions = {
        {m_game.get_cw() + 122, m_game.get_ch() - 250},
        {m_game.get_cw() - 275, m_game.get_ch()},
        {m_game.get_cw() - 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 275, m_game.get_ch()}
    };

    // Get a sequence of 4 random numbers, then insert the level number (9), then (7) at the beginning.
    vector<int> button_values = m_game.get_random_sequence(m_choice_count - 2, 1, 8, {7});
    button_values.insert(button_values.begin(), 9);
    button_values.insert(button_values.begin(), 7);

    // Get a sequence of 4 random colors, then insert the level number's color (GOLD) at the beginning.
    vector<Color> button_colors = m_game.get_random_color_sequence(m_choice_count - 1);
    button_colors.insert(button_colors.begin(), ORANGE);
    
    // Ensure all the vectors are constructed to the same proper size.
    GAME_ASSERT(
        (button_positions.size() == m_choice_count) &&
        (button_values.size() == m_choice_count) &&
        (button_colors.size() == m_choice_count),
        "Not all button construction vectors are of the class-defined size (m_choice_count)."
    );

    // Get 3 iterators for each of these vectors, and *it++ them throughout the loop.
    vector<Vector2>::iterator positions_it = button_positions.begin();
    vector<int>::iterator values_it = button_values.begin();
    vector<Color>::iterator colors_it = button_colors.begin();
    
    m_button_seven = add_text_button(
        to_string(*values_it++),
        80,
        *colors_it++,
        *positions_it++
    );

    m_button_nine = add_text_button(
        to_string(*values_it++),
        80,
        *colors_it++,
        *positions_it++
    );

    for (size_t loop_count = 2; loop_count != m_choice_count; ++loop_count) {
        add_text_button(
            to_string(*values_it++),
            80,
            *colors_it++,
            *positions_it++
        );
    }

	for (button* btn : get_buttons()) {
		btn->add_trait(new grows_when_hovered());
		btn->add_trait(new grabbable());
	}
}

void level_seven::update()
{
    level::update();

    button* button_in_hand = m_game.get_button_in_hand();

    if (button_in_hand != nullptr) {
        for (button* btn : get_buttons()) {
            const bool two_buttons_collided = (
                btn != button_in_hand &&
                CheckCollisionRecs(button_in_hand->get_rectangle(), btn->get_rectangle())
            );

            if (two_buttons_collided) {
                const bool seven_and_nine_collided = (
                    (button_in_hand == m_button_seven && btn == m_button_nine) ||
                    (button_in_hand == m_button_nine && btn == m_button_seven)
                );

                if (seven_and_nine_collided) {
                    m_game.set_next_level(new level_eight());
                }
                else {
                    m_game.set_next_level(new level_lose());
                }
            }
        }
    }
}

// ------------------------------------------------------------------------------------------ //
//                                          level 8.                                          //
// ------------------------------------------------------------------------------------------ //
vector<int> level_eight::get_fib_seq(int num_one, int num_two)
{
    vector<int> sequence(m_fib_seq_len);
    int num_three;
    for (size_t i = 0; i != m_fib_seq_len; ++i) {
        sequence[i] = num_one;
        GAME_ASSERT(
        num_one + num_two < INT_MAX,
        "INT_MAX was exceeded when trying to calculate a member of a fibbonacci sequence of the requested size."
        );
       num_three = num_one + num_two;
       num_one = num_two;
       num_two = num_three;
    }
    return sequence;
}

level_eight::level_eight()
{
    //
    // Main UI elements (level title, directions, Fibbonacci sequence hint).
    //
    add_simple_text(
        "level  ",
        80,
        ORANGE,
        {m_game.get_cw() - 4,
        m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "What number comes next?",
        40,
        RAYWHITE,
        {m_game.get_cw(),
        m_game.get_ch() - 150},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);

    // Get a completed fibbonacci sequence from a random first and second number.
    int fib_seq_num_one = m_game.get_random_value(1, 4);
    int fib_seq_num_two = m_game.get_random_value(5, 9);
    vector<int> fib_seq = get_fib_seq(fib_seq_num_one, fib_seq_num_two);

    string fib_seq_question_str = ""; 

    for (vector<int>::iterator it = fib_seq.begin(); it != fib_seq.end() - 1; ++it) {
        fib_seq_question_str += to_string(*it);
        fib_seq_question_str += ", ";
    }   
    fib_seq_question_str += "?";

    add_simple_text(
        fib_seq_question_str,
        40,
        YELLOW,
        {m_game.get_cw(), m_game.get_ch() - 100},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);

    //
    // Button creation.
    //
    vector<Vector2> button_positions = {
        {m_game.get_cw() + 122, m_game.get_ch() - 250},
        {m_game.get_cw() - 275, m_game.get_ch()},
        {m_game.get_cw() - 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 275, m_game.get_ch()},
        {m_game.get_cw(), m_game.get_ch() + 50}
    };

    int correct_value = fib_seq.back();
 
    // All possible Fibbonacci sequences derived from 1-4 and 5-9 have a 2-digit 5th number.
    // This is why we can hard code this range of incorrect value, but correct digit-length
    // wrong numbers.
    const int min_choice = 10;
    const int max_choice = 99;

    vector<int> button_values = m_game.get_random_sequence(
        m_choice_count - 2,
        min_choice,
        max_choice,
        {correct_value}
    );
    button_values.insert(button_values.begin(), correct_value);
    button_values.insert(button_values.begin(), 8);

    vector<Color> button_colors = m_game.get_random_color_sequence(m_choice_count - 1);
    button_colors.insert(button_colors.begin(), ORANGE);
    
    // Ensure all the vectors are constructed to the same proper size.
    GAME_ASSERT(
        (button_positions.size() == m_choice_count) &&
        (button_values.size() == m_choice_count) &&
        (button_colors.size() == m_choice_count),
        "Not all button construction vectors are of the class-defined size (m_choice_count)."
    );

    // Get 3 iterators for each of these vectors, and *it++ them throughout the loop.
    vector<Vector2>::iterator positions_it = button_positions.begin();
    vector<int>::iterator values_it = button_values.begin();
    vector<Color>::iterator colors_it = button_colors.begin();

    size_t buttons_created = 0;

    // Level number button.
    add_text_button(
        to_string(*values_it++),
        80,
        *colors_it++,
        *positions_it++
    );
    ++buttons_created;

    // Correct answer.
    m_correct_button = add_text_button(
        to_string(*values_it++),
        80,
        *colors_it++,
        *positions_it++
    ); 
    ++buttons_created;

    // All other incorrect choices.
    while (buttons_created != m_choice_count) {
        add_text_button(
            to_string(*values_it++),
            80,
            *colors_it++,
            *positions_it++
        ); 
        ++buttons_created;
    }

	for (button* btn : get_buttons()) {
		btn->add_trait(new grows_when_hovered());
	}
}

void level_eight::update()
{
    level::update();

    if (m_correct_button->is_pressed()) {
        m_game.set_next_level(new level_nine());
    }
    else {
        for (button* btn : get_buttons()) {
            if (btn->is_pressed()) {
                m_game.set_next_level(new level_lose());
            }
        }
    }
}

// ------------------------------------------------------------------------------------------ //
//                                          level 9.                                          //
// ------------------------------------------------------------------------------------------ //
level_nine::level_nine()
{
    //
    // Main UI elements (level title, directions, submit box).
    //
    add_simple_text(
        "level  ",
        80,
        ORANGE,
        {m_game.get_cw() - 4, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "Put the largest number in the box",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);

    this->m_submit_box = add_entity(
        new label(
            BLACK,
            WHITE,
            {250, 150},
            9,
            {m_game.get_cw(), m_game.get_ch() - 25},
            -10
        )
    );

    this->m_submit_button = add_ui_button("Submit");

    //
    // Button creation.
    //
    m_correct_button_layout.reserve(m_choice_count);

    vector<Vector2> button_positions = {
        {m_game.get_cw() + 122, m_game.get_ch() - 250},
        {m_game.get_cw() - 275, m_game.get_ch()},
        {m_game.get_cw() - 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 275, m_game.get_ch()}
    };

    // Get a sequence of 4 random numbers, then insert the level number (9) at the beginning.
    vector<int> button_values = m_game.get_random_sequence(4, 1, 8, {9});
    button_values.insert(button_values.begin(), 9);

    // Get a sequence of 4 random colors, then insert the level number's color (GOLD) at the beginning.
    vector<Color> button_colors = m_game.get_random_color_sequence(4);
    button_colors.insert(button_colors.begin(), ORANGE);
    
    // Ensure all the vectors are constructed to the same proper size.
    GAME_ASSERT(
        (button_positions.size() == m_choice_count) &&
        (button_values.size() == m_choice_count) &&
        (button_colors.size() == m_choice_count),
        "Not all button construction vectors are of the class-defined size (m_choice_count)."
    );

    // Get 3 iterators for each of these vectors, and *it++ them throughout the loop.
    vector<Vector2>::iterator positions_it = button_positions.begin();
    vector<int>::iterator values_it = button_values.begin();
    vector<Color>::iterator colors_it = button_colors.begin();

    while (m_correct_button_layout.size() < m_choice_count) {
        button* btn = add_text_button(
            to_string(*values_it++),
            80,
            *colors_it++,
            *positions_it++
        );
		btn->add_trait(new grows_when_hovered());
		btn->add_trait(new grabbable());
        m_correct_button_layout.push_back(btn);
    }
    
    // Sort based on button string values, left to right, greatest to least.
    sort(m_correct_button_layout.begin(), m_correct_button_layout.end(),
        [](button* a, button* b) {
            return stoi(a->get_text()) > stoi(b->get_text());
        }
    );
}

void level_nine::update()
{
    level::update();

    //
    // On submission, add every button inside of the submission box to a vector, then organize
    // their text object's text strings in the same order that they exist spatially. Save this
    // created number to a string and check if it matches 'm_correct_number'.
    //
    if (m_submit_button->is_pressed()) {

        vector<button*> numbers_in_box; // For buttons that are inside of the submission box.
        numbers_in_box.reserve(m_choice_count);

        for (button* btn : get_buttons()) {
            if (CheckCollisionRecs(btn->get_rectangle(), m_submit_box->get_rectangle())) {
                vector<button*>::iterator it = numbers_in_box.begin();
                while (it != numbers_in_box.end() && (*it)->get_position().x <= btn->get_position().x) {
                    ++it;
                }
                numbers_in_box.insert(it, btn);
            }
        }

        // Assume true and override if proven false. 
        bool answer_was_chosen = true;
        vector<button*>::iterator choice_it = numbers_in_box.begin();
        vector<button*>::iterator answer_it = m_correct_button_layout.begin();

        if (numbers_in_box.size() != m_correct_button_layout.size()) {
            answer_was_chosen = false;
        }

        while (choice_it != numbers_in_box.end() && answer_it != m_correct_button_layout.end()) {
            if ((*choice_it)->get_text() != (*answer_it)->get_text()) {
                answer_was_chosen = false;
            }
            ++choice_it;
            ++answer_it;
        }

        if (answer_was_chosen) {
            m_game.set_next_level(new level_ten());
        }
        else {
            m_game.set_next_level(new level_lose());
        }
    }
}

// ------------------------------------------------------------------------------------------ //
//                                          level 10.                                         //
// ------------------------------------------------------------------------------------------ //
level_ten::level_ten()
{
    //
    // Main UI elements (level title, directions, submit box).
    //
    add_simple_text(
        "level  ",
        80,
        ORANGE,
        {m_game.get_cw() - 4, m_game.get_ch() - 250},
        0
    );

    add_simple_text(
        "Put the largest number in the box",
        40,
        RAYWHITE,
        {m_game.get_cw(), m_game.get_ch() - 150},
        0
    )
    ->add_anim_rotate(0.0f, 4.0f, 1.5f);

    Vector2 submit_box_position = {m_game.get_cw(), m_game.get_ch() - 25};
    this->m_submit_box = add_entity(
        new button(
            new text("", 80, BLACK, BLACK, submit_box_position),
            WHITE,
            {250, 150},
            submit_box_position,
            -10
        )
    );
    m_submit_box->add_trait(new grabbable());

    this->m_submit_button = add_ui_button("Submit");

    //
    // Button creation.
    //
    m_correct_button_layout.reserve(m_choice_count);

    vector<Vector2> button_positions = {
        {m_game.get_cw() + 122, m_game.get_ch() - 250},
        {m_game.get_cw() - 275, m_game.get_ch()},
        {m_game.get_cw() - 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 175, m_game.get_ch() + 175},
        {m_game.get_cw() + 275, m_game.get_ch()}
    };

    // Get a sequence of 4 random numbers, sort them greatest to least, then insert the level
    // number (10) at the beginning.
    vector<int> button_values = m_game.get_random_sequence(4, 1, 99, {10});
    sort(button_values.begin(), button_values.end(),
        [](int a, int b) {
            return a < b;
        }
    );
    button_values.insert(button_values.begin(), 10);

    // Get a sequence of 4 random colors, then insert the level number's color (GOLD) at the beginning.
    vector<Color> button_colors = m_game.get_random_color_sequence(4);
    button_colors.insert(button_colors.begin(), ORANGE);
    
    // Ensure all the vectors are constructed to the same proper size.
    GAME_ASSERT(
        (button_positions.size() == m_choice_count) &&
        (button_values.size() == m_choice_count) &&
        (button_colors.size() == m_choice_count),
        "Not all button construction vectors are of the class-defined size (m_choice_count)."
    );

    // Get 3 iterators for each of these vectors, and *it++ them throughout the loop.
    vector<Vector2>::iterator positions_it = button_positions.begin();
    vector<int>::iterator values_it = button_values.begin();
    vector<Color>::iterator colors_it = button_colors.begin(); 

    while (m_correct_button_layout.size() < m_choice_count) {
        button* btn = add_text_button(
            to_string(*values_it++),
            80,
            *colors_it,
            *positions_it
        );  
        m_correct_button_layout.push_back(btn); 

        // Place an ice cube over every number except for the greatest to show that they are
        // not holdable.
        if (values_it != button_values.end()) {
            Vector2 ice_cube_size;
            int ice_cube_padding = 32;
            ice_cube_size.x = btn->get_rectangle().width + ice_cube_padding;
            ice_cube_size.y = btn->get_rectangle().height + ice_cube_padding;

            Vector2 ice_cube_position = *positions_it;
            ice_cube_position.x += 4;   // Accomodate for the shadow offset of the text.
            add_entity(
                new label(
                    {255, 255, 255, 75},
                    {75, 150, 255, 75},
                    ice_cube_size,
                    4,
                    ice_cube_position,
                    1
                )
            );
        }
        else {
            btn->add_trait(new grows_when_hovered());
            btn->add_trait(new grabbable());
        }

        ++colors_it;
        ++positions_it;
    } 

    // Erase everything except for the last two numbers, swap them so that they together create
    // the largest compound number, then make the larger of the two (the first) the holdable one.
    m_correct_button_layout.erase(
        m_correct_button_layout.begin(),
        m_correct_button_layout.begin() + m_choice_count - 2
    );
    std::swap(m_correct_button_layout.at(0), m_correct_button_layout.at(1));
    m_holdable_number = m_correct_button_layout.front();
}

void level_ten::update()
{
    level::update();

    //
    // On submission, add every button inside of the submission box to a vector, then organize
    // their text object's text strings in the same order that they exist spatially. Save this
    // created number to a string and check if it matches 'm_correct_number'.
    //
    if (m_submit_button->is_pressed()) {

        vector<button*> numbers_in_box; // For buttons that are inside of the submission box.
        numbers_in_box.reserve(m_choice_count);

        for (button* btn : get_buttons()) {
            if (btn == m_submit_button || btn == m_submit_box) { continue; }
            if (CheckCollisionRecs(btn->get_rectangle(), m_submit_box->get_rectangle())) {
                vector<button*>::iterator it = numbers_in_box.begin();
                while (it != numbers_in_box.end() && (*it)->get_position().x <= btn->get_position().x) {
                    ++it;
                }
                numbers_in_box.insert(it, btn);
            }
        }

        // Assume true and override if proven false. 
        bool answer_was_chosen = true;
        vector<button*>::iterator choice_it = numbers_in_box.begin();
        vector<button*>::iterator answer_it = m_correct_button_layout.begin();

        if (numbers_in_box.size() != m_correct_button_layout.size()) { answer_was_chosen = false; }

        while(choice_it != numbers_in_box.end() && answer_it != m_correct_button_layout.end()) {
            if ((*choice_it)->get_text() != (*answer_it)->get_text()) {
                answer_was_chosen = false;
            }
            ++choice_it;
            ++answer_it;
        }

        if (answer_was_chosen) {
            m_game.set_next_level(new level_win());
        }
        else {
            m_game.set_next_level(new level_lose());
        }
    }
}
