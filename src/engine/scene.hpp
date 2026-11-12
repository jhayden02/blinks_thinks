/***********************************************************************************************
*
*   scene.hpp - The library for creating, loading, and unloading scenes.
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
#include "entity.hpp"
#include "animation.hpp"
#include "background.hpp"
#include "overlay.hpp"
#include "text.hpp"
#include "box.hpp"
#include "button.hpp"

// Standard library.
#include <string>
#include <vector>
#include <type_traits>
#include <functional>
#include <cstddef>

// Source.
using engine::text;

// Standard library.
using std::function;
using std::vector;
using std::size_t;
using std::string;
using std::vector;
using std::is_base_of;

namespace engine
{

class game;

// ------------------------------------------------------------------------------------------ //
//                                           scene                                            //
// ------------------------------------------------------------------------------------------ //
class scene
{
    public:
        scene();

        virtual ~scene();

        virtual void update();

        virtual void draw();

        template <typename T>
        T* add_entity(T* ent)
        {
            static_assert(is_base_of<entity, T>::value, "T must derive from entity.");

            auto it = m_entities.begin();
            while (it != m_entities.end() && (*it)->get_layer() <= ent->get_layer()) {
                ++it;
            }
            m_entities.insert(it, ent);

            return ent;
        }

        template <typename T>
        vector<T*> get_entities()
        {
            static_assert(is_base_of<entity, T>::value, "T must derive from entity.");

            vector<T*> result;
            for (entity* ent : m_entities) {
                if (T* casted = dynamic_cast<T*>(ent)) {
                    result.push_back(casted);
                }
            }
            return result;
        }

        text* add_simple_text(
            string text,
            float font_size,
            Color text_color,
            Vector2 position,
            int layer
        );

        button* add_ui_button(string text);

        button* add_text_button(string text, int font_size, Color text_color, Vector2 position);

        // Creates a level-number button at the fixed top-right level_num_position with
        // ORANGE coloring. Does NOT register the button with m_grouped_buttons.
        button* add_level_num_button(int num);

        // Generates 'count' unique random values in [min_value, max_value] excluding
        // any value in excluded_values, creates a text_button for each at initial
        // position {0, 0} with a random color, registers each with m_grouped_buttons
        // (triggering a single rearrangement), and returns them in creation order.
        vector<button*> add_grouped_buttons(
            size_t count,
            int min_value,
            int max_value,
            const vector<int>& excluded_values
        );

        // Registers an externally-constructed button with m_grouped_buttons and
        // triggers a rearrangement.
        void add_to_group(button* btn);

        button* get_button_in_hand() { return m_button_in_hand; }
        void set_button_in_hand(button* btn) { m_button_in_hand = btn; }

    protected:
        game& m_game;

    private:
        vector<entity*> m_entities;

        button* m_button_in_hand;

        // Buttons whose positions are auto-managed. Members are arranged in a single
        // horizontal row centered on m_grouped_buttons_center; membership changes trigger
        // rearrangement.
        vector<button*> m_grouped_buttons;

        // The anchor point around which m_grouped_buttons are laid out.
        static const Vector2 m_grouped_buttons_center;

        // Horizontal spacing between adjacent buttons in the row.
        static inline const float m_grouped_buttons_spacing = 150.0f;

        // The maximum number of buttons the row can hold.
        static inline const size_t m_grouped_buttons_max = 4;

        // Recomputes the position of every button in m_grouped_buttons based on its
        // current size. Snap-instant; no animation.
        void rearrange_group();
};

// ------------------------------------------------------------------------------------------ //
//                                       scene_manager                                        //
// ------------------------------------------------------------------------------------------ //
class scene_manager
{
    public:
        scene_manager();
        ~scene_manager();

        scene_manager(const scene_manager&) = delete;
        scene_manager& operator=(const scene_manager&) = delete;
        scene_manager(scene_manager&&) = delete;
        scene_manager& operator=(scene_manager&&) = delete;

        // Performs the queued scene swap and ticks the current scene.
        void update();

        // Draws the current scene.
        void draw();

        // Advances to the next scene in m_advance.
        void advance();

        // Queues the lose scene without changing the advance index.
        void lose();

        // Jumps to the title scene in m_advance.
        void goto_title();

        // Jumps to the restart-point scene in m_advance.
        void goto_restart_point();

        scene* get_current_scene() { return m_current_scene; }

    private:
        vector<function<scene*()>> m_advance;
        function<scene*()> m_lose;
        size_t m_index;
        size_t m_title_index;
        size_t m_restart_point_index;
        scene* m_current_scene;
        scene* m_next_scene;
};

} // NAMESPACE ENGINE.
