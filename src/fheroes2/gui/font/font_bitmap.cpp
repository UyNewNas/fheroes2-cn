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

#include <cassert>
#include <cstdint>

#include "font_types.h"
#include "game_assets.h"
#include "icn.h"
#include "image.h"

namespace fheroes2
{
    namespace bitmap_font
    {
        namespace
        {
            const Sprite errorImage;
        }

        const Sprite & getSprite( const uint8_t character, const FontType & fontType )
        {
            switch ( fontType.size ) {
            case FontSize::SMALL:
                switch ( fontType.color ) {
                case FontColor::WHITE:
                    return Assets::getImage( ICN::SMALFONT, character );
                case FontColor::GRAY:
                    return Assets::getImage( ICN::GRAY_SMALL_FONT, character );
                case FontColor::YELLOW:
                    return Assets::getImage( ICN::YELLOW_SMALLFONT, character );
                default:
                    assert( 0 );
                    break;
                }
                break;
            case FontSize::NORMAL:
                switch ( fontType.color ) {
                case FontColor::WHITE:
                    return Assets::getImage( ICN::FONT, character );
                case FontColor::GRAY:
                    return Assets::getImage( ICN::GRAY_FONT, character );
                case FontColor::YELLOW:
                    return Assets::getImage( ICN::YELLOW_FONT, character );
                case FontColor::GOLDEN_GRADIENT:
                    return Assets::getImage( ICN::GOLDEN_GRADIENT_FONT, character );
                case FontColor::SILVER_GRADIENT:
                    return Assets::getImage( ICN::SILVER_GRADIENT_FONT, character );
                default:
                    assert( 0 );
                    break;
                }
                break;
            case FontSize::LARGE:
                switch ( fontType.color ) {
                case FontColor::WHITE:
                    return Assets::getImage( ICN::WHITE_LARGE_FONT, character );
                case FontColor::GOLDEN_GRADIENT:
                    return Assets::getImage( ICN::GOLDEN_GRADIENT_LARGE_FONT, character );
                case FontColor::SILVER_GRADIENT:
                    return Assets::getImage( ICN::SILVER_GRADIENT_LARGE_FONT, character );
                default:
                    assert( 0 );
                    break;
                }
                break;
            case FontSize::BUTTON_RELEASED:
                switch ( fontType.color ) {
                case FontColor::WHITE:
                    return Assets::getImage( ICN::BUTTON_GOOD_FONT_RELEASED, character );
                case FontColor::GRAY:
                    return Assets::getImage( ICN::BUTTON_EVIL_FONT_RELEASED, character );
                default:
                    assert( 0 );
                    break;
                }
                break;
            case FontSize::BUTTON_PRESSED:
                switch ( fontType.color ) {
                case FontColor::WHITE:
                    return Assets::getImage( ICN::BUTTON_GOOD_FONT_PRESSED, character );
                case FontColor::GRAY:
                    return Assets::getImage( ICN::BUTTON_EVIL_FONT_PRESSED, character );
                default:
                    assert( 0 );
                    break;
                }
                break;
            default:
                assert( 0 );
                break;
            }

            assert( 0 );

            return errorImage;
        }

        uint32_t getCharacterLimit( const FontSize fontSize )
        {
            switch ( fontSize ) {
            case FontSize::SMALL:
                return Assets::getImageCount( ICN::SMALFONT );
            case FontSize::NORMAL:
            case FontSize::LARGE:
                return Assets::getImageCount( ICN::FONT );
            case FontSize::BUTTON_RELEASED:
            case FontSize::BUTTON_PRESSED:
                return Assets::getImageCount( ICN::BUTTON_GOOD_FONT_RELEASED );
            default:
                assert( 0 );
            }

            return 0;
        }

        int32_t getLineHeight( const FontSize fontSize )
        {
            switch ( fontSize ) {
            case FontSize::SMALL:
                return 8 + 2 + 1;
            case FontSize::NORMAL:
                return 13 + 3 + 1;
            case FontSize::LARGE:
                return 26 + 6 + 1;
            case FontSize::BUTTON_RELEASED:
            case FontSize::BUTTON_PRESSED:
                return 15;
            default:
                assert( 0 );
            }

            return 0;
        }

        int32_t getSpaceWidth( const FontSize fontSize )
        {
            switch ( fontSize ) {
            case FontSize::SMALL:
                return 4;
            case FontSize::NORMAL:
                return 6;
            case FontSize::LARGE:
            case FontSize::BUTTON_RELEASED:
            case FontSize::BUTTON_PRESSED:
                return 8;
            default:
                assert( 0 );
            }

            return 0;
        }

        bool isCharAvailable( const uint8_t character, const FontType & fontType )
        {
            if ( character == ' ' || character == '\n' ) {
                return true;
            }

            if ( character < 0x21 ) {
                return false;
            }

            const uint32_t limit = getCharacterLimit( fontType.size );
            return character <= limit;
        }
    }
}
