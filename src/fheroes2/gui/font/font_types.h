/***************************************************************************
 *   fheroes2: https://github.com/ihhub/fheroes2                           *
 *   Copyright (C) 2026                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#pragma once

#include <cstdint>

namespace fheroes2
{
    enum class FontSize : uint8_t
    {
        SMALL,
        NORMAL,
        LARGE,
        // These are special fonts designed only for buttons. Use WHITE font color for Good Interface and GRAY for Evil Interface.
        BUTTON_RELEASED,
        BUTTON_PRESSED,
    };

    enum class FontColor : uint8_t
    {
        WHITE,
        GRAY,
        YELLOW,
        GOLDEN_GRADIENT,
        SILVER_GRADIENT,
    };

    struct FontType
    {
        FontType() = default;

        constexpr FontType( const FontSize size_, const FontColor color_ )
            : size( size_ )
            , color( color_ )
        {
            // Do nothing.
        }

        FontSize size = FontSize::NORMAL;
        FontColor color = FontColor::WHITE;

        static constexpr FontType normalWhite()
        {
            return { FontSize::NORMAL, FontColor::WHITE };
        }

        static constexpr FontType normalYellow()
        {
            return { FontSize::NORMAL, FontColor::YELLOW };
        }

        static constexpr FontType smallWhite()
        {
            return { FontSize::SMALL, FontColor::WHITE };
        }

        static constexpr FontType smallYellow()
        {
            return { FontSize::SMALL, FontColor::YELLOW };
        }

        static constexpr FontType largeWhite()
        {
            return { FontSize::LARGE, FontColor::WHITE };
        }

        static constexpr FontType buttonReleasedWhite()
        {
            return { FontSize::BUTTON_RELEASED, FontColor::WHITE };
        }
    };

}
