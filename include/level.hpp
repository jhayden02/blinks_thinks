/***********************************************************************************************
* 
*   level.hpp - The library for creating, loading, and unloading levels. 
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
#include "label.hpp"
#include "button.hpp"

using engine::text;

// Standard library.
#include <string>
#include <vector>
#include <type_traits>

using std::string;
using std::vector;
using std::is_base_of;

namespace engine
{

class game;

class level
{
    public:
        level();

        virtual ~level();

        virtual void update();

        virtual void draw();

        vector<button*> get_buttons() { return m_buttons; }

        template <typename T>
        T* add_entity(T* ent)
        {
            static_assert(is_base_of<entity, T>::value, "T must derive from entity.");

            auto it = m_entities.begin();
            while (it != m_entities.end() && (*it)->get_layer() <= ent->get_layer()) {
                ++it;
            }
            m_entities.insert(it, ent);

            if constexpr (is_base_of<button, T>::value) {
                m_buttons.push_back(static_cast<button*>(ent));
            }

            return ent;
        }

        text* add_simple_text(string text, float font_size, Color text_color, Vector2 position,
                            int layer);

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

    protected:
        game& m_game;

    private:
        vector<entity*> m_entities;

        vector<button*> m_buttons;

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

} // NAMESPACE ENGINE.
