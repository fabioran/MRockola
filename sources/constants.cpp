/*
 * MRockola.
 * Copyright (C) 2011-2017  Edgar Armando Lopez Hernandez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more open details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: inge_lopez@yahoo.com
 */
#include "constants.h"


QStringList MRockola::audioVideoFileExt()
{
    QStringList list;
    list  << "*.mp3"
          << "*.wav"
          << "*.ogg"
          << "*.mpg"
          << "*.avi"
          << "*.flv"
          << "*.wmv";

    return list;
}

//------------------------------------------------------------------------------

QStringList MRockola::hotKeys()
{
    QStringList list;
    list << "Scroll Left:"
         << "Scroll Right:"
         << "Enter Selection:"
         << "Back Screen:"
         << "Backspace:"
         << "Finder (open/close):"
         << "Skip and Next song:"
         << "Volume Up:"
         << "Volume Down:"
         << "Enable/Disable Full Screen: "
         << "Quit:"
         << "Remove all songs:"
         << "Up:"
         << "Down:"
         << "Credits:";

    return list;
}

//------------------------------------------------------------------------------

QStringList MRockola::imageFileExt()
{
    QStringList list;
    list << "*.jpg"
         << "*.png";

    return list;
}

//------------------------------------------------------------------------------

QStringList MRockola::karaokeFileExt()
{
    QStringList list;
    list << "*.cdg";

    return list;
}

//------------------------------------------------------------------------------

QStringList MRockola::soundFileExt()
{
    QStringList list;
    list << "*.mp3"
         << "*.wav"
         << "*.ogg"
         << "*.wma";

    return list;
}

//------------------------------------------------------------------------------

QStringList MRockola::videoFileExt()
{
    QStringList list;
        list << "*.mpg"
             << "*.avi"
             << "*.flv"
             << "*.wmv"
             << "*.mp4";

    return list;
}

