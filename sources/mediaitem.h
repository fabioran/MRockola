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

#ifndef MEDIAITEM_H
#define MEDIAITEM_H

#include <QObject>
#include <QVariantList>

class mediaItem
{

public:
    explicit mediaItem();

    mediaItem(int order ,int id, QString album, QString artist, QString imagenName, int type, QVariantList playlist);

    int   getId() const;
    int   getType() const;

    QString getAlbum() const;
    QString getAlbumId() const;
    QString getArtist() const;
    QString getImage() const;
    QVariantList getListTrack() const;
    QString getTrack(int id) const;


private:
    QString      s_albumName;
    QString      s_artistName;
    QString      s_cover;
    QString      stringId;
    QVariantList s_playList;

    int           m_id;
    int           m_type;
};

#endif // MEDIAITEM_H
