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

#include "database.h"
#include <QDir>
#include <QtSql/QSqlQuery>
#include <QTextStream>

#define CREATE_TABLE_ALBUM  "CREATE TABLE IF NOT EXISTS ALBUM (id integer primary key autoincrement, name varchar(256), path varchar(512), image varchar(512), artistId integer, type integer, FOREIGN KEY(artistId) REFERENCES artist(id))"
#define CREATE_TABLE_ARTIST "CREATE TABLE IF NOT EXISTS ARTIST (id integer primary key autoincrement, name varchar(256), genere integer)"
#define CREATE_TABLE_TRACK  "CREATE TABLE IF NOT EXISTS TRACK (id integer primary key autoincrement, albumId integer, name text, position integer, FOREIGN KEY(albumId) REFERENCES album(id))"

#define INSERT_ALBUM        "INSERT INTO ALBUM VALUES(NULL,'%1','%2','%3',%4,%5)"
#define INSERT_ARTIST       "INSERT INTO ARTIST VALUES(NULL,'%1',%2)"
#define INSERT_TRACK        "INSERT INTO TRACK VALUES(NULL, %1,'%2',%3)"

#define GET_ALBUM_PATH      "SELECT path FROM ALBUM WHERE id=%1"
#define GET_ARTIST_NAME     "SELECT UPPER(name) FROM ARTIST WHERE id=%1"
#define GET_TRACK_NAME      "SELECT name FROM TRACK WHERE albumId=%1 AND position=%2"

#define GET_ALL_ALBUM       "SELECT id, UPPER(name), path, image, artistId, type FROM ALBUM  ORDER BY ABS(type), artistId, UPPER(name)"
#define GET_ALL_TRACKS      "SELECT name FROM TRACK WHERE albumId=%1"

#define DELETE_ALBUM        "DROP TABLE ALBUM"
#define DELETE_ARTIST       "DROP TABLE ARTIST"
#define DELETE_TRACK        "DROP TABLE TRACK"

//search
#define GET_ALBUM_INDEX     "SELECT album.id, UPPER(artist.name) FROM ARTIST, ALBUM WHERE UPPER(artist.name) LIKE '%1%2' AND album.artistId=artist.id AND type=%3  ORDER BY UPPER(artist.name)  ASC"
#define GET_ARTIST_ALLSONG  "SELECT  album.id, artist.name FROM ARTIST, ALBUM WHERE album.artist=artist.id AND album.type=%1 GROUP BY artist.name ORDER BY MIN(album.id)"
#define GET_ARTIST_INDEX    "SELECT  album.id, artist.name FROM ARTIST, ALBUM WHERE album.artistId=artist.id AND album.type = :type GROUP BY artist.name ORDER BY MIN(album.id)"


DataBase::DataBase(QObject *parent) :
    QAbstractListModel(parent)
{

}

//------------------------------------------------------------------------------

bool DataBase::createDB()
{
    deleteDB();
    m_dbase.open();
    QSqlQuery query;
    bool ret = (query.exec(CREATE_TABLE_ARTIST)&& query.exec(CREATE_TABLE_ALBUM) &&
                query.exec(CREATE_TABLE_TRACK) );
    m_dbase.close();
    return ret;
}

//------------------------------------------------------------------------------

void DataBase::deleteDB()
{
    m_dbase.open();
    QSqlQuery query;
    query.exec(DELETE_ARTIST);
    query.exec(DELETE_ALBUM);
    query.exec(DELETE_TRACK);
    m_dbase.close();
 }

//------------------------------------------------------------------------------

QString DataBase::getPathTrack(int disc, int song)
{
    m_dbase.open();
    QSqlQuery query;
    query.exec(QString( GET_ALBUM_PATH ).arg(disc));
    query.next();
    QString path = query.value(0).toString();
    
    QSqlQuery query2;
    query2.exec(QString( GET_TRACK_NAME ).arg(disc).arg(song + 1));  //1++ index init on zero
    query2.next();
    QString track = query2.value(0).toString();
    m_dbase.close();
    return path+"/"+track;
}

//------------------------------------------------------------------------------

void DataBase::initDB()
{
    if(m_dbase.isValid())
        return;
    
    m_dbase = QSqlDatabase::addDatabase("QSQLITE");
    QString path(QDir::homePath());
    path.append(QDir::separator()).append(DBASE_NAME);
    path = QDir::toNativeSeparators(path);
    m_dbase.setDatabaseName(path);
}

//------------------------------------------------------------------------------

void DataBase::loadCovers()
{
    initDB();

    m_dbase.open();    
    m_covers.clear();

    QSqlQuery queryDisc;
    queryDisc.prepare(GET_ALL_ALBUM);
    queryDisc.setForwardOnly(true);
    queryDisc.exec();

    if(!queryDisc.first())
    {
       emit setConfigText(true);
       m_dbase.close();
       return;         
    }   

    emit setConfigText(false);
    QSqlQuery queryCover;
    queryCover.setForwardOnly(true);
    queryCover.prepare( "SELECT name FROM TRACK WHERE albumId = :album");

    QSqlQuery qryArtist;
    qryArtist.setForwardOnly(true);
    qryArtist.prepare("SELECT UPPER(name) FROM ARTIST WHERE id = :artistId");

    int i = 0;
    
    do    
    {
        QVariantList list;
        queryCover.bindValue(":album", queryDisc.value(MRockola::FileId).toInt());
        queryDisc.setForwardOnly(true);
        queryCover.exec();

        while( queryCover.next() )
        {
            QString str = queryCover.value(D_TRACK).toString().toLower();
            str.chop(4);
            str.remove(AV_REMOVE_NUMBERS);
            int index = str.lastIndexOf(QChar('-'));

            if(index != -1)
                str.remove(0, index + 1);

            if(str.size() > MAX_LENGTH)
                str.resize(MAX_LENGTH);
                
            list << str;
        }

        qryArtist.bindValue(":artistId", queryDisc.value(MRockola::FileArtist).toInt());
        queryDisc.setForwardOnly(true);
        qryArtist.exec();
        //QCoreApplication::processEvents();
        qryArtist.next();///???
        QString artist = qryArtist.value(0).toString();
                  
        beginInsertRows(QModelIndex(), rowCount(), rowCount());

        if( queryDisc.value(MRockola::FileImage).toString().compare("NONE") == 0 )
        {
            m_covers << mediaItem(i, queryDisc.value(MRockola::FileId).toInt(),
                                artist, queryDisc.value(MRockola::FileName).toString(),
                                queryDisc.value(MRockola::FileImage).toString(),
                                queryDisc.value(MRockola::FileType).toInt(), list);
        }
        else
        {
            m_covers << mediaItem(i, queryDisc.value(MRockola::FileId).toInt(),
                                artist, queryDisc.value(MRockola::FileName).toString(),
                                queryDisc.value(MRockola::FilePath).toString()+"/"+queryDisc.value(MRockola::FileImage).toString(),
                                queryDisc.value(MRockola::FileType).toInt(), list);
        }

        endInsertRows();

        ++i;
     } while( queryDisc.next() );
     

    m_dbase.commit();
    m_dbase.close();
}

//------------------------------------------------------------------------------

int DataBase::getFoundAlbum(QString index, int type)
{
    m_dbase.open();
    QSqlQuery query;
    query.exec( QString(GET_ALBUM_INDEX).arg(index).arg("%").arg(type));
    query.next();
    int ret = query.value(0).toInt();
    m_dbase.close();
    return ret;
}

//------------------------------------------------------------------------------

void DataBase::updateDataBase(QStringList path, QList<bool> folderOption, int type)
{
    emit stepLoading(0); //reset progressBar
    initDB();
    createDB();
    albumCount = 0;
    m_dbase.open();
    m_dbase.transaction();

    for (int k = 0; k < path.size(); ++k)
    {
        QDir folder(path.at(k));
        if ( folder.exists() )
        {
            QFileInfoList folderList = folder.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot |
                                                            QDir::NoSymLinks, QDir::Name | QDir::IgnoreCase);

            MRockola::MediaType mediaType = (folderOption.at(k)? MRockola::MediaVideoKaraoke :
                                                                  MRockola::MediaNONE);

            emit stepMaxCount( folderList.size() * path.size());
            for ( int i = 0; i < folderList.size(); ++i )
            {
                 emit stepLoading(i);
                 searchMedia(folderList.at(i), folder.dirName(), mediaType, type);
            }
        }
    }
    m_dbase.commit();
    m_dbase.close();
    artistList.clear();
    loadCovers();
    emit stepLoadingDone();
}

//------------------------------------------------------------------------------

void DataBase::searchMedia(QFileInfo folder, QString artist, MRockola::MediaType mediaType, int type)
{

    QFileInfoList folderList =  QDir(folder.absoluteFilePath()).entryInfoList(QDir::Dirs |
                                                                                        QDir::NoDotAndDotDot |
                                                                                        QDir::NoSymLinks,
                                                                                        QDir::Name |
                                                                                        QDir::IgnoreCase);

    if( folderList.isEmpty())
    {
        if(filesIntoFolder(folder.absoluteFilePath()))
        {
            QString artistName = artist;
            QString albumName = "";

            if(type == 1)
            {
                artistName = (folder.fileName()).section("-", 0, 0).trimmed();
                albumName = (folder.fileName()).section("-", -1, -1).trimmed();
            }

            if(artistFound(artistName) == 0)
            {
                storeArtist(artistName);
            }

            storeAll(folder.absoluteFilePath(), albumName, mediaType);
        }
    }
    else
    {
        for ( int i = 0; i < folderList.size(); ++i )
        {
            searchMedia(folderList.at(i), folderList.at(i).absoluteDir().dirName(), mediaType, type);
        }
    }
}


//------------------------------------------------------------------------------

bool DataBase::artistFound(QString name)
{
    int centro;
    int inf = 0;
    int sup = artistList.count() - 1;

    if(artistList.count() == 0)
    {
        artistIndex = artistList.count() + 1;
        return false;
    }

    while(inf<=sup)
    {
        centro=(sup+inf)/2;
        if(artistList.at(centro) == name)
        {
            artistIndex = centro + 1;
            return true;
        }
        else if(name < artistList.at(centro).toString() )
        {
            sup=centro-1;
        }
        else
        {
           inf=centro+1;
        }
    }
       artistIndex = artistList.count() + 1;
       return false;
}


//------------------------------------------------------------------------------

bool DataBase::filesIntoFolder(QString path)
{
    QDir folder(path);
    QFileInfoList fileList = folder.entryInfoList(MRockola::audioVideoFileExt(),
                                                  QDir::Files | QDir::NoDotAndDotDot,
                                                  QDir::Name | QDir::IgnoreCase);
    return !fileList.isEmpty();
}

//------------------------------------------------------------------------------

void DataBase::storeAll(QString path, MRockola::MediaType mType)
{
    storeAll(path, "", mType);
}

//------------------------------------------------------------------------------

void DataBase::storeAll(QString path, QString albumName, MRockola::MediaType mType)
{
    QDir intoDir(path);
    QString setAlbumName = (albumName.isEmpty()? intoDir.dirName() : albumName );
    
    QFileInfoList fileImg = intoDir.entryInfoList(MRockola::imageFileExt(), QDir::Files | QDir::NoDotAndDotDot |
                                        QDir::Hidden, QDir::Name | QDir::IgnoreCase);
    
    QFileInfoList  fileList = intoDir.entryInfoList(MRockola::karaokeFileExt(), QDir::Files |
                                        QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
    
    if ( !fileList.isEmpty() )
    {
        fileList = intoDir.entryInfoList(MRockola::soundFileExt(), QDir::Files | QDir::NoDotAndDotDot,
                                        QDir::Name | QDir::IgnoreCase);     
        storeAlbum(setAlbumName, path, (fileImg.size()? fileImg.first().fileName() : QString("NONE")), MRockola::MediaKaraokeCDG);
        storeTracks(fileList);
    } 
    else
    {
        fileList = intoDir.entryInfoList(MRockola::soundFileExt(), QDir::Files | QDir::NoDotAndDotDot,
                                        QDir::Name | QDir::IgnoreCase);     
        if ( !fileList.isEmpty() )
        {
            storeAlbum(setAlbumName, path, (fileImg.size()? fileImg.first().fileName() : QString("NONE")), MRockola::MediaMusic);
            storeTracks(fileList);
        }
    }
       
    fileList = intoDir.entryInfoList(MRockola::videoFileExt(), QDir::Files | QDir::NoDotAndDotDot,
                                        QDir::Name | QDir::IgnoreCase);
    if ( !fileList.isEmpty() )
    {
        if(mType == MRockola::MediaVideoKaraoke)
        {
            storeAlbum(setAlbumName, path, (fileImg.size()? fileImg.first().fileName() : QString("NONE")), MRockola::MediaKaraokeCDG);
        }
        else 
        {
            storeAlbum(setAlbumName, path, (fileImg.size()? fileImg.first().fileName() : QString("NONE")), MRockola::MediaVideo);
        }
        storeTracks(fileList);
    }
}

//------------------------------------------------------------------------------

void DataBase::storeArtist(QString name, int genere)
{
    //++artistIndex;
    QSqlQuery query(m_dbase);
    query.exec( QObject::trUtf8 ( INSERT_ARTIST ).arg(name.replace("'","''")).arg(genere));
    artistList << name;
}

//------------------------------------------------------------------------------

void DataBase::storeAlbum(QString name, QString path, QString image, int type)
{
    QSqlQuery query(m_dbase);

    if(query.exec( QObject::trUtf8( INSERT_ALBUM ).arg(name.replace("'","''")).arg(path).arg(image).arg(artistIndex).arg(type)))
    {
        ++albumCount;
    }
}

//------------------------------------------------------------------------------

void DataBase::storeTracks(QFileInfoList fileList)
{
    int size = (fileList.size() > MAX_TRACKS_LIST? MAX_TRACKS_LIST : fileList.size());
    QSqlQuery query(m_dbase);

    for(int pos = 0; pos < size; ++pos)
    {
        query.exec(QObject::trUtf8(INSERT_TRACK).arg(albumCount).arg(fileList.at(pos).fileName().replace("'","''")).arg(pos + 1)); //start one (zero reserve for play all album song))
    }
}

//------------------------------------------------------------------------------

bool DataBase::validateDB()
{
    QString path(QDir::homePath());
    path.append(QDir::separator()).append(DBASE_NAME);
    path = QDir::toNativeSeparators(path);

    QFile file(path);
    if(file.exists())
    {
        initDB();
        loadCovers();
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------

int DataBase::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_covers.size();
}

//------------------------------------------------------------------------------

QHash<int, QByteArray> DataBase::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[OrderRole] = "order";
    roles[IdRole] = "id";
    roles[albumRole] = "album";
    roles[artistRole] = "artist";
    roles[imageRole] = "imagen";
    roles[TypeRole] = "type";
    roles[PlayListRole] = "playlist";
    return roles;
}

//------------------------------------------------------------------------------

QVariant DataBase::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_covers.count())
        return QVariant();

    const mediaItem &covers = m_covers[index.row()];
    if (role == OrderRole)
        return covers.getAlbumId();
    else if (role == IdRole)
        return covers.getId();
    else if(role == albumRole)
        return covers.getAlbum();
    else if(role == artistRole)
        return covers.getArtist();
    else if(role == imageRole)
        return covers.getImage();
    else if(role == TypeRole)
        return covers.getType();
    else if(role == PlayListRole)
        return covers.getListTrack();
    return QVariant();
}

//------------------------------------------------------------------------------

void DataBase::setPaths(QStringList list,  QList<bool> option)
{
    updateDataBase(list, option, 0);
}
