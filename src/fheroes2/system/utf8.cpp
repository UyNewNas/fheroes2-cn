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

#include "utf8.h"

#include <array>
#include <cassert>

namespace
{
    constexpr uint32_t replacementCharacter = 0xFFFD;

    // Character lookup table for ASCII tolower, compatible with single-byte code pages.
    const std::array<unsigned char, 128> tolowerLUT
        = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
            ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
            '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
            'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '[', '\\', ']', '^', '_',
            '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
            'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', 0x7F };
}

namespace fheroes2
{
    namespace utf8
    {
        uint32_t decode( const char * data, const size_t size, size_t & consumed )
        {
            if ( data == nullptr || size == 0 ) {
                consumed = 0;
                return replacementCharacter;
            }

            const auto * bytes = reinterpret_cast<const uint8_t *>( data );
            const uint8_t byte1 = bytes[0];

            // ASCII: 0xxxxxxx
            if ( byte1 < 0x80 ) {
                consumed = 1;
                return byte1;
            }

            // Determine sequence length from leading 1-bits.
            int seqLen = 0;
            uint8_t mask = 0x80;

            while ( byte1 & mask ) {
                ++seqLen;
                mask >>= 1;
            }

            // Overlong encoding or unexpected byte (10xxxxxx without prefix).
            if ( seqLen < 2 || seqLen > 4 ) {
                consumed = 1;
                return replacementCharacter;
            }

            // Not enough bytes remaining.
            if ( static_cast<size_t>( seqLen ) > size ) {
                consumed = size;
                return replacementCharacter;
            }

            // Extract data bits from the first byte.
            uint32_t codepoint = byte1 & ( ( 1u << ( 7 - seqLen ) ) - 1 );

            // Validate and accumulate continuation bytes (10xxxxxx).
            for ( int i = 1; i < seqLen; ++i ) {
                const uint8_t cont = bytes[i];

                if ( ( cont & 0xC0 ) != 0x80 ) {
                    consumed = static_cast<size_t>( i );
                    return replacementCharacter;
                }

                codepoint = ( codepoint << 6 ) | ( cont & 0x3F );
            }

            // Reject overlong encodings.
            if ( seqLen == 2 && codepoint < 0x80 ) {
                consumed = static_cast<size_t>( seqLen );
                return replacementCharacter;
            }
            if ( seqLen == 3 && codepoint < 0x800 ) {
                consumed = static_cast<size_t>( seqLen );
                return replacementCharacter;
            }
            if ( seqLen == 4 && codepoint < 0x10000 ) {
                consumed = static_cast<size_t>( seqLen );
                return replacementCharacter;
            }

            // Reject codepoints above U+10FFFF.
            if ( codepoint > 0x10FFFF ) {
                consumed = static_cast<size_t>( seqLen );
                return replacementCharacter;
            }

            // Reject surrogate pairs (U+D800-U+DFFF).
            if ( codepoint >= 0xD800 && codepoint <= 0xDFFF ) {
                consumed = static_cast<size_t>( seqLen );
                return replacementCharacter;
            }

            consumed = static_cast<size_t>( seqLen );
            return codepoint;
        }

        int encode( const uint32_t codepoint, char * buf )
        {
            // buf must point to at least 4 writable bytes.
            assert( buf != nullptr );

            if ( codepoint <= 0x7F ) {
                buf[0] = static_cast<char>( codepoint );
                return 1;
            }

            if ( codepoint <= 0x7FF ) {
                buf[0] = static_cast<char>( 0xC0 | ( codepoint >> 6 ) );
                buf[1] = static_cast<char>( 0x80 | ( codepoint & 0x3F ) );
                return 2;
            }

            if ( codepoint <= 0xFFFF ) {
                if ( codepoint >= 0xD800 && codepoint <= 0xDFFF ) {
                    return 0;
                }

                buf[0] = static_cast<char>( 0xE0 | ( codepoint >> 12 ) );
                buf[1] = static_cast<char>( 0x80 | ( ( codepoint >> 6 ) & 0x3F ) );
                buf[2] = static_cast<char>( 0x80 | ( codepoint & 0x3F ) );
                return 3;
            }

            if ( codepoint <= 0x10FFFF ) {
                buf[0] = static_cast<char>( 0xF0 | ( codepoint >> 18 ) );
                buf[1] = static_cast<char>( 0x80 | ( ( codepoint >> 12 ) & 0x3F ) );
                buf[2] = static_cast<char>( 0x80 | ( ( codepoint >> 6 ) & 0x3F ) );
                buf[3] = static_cast<char>( 0x80 | ( codepoint & 0x3F ) );
                return 4;
            }

            return 0;
        }

        // ASCII-only case conversion on UTF-8 encoded text.
        // Only modifies bytes in the ASCII range (0x00-0x7F). Non-ASCII bytes
        // are copied verbatim without UTF-8 validity checking.
        std::string tolowerAscii( const std::string_view text )
        {
            std::string result;
            result.reserve( text.size() );

            const auto * pos = reinterpret_cast<const uint8_t *>( text.data() );
            const auto * end = pos + text.size();

            while ( pos < end ) {
                const uint8_t byte = *pos;

                if ( byte < 0x80 ) {
                    result += static_cast<char>( tolowerLUT[byte] );
                    ++pos;
                }
                else {
                    // Multibyte UTF-8: pass through unchanged.
                    int skip = 1;
                    if ( ( byte & 0xE0 ) == 0xC0 ) {
                        skip = 2;
                    }
                    else if ( ( byte & 0xF0 ) == 0xE0 ) {
                        skip = 3;
                    }
                    else if ( ( byte & 0xF8 ) == 0xF0 ) {
                        skip = 4;
                    }

                    if ( pos + skip > end ) {
                        skip = static_cast<int>( end - pos );
                    }

                    result.append( reinterpret_cast<const char *>( pos ), skip );
                    pos += skip;
                }
            }

            return result;
        }

        bool isCommonCJK( const uint32_t codepoint )
        {
            // CJK Unified Ideographs
            if ( codepoint >= 0x4E00 && codepoint <= 0x9FFF ) {
                return true;
            }

            // CJK Unified Ideographs Extension A
            if ( codepoint >= 0x3400 && codepoint <= 0x4DBF ) {
                return true;
            }

            // CJK Compatibility Ideographs
            if ( codepoint >= 0xF900 && codepoint <= 0xFAFF ) {
                return true;
            }

            return false;
        }
    }
}
