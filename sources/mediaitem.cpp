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

#include "mediaitem.h"


mediaItem::mediaItem()
{
}

//------------------------------------------------------------------------------

mediaItem::mediaItem(int order, int id, QString artist, QString album, QString imagenName, int type, QVariantList playlist)
{
    m_id = id;
    stringId = QString(QString("%1").arg(order, 4, 10, QChar('0'))); //format 0000 .. 9999);
    s_artistName = artist;
    s_albumName = album;
    s_playList = playlist;
    s_cover = imagenName;
    m_type = type;
}

//------------------------------------------------------------------------------

QString mediaItem::getTrack(int id) const
{
    id-=1;  //song initialize with 1 in the database
    if(s_playList.size() > id)
        return s_playList.at(id).toString();

    return  NULL;
}

//------------------------------------------------------------------------------

QString mediaItem::getArtist() const
{
    return s_artistName;
}

//------------------------------------------------------------------------------

QString mediaItem::getAlbum() const
{
    return s_albumName;
}

//------------------------------------------------------------------------------

QString mediaItem::getAlbumId() const
{
    return stringId;
}

//------------------------------------------------------------------------------

int mediaItem::getId() const
{
    return m_id;
}

//------------------------------------------------------------------------------

QString mediaItem::getImage() const
{
    return s_cover;
}

//------------------------------------------------------------------------------

QVariantList mediaItem::getListTrack() const
{
    return s_playList;
}


//------------------------------------------------------------------------------

int mediaItem::getType() const
{
    return m_type;
}
