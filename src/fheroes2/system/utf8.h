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

// UTF-8 utilities independent from font rendering.
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace fheroes2
{
    namespace utf8
    {
        // Decode a single UTF-8 codepoint from a byte buffer.
        // Reads up to 4 bytes starting at 'data'. Returns the decoded codepoint
        // and sets 'consumed' to the number of bytes read (1-4).
        // On invalid or incomplete UTF-8, returns U+FFFD replacement character.
        // In this case 'consumed' indicates how many bytes the caller should
        // skip before calling decode() again.
        uint32_t decode( const char * data, size_t size, size_t & consumed );

        // Encode a Unicode codepoint into UTF-8 bytes.
        // Writes 1 to 4 bytes to 'buf' (must have space for at least 5 bytes).
        // Returns the number of bytes written, or 0 if the codepoint is invalid.
        int encode( uint32_t codepoint, char * buf );

        // ASCII-only case conversion on UTF-8 encoded text.
        // Only modifies bytes in the ASCII range (0x00-0x7F). Multibyte UTF-8
        // sequences are passed through unchanged.
        std::string tolowerAscii( std::string_view text );

        // Returns true if the codepoint is a CJK Unified Ideograph.
        // Checks CJK Unified Ideographs (U+4E00-U+9FFF), CJK Extension A
        // (U+3400-U+4DBF), and CJK Compatibility Ideographs (U+F900-U+FAFF).
        // This does not cover CJK Extension B/C/D and other supplementary ranges.
        bool isCommonCJK( uint32_t codepoint );
    }
}
