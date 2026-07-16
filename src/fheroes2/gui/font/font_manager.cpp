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

#include "font_manager.h"

#include <cassert>
#include <cstdint>

#include "font_bitmap.h"
#include "game_assets.h"
#include "image.h"
#include "ui_language.h"

namespace fheroes2
{
    FontManager & FontManager::instance()
    {
        static FontManager manager;
        return manager;
    }

    void FontManager::switchLanguage( const SupportedLanguage language )
    {
        const SupportedLanguage resourceLanguage = getResourceLanguage();

        // The original French assets replace several ASCII special characters with language-specific characters.
        // In the engine we use CP1252 for these characters.
        if ( ( language == SupportedLanguage::English ) && ( resourceLanguage == SupportedLanguage::French ) ) {
            // Force generate CP1252 alphabet when English language is selected for French assets.
            Assets::updateLanguageDependentResources( SupportedLanguage::French, false );
        }
        else {
            // To generate CP1252 alphabet for French assets we must assume that these assets are not original.
            // Also we force generation of Cyrillic symbols for Russian assets - it is done for consistency of fonts with different assets.
            const bool isOriginalResourceLanguage
                = ( language == SupportedLanguage::English )
                  || ( language == resourceLanguage && resourceLanguage != SupportedLanguage::French && resourceLanguage != SupportedLanguage::Russian );

            Assets::updateLanguageDependentResources( language, isOriginalResourceLanguage );
        }
    }

    void FontManager::switchLanguage( const std::string_view languageAbbreviation )
    {
        switchLanguage( getLanguageFromAbbreviation( std::string{ languageAbbreviation } ) );
    }

    const Sprite & FontManager::getCharSprite( const uint32_t codepoint, const FontType & fontType ) const
    {
        // Bitmap backend: only codepoints 0-255 are valid.
        // Characters beyond this range fall back to '?'.
        constexpr uint32_t invalidCodepoint = static_cast<uint32_t>( '?' );
        const uint8_t character = ( codepoint <= 255 ) ? static_cast<uint8_t>( codepoint ) : static_cast<uint8_t>( invalidCodepoint );

        return bitmap_font::getSprite( character, fontType );
    }

    int32_t FontManager::getCharWidth( const uint32_t codepoint, const FontType & fontType ) const
    {
        const Sprite & sprite = getCharSprite( codepoint, fontType );
        return sprite.x() + sprite.width();
    }

    bool FontManager::isCharAvailable( const uint32_t codepoint, const FontType & fontType ) const
    {
        if ( codepoint > 255 ) {
            return false;
        }

        return bitmap_font::isCharAvailable( static_cast<uint8_t>( codepoint ), fontType );
    }

    uint32_t FontManager::getCharacterLimit( const FontSize fontSize ) const
    {
        return bitmap_font::getCharacterLimit( fontSize );
    }

    int32_t FontManager::getLineHeight( const FontSize fontSize ) const
    {
        return bitmap_font::getLineHeight( fontSize );
    }

    int32_t FontManager::getSpaceWidth( const FontSize fontSize ) const
    {
        return bitmap_font::getSpaceWidth( fontSize );
    }
}
