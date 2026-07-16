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
#include <string_view>

#include "font_types.h"

namespace fheroes2
{
    class Sprite;
    enum class SupportedLanguage : uint8_t;

    class FontManager
    {
    public:
        static FontManager & instance();

        // Switch font alphabet to the given language (by enum).
        void switchLanguage( SupportedLanguage language );

        // Switch font alphabet to the given language (by abbreviation, e.g. "fr").
        void switchLanguage( std::string_view languageAbbreviation );

        // -- Character sprite and metrics queries --

        const Sprite & getCharSprite( uint32_t codepoint, const FontType & fontType ) const;

        int32_t getCharWidth( uint32_t codepoint, const FontType & fontType ) const;

        bool isCharAvailable( uint32_t codepoint, const FontType & fontType ) const;

        uint32_t getCharacterLimit( FontSize fontSize ) const;

        int32_t getLineHeight( FontSize fontSize ) const;

        int32_t getSpaceWidth( FontSize fontSize ) const;

    private:
        FontManager() = default;

        FontManager( const FontManager & ) = delete;
        FontManager( FontManager && ) = delete;
        FontManager & operator=( const FontManager & ) = delete;
        FontManager & operator=( FontManager && ) = delete;
    };
}
