/*
 * MRockola.
 * Copyright (C) 2011-2016  Edgar Armando Lopez Hernandez
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

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QtCore/QStringList>

#define MAX_LENGTH          50
#define MAX_TRACKS_LIST     42

#define AV_REMOVE_NUMBERS QRegExp("^\\d\\d?")

#define DBASE_NAME  "mrockola.db"

#define D_TRACK  0

namespace MRockola
{
    enum ActionScreen
    {
        MainScreen,
        VideoScreen
    };

    enum MediaType
    {
        MediaMusic,
        MediaVideo,
        MediaKaraokeCDG,
        MediaVideoKaraoke,
        MediaNONE
    };

    enum Shortcut
    {
        CoverLeft,
        CoverRight,
        Enter,
        Return,
        BackSpace,
        AlbumList,
        SkipSong,
        VolumenUp,
        VolumenDown,
        FullScreen,
        QuitApplication,
        RemoveAll,
        Up,
        Down,
        Credits,
        Shortcut_max
    };

    enum DataStruct
    {
        FileId,
        FileName,
        FilePath,
        FileImage,
        FileArtist,
        FileType
    };

    enum Languages
    {
        LanguageEn,
        LanguageEs
    };

    enum Finders
    {
        defaultFinder,
        listFinder
    };

    QStringList audioVideoFileExt();
    QStringList hotKeys();
    QStringList imageFileExt();
    QStringList karaokeFileExt();
    QStringList soundFileExt();
    QStringList videoFileExt();
}
#endif // CONSTANTS_H
