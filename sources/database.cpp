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
#include <QQuickWindow>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <QDebug>

#define CREATE_TABLE_ALBUM  "CREATE TABLE IF NOT EXISTS ALBUM (id integer primary key autoincrement, name varchar(256), path varchar(512), image varchar(512), artistId integer, type integer, FOREIGN KEY(artistId) REFERENCES artist(id))"
#define CREATE_TABLE_ARTIST "CREATE TABLE IF NOT EXISTS ARTIST (id integer primary key autoincrement, name varchar(256), genere integer)"
#define CREATE_TABLE_TRACK  "CREATE TABLE IF NOT EXISTS TRACK (id integer primary key autoincrement, albumId integer, name text, position integer, FOREIGN KEY(albumId) REFERENCES album(id))"
#define CREATE_TABLE_PLAYLIST "CREATE TABLE IF NOT EXISTS PLAYLIST (id integer primary key autoincrement, albumId integer, disc integer, song text, position integer,  FOREIGN KEY(albumId) REFERENCES album(id))"
#define CREATE_TABLE_HISTORY "CREATE TABLE IF NOT EXISTS HISTORY (id integer primary key autoincrement, albumId integer, disc integer, song text, position integer, daytime text, FOREIGN KEY(albumId) REFERENCES album(id))"
#define CREATE_TABLE_QUEUE "CREATE TABLE QUEUE (qid integer primary key autoincrement, song text, daytime text"

#define INSERT_ALBUM        "INSERT INTO ALBUM VALUES(NULL,'%1','%2','%3',%4,%5)"
#define INSERT_ARTIST       "INSERT INTO ARTIST VALUES(NULL,'%1',%2)"
#define INSERT_TRACK        "INSERT INTO TRACK VALUES(NULL, %1,'%2',%3)"
#define INSERT_PLAYLIST     "INSERT INTO PLAYLIST VALUES(NULL,'%1','%2','%3',%4)"
#define INSERT_HISTORY      "INSERT INTO HISTORY VALUES(NULL,'%1','%2','%3',%4, datetime(strftime('%s','now'), 'unixepoch', 'localtime') )"


#define GET_ALBUM_PATH      "SELECT path FROM ALBUM WHERE id = %1"
#define GET_ARTIST_NAME     "SELECT UPPER(name) FROM ARTIST WHERE id=%1"
#define GET_TRACK_NAME      "SELECT name FROM TRACK WHERE albumId=%1 AND position=%2"

// "select album.path, track.name from TRACK inner join ALBUM on track.albumId = album.id where track.position = %1 and album.id = %2"

#define GET_ALL_ALBUM       "SELECT id, UPPER(name), path, image, artistId, type FROM ALBUM  ORDER BY ABS(type), artistId, UPPER(name)"
#define GET_ALL_TRACKS      "SELECT name FROM TRACK WHERE albumId=%1"

#define DELETE_ALBUM        "DROP TABLE ALBUM"
#define DELETE_ARTIST       "DROP TABLE ARTIST"
#define DELETE_TRACK        "DROP TABLE TRACK"

//search
#define GET_ALBUM_INDEX     "SELECT album.id, UPPER(artist.name) FROM ARTIST, ALBUM WHERE UPPER(artist.name) LIKE '%1%2' AND album.artistId=artist.id AND type=%3  ORDER BY UPPER(artist.name)  ASC"
#define GET_ARTIST_ALLSONG  "SELECT  album.id, artist.name FROM ARTIST, ALBUM WHERE album.artistId=artist.id AND album.type=%1 GROUP BY artist.name ORDER BY MIN(album.id)"
#define GET_ARTIST_INDEX    "SELECT  album.id, artist.name FROM ARTIST, ALBUM WHERE album.artistId=artist.id AND album.type = :type GROUP BY artist.name ORDER BY MIN(album.id)"
#define GET_ARTIST    "SELECT artistId FROM ALBUM WHERE id = %1"


DataBase::DataBase(QObject *parent) :
    QAbstractListModel(parent)
{

}
/***
* Teste do QVARIANT
* passa pra lah... e devolve!
*/
QVariant DataBase::scan(QVariant myList) {
  QVariantList varlist;
  QVariant var;

  qDebug("Inside function");
  varlist.append("Test1 \n");
  varlist.append("Test2 \n");
  varlist.append("SAPORRA AI \n");
  // varlist.insert(varlist.length(), {{1,2,3}} );

  for (int k = 0; k < myList.toStringList().length(); ++k) {
      qDebug() << myList.toStringList()[k];
  }

  var = QVariant(varlist);

  return var;
}
//------------------------------------------------------------------------------

bool DataBase::createDB() {
    deleteDB();
    m_dbase.open();
    QSqlQuery query;
    bool ret = (query.exec(CREATE_TABLE_ARTIST)&& query.exec(CREATE_TABLE_ALBUM) &&
                query.exec(CREATE_TABLE_TRACK) && query.exec(CREATE_TABLE_PLAYLIST) &&
                query.exec(CREATE_TABLE_HISTORY) );
    m_dbase.close();
    return ret;
}

//------------------------------------------------------------------------------

void DataBase::deleteDB() {
    m_dbase.open();
    QSqlQuery query;
    query.exec(DELETE_ARTIST);
    query.exec(DELETE_ALBUM);
    query.exec(DELETE_TRACK);
    query.exec("DROP TABLE PLAYLIST");
    m_dbase.close();
 }

//------------------------------------------------------------------------------

QString DataBase::getPathTrack(int disc, int song)
{
    m_dbase.open();

    QSqlQuery query0;
    // Checar se existe como sendo o _Playlist (cadastrado como artista)
    query0.exec(QString( GET_ARTIST ).arg(disc));
    query0.next();
    QString artist = query0.value(0).toString();

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
    QString path(QDir::homePath()+"/tmp");
    // QString path("/home/vmadmin/tmp");
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
    // initDB();
    createDB();
    initDB();
    albumCount = 0;
    m_dbase.open();
    m_dbase.transaction();

    for (int k = 0; k < path.size(); ++k)
    {
        QDir folder(path.at(k));
        qDebug("UPDATE DB FUNCTION");
        if ( folder.exists() )
        {
            QStringList nameFilter;
            nameFilter << "*.mp3" << "*.MP3" << "*.mP3";
            QFileInfoList list = folder.entryInfoList( nameFilter, QDir::Files );
            qDebug("CHEGOU AQUI MP3");
            QFileInfoList folderList =  folder.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name );
            qDebug("FOI DEPOIS");
            /*
            QFileInfoList folderList = folder.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot |
                                                            QDir::NoSymLinks, QDir::Name | QDir::IgnoreCase);
                                                            */

            MRockola::MediaType mediaType = (folderOption.at(k)? MRockola::MediaVideoKaraoke :
                                                                  MRockola::MediaNONE);
            // qDebug("FOLDER OPTIONS");
            // qDebug() << folderOption.at(k);
            // qDebug("MEDIA LIST");
            // qDebug() << folderList.at(0);
            qDebug("MEDIA TYPE");
            qDebug() << mediaType;

            emit stepMaxCount( folderList.size() * path.size());
            for ( int i = 0; i < folderList.size(); ++i ) {
                emit stepLoading(i);
                QByteArray myPath = folderList.at(i).path().toLocal8Bit();
                DIR *path = opendir(myPath.data());
                qDebug() << folder.dirName();
                qDebug() << "expurgos folder.dirname";
                qDebug() << myPath;
                // searchMedia(folderList.at(i), folder.dirName(), mediaType, type);
                searchMedia2 (path, 0, folderList.at(i).path(), folder.dirName(), mediaType);
            } //end for
        }
    }
    m_dbase.commit();
    m_dbase.close();
    artistList.clear();
    loadCovers();
    emit stepLoadingDone();
}

/***
* em 25/07/2024 -> função para substituir searchMedia
*
*
*/
void DataBase::searchMedia2 (DIR *parent, int level, QString path, QString artist, int type) {
    struct dirent *ent;
    if (!parent) {
        return;
    }
    while ((ent = readdir(parent)) != nullptr) {
        if ((strcmp(ent->d_name, ".") == 0) ||
            (strcmp(ent->d_name, "..") == 0)) {
            continue;
        }

        int parent_fd = dirfd(parent);
        if (parent_fd < 0) {
            perror("dirfd");
            continue;
        }
        int fd = openat(parent_fd, ent->d_name, O_RDONLY | O_DIRECTORY);
        if (fd != -1) { // Directory
            // printf("%*s%s/\n", level, "", ent->d_name);
            // qDebug() << level << ent->d_name;
            DIR *child = fdopendir(fd);
            if (child) {
                searchMedia2(child, level + 1, path +"/"+ ent->d_name, ent->d_name, type);
                closedir(child);
            } else
                perror("fdopendir");
        } else if (errno == ENOTDIR) { // Regular file
            // printf("%*s%s\n", level, "", ent->d_name);
            // path = path + "/" + ent->d_name;
            // qDebug() << path;
            QString fileExt = QString(ent->d_name).split(".")[1];

            if(filesIntoFolder(path)) {
                if(fileExt.contains("mp3"))
                    qDebug() << "ENTROU no mp3";
                    // storeTracks()
                else
                    if ( fileExt.contains("png") or fileExt.contains("jpg") ) {
                        QString artistName = artist; // tava fora daqui... no mundao...
                        QString albumName = "";
                        if(type == 1) {
                            artistName = path.section("-", 0, 0).trimmed();
                            albumName = path.section("-", -1, -1).trimmed();
                        }
                        if(artistFound(artistName) == 0)
                            storeArtist(artistName); // trouxe pra ca pra testar
                        qDebug() << "entrou no PNG" << level << fileExt;
                        QStringList hPath = path.split("/");
                        albumName = hPath[hPath.length()-1];
                        artist = hPath[hPath.length()-2];
                        qDebug() << "artist ->" <<  artist;
                        qDebug() << "album ->" << albumName << level;
                        storeAlbum(albumName, path, QString(ent->d_name), type);
                    }
            } // fim checa path

        } else
            perror("openat");

    } // end while
} //end void

/***
* Inda num entendi como funciona essa m... sei que é recursiva...
* 24/07/2024 -> serve para separar os albuns por diretorio ou por nome separado por hifen
* o paramentro mais importante é o objeto com o caminho completo com o nome do mp3.
*/
void DataBase::searchMedia(QFileInfo folder, QString artist, MRockola::MediaType mediaType, int type)
{
    /*
    QFileInfoList folderList =  QDir(folder.absoluteFilePath()).entryInfoList(QDir::Dirs |
                                                                                        QDir::NoDotAndDotDot |
                                                                                        QDir::NoSymLinks,
                                                                                        QDir::Name |
                                                                                        QDir::IgnoreCase);
    */
    // Essa aqui eh minha linha de teste... ja dah crash de cara...
    QFileInfoList folderList =  QDir(folder.absoluteFilePath()).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );

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
            qDebug() << "Parada do storeAll" << albumName;
            qDebug() << folder.absoluteFilePath();
        }
    }
    else
    {
        for ( int i = 0; i < folderList.size(); ++i )
        {
            // qDebug() << folderList.at(i);
            qDebug("ENTROU NO FOR .. size");
            searchMedia(folderList.at(i), folderList.at(i).absoluteDir().dirName(), mediaType, type);
            qDebug("I %d ->", i);
            qDebug() << folderList.at(i);
            qDebug() << folderList.at(i).absoluteDir().dirName();
        }
    }
}
/******
* alguma coisa relacionada com a posicao no paginador...
*
*/
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

    while(inf<=sup) {
        centro=(sup+inf)/2;
        if(artistList.at(centro) == name) {
            artistIndex = centro + 1;
            qDebug() << "TAH NO ARTIST FOUND";
            qDebug() << artistList.at(centro).toString();
            return true;
        }
        else
            if(name < artistList.at(centro).toString() )
                sup=centro-1;
            else
                inf=centro+1;
    } // end while
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
    qDebug() << "Saida path esperada ->"  << path;
    qDebug() << "MediaType ->" << mType;
    qDebug() << "fileImg.size ->" << fileImg.size();
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
    // query.exec( QObject::trUtf8 ( INSERT_ARTIST ).arg(name.replace("'","''")).arg(genere));
    query.exec( QString::fromLatin1( INSERT_ARTIST ).arg(name.replace("'","''")).arg(genere));
    artistList << name;
}

//------------------------------------------------------------------------------ esse ja tinha

void DataBase::storeAlbum(QString name, QString path, QString image, int type) {
    QSqlQuery query(m_dbase);

    if(query.exec( QObject::trUtf8( INSERT_ALBUM ).arg(name.replace("'","''")).arg(path).arg(image).arg(artistIndex).arg(type)))
    {
        ++albumCount;
    }
}

//------------------------------------------------------------------------------

void DataBase::storeTracks(QFileInfoList fileList) {
    int size = (fileList.size() > MAX_TRACKS_LIST? MAX_TRACKS_LIST : fileList.size());
    QSqlQuery query(m_dbase);

    for(int pos = 0; pos < size; ++pos)  {
      query.exec(QObject::trUtf8(INSERT_TRACK).arg(albumCount).arg(fileList.at(pos).fileName().replace("'","''")).arg(pos + 1)); //start one (zero reserve for play all album song))
    }
}

/***
* Funcao para armazenar no nome do album (gerar a capinha com mixtape)
*
*/
QString DataBase::storeList(int disc, QString song, QString nameList, QVariant varArray) {
    int albumThing = 0;
    int pos = varArray.toStringList()[0].toInt();
    int order = varArray.toStringList()[1].toInt();
    QStringList title = song.split("/"); // Musica com o path completo
    // name.sprintf("%+06.2f", albumCount);
    m_dbase.open();
    QSqlQuery query(m_dbase);

    // Executar o split do PLAYLIST_xx aqui para usar o storeTrack..
    QRegExp rx("\\_");
    QStringList strId = nameList.split(rx);

    if (disc == 0)
      song = song.simplified(); // num faz nada. E so pra pular a query de busca do album...
    else {
        query.exec( QString( "SELECT id FROM ALBUM WHERE name = '%1'" ).arg(nameList) );
        query.next();
        albumThing = query.value(0).toInt();
        // query.value(0).isNull()

        // Checar se existe como sendo o _Playlist (cadastrado como artista)
        query.exec(QString( "SELECT id FROM ARTIST WHERE name like 'Ziggfly%'" ));
        query.next();
        artistIndex = query.value(0).toInt();
        if (albumThing == 0)
          storeAlbum(nameList, QString(QDir::homePath()+"/tmp"), QString("playlist_cover1.png"), 4);
        else
          song = song.simplified(); //song = "song"; // num faz nada dinovo
        // qDebug("ALBUM THING: ");
        // qDebug() << albumThing;
        // qDebug("SONG THING: ");
        // qDebug() << song;
      // PLAYLIST (id , albumId , disc , song , position )
    } // fim do else

    // save playlist --> aqui a variavel ORDER das capinhas fica na tabela no lugar de pos (field position)
    query.exec( QObject::trUtf8( INSERT_PLAYLIST ).arg(strId[1]).arg(disc).arg(song).arg(order));
    // start one (zero reserve for play all album song))
    query.exec(QObject::trUtf8(INSERT_TRACK).arg(strId[1]).arg(title[title.length()-1].replace("'","''")).arg(pos));
    m_dbase.close();

    return nameList.toUpper(); // name+"|"+QString::number(artistId);

}

/***
* Funcao para gerar nome automatico do album.
*
*/
QString DataBase::autoplaylist(QString AutoList) {

    if (AutoList == "none") {
      m_dbase.open();
      QSqlQuery query(m_dbase);

      // query.exec(QString( "SELECT max(rowid) FROM ALBUM" ));
      // query que pega o autoincrement...
      query.exec(QString( "SELECT seq+1 FROM sqlite_sequence WHERE name = 'ALBUM' " ));
      query.next();
      // QString name = "PLAYLIST_"+QString::number(albumCount);
      m_dbase.close();
      return "PLAYLIST_"+query.value(0).toString();
    }
    else
      return AutoList;
}

//------------------------------------------------------------------------------

/***
* Funcao para armazenar o history
*
*/
QString DataBase::storeHistory(int disc, QString song, QString nameList, int pos) {
    int albumThing = 0;

    m_dbase.open();
    QSqlQuery query(m_dbase);

    if (disc == 0)
      song = "song"; // num faz nada. E so pra pular a query de busca do album...
    else {
      query.exec( QString( "SELECT id FROM ALBUM WHERE name = '%1'" ).arg(nameList) );
      query.next();
      albumThing = query.value(0).toInt();

      // HISTORY (id , albumId , disc , song , position )

    }
    // salvar na tabela history
    query.exec( QObject::trUtf8( INSERT_HISTORY ).arg(disc).arg(disc).arg(song).arg(pos));

    m_dbase.close();

    return QString::number(albumThing);

}

//---------------------------------------------
/***
* Funcao exportar playlist a partir de albumId para ser chamada em qml (CoverDetails)
*
*  SELECT albumId, disc, song, position, ALBUM.artistId, ARTIST.name FROM PLAYLIST inner join ALBUM on playlist.disc = album.id inner join ARTIST on ALBUM.artistId = ARTIST.id;
*/
QVariant DataBase::get2PlayTable(int indexSong) {
    QString sSong;
    QVariantList tmp;
    m_dbase.open();
    QSqlQuery query;
    QVariantList varlist;
    QVariant var;
    // QString::number(myNumber).rightJustified(5, '0');

    // if(query.exec( QObject::trUtf8( "SELECT song FROM PLAYLIST where albumId = '%1'" ).arg(indexSong))) {
    if(query.exec( QObject::trUtf8( "SELECT albumId, disc, song, ARTIST.name, position, ALBUM.path, ALBUM.image FROM PLAYLIST inner join ALBUM on playlist.disc = album.id inner join ARTIST on ALBUM.artistId = ARTIST.id where PLAYLIST.albumId = '%1'" ).arg(indexSong))) {
        // query.first();
        while (query.next()) {
            // varlist.append(query.value(0).toString());
            varlist.append(query.value(0).toString()+";"+query.value(1).toString()+";"+
                           query.value(2).toString()+";"+query.value(3).toString()+";"+
                           query.value(4).toString().rightJustified(4, '0')+";file://"+
                           query.value(5).toString()+"/"+query.value(6).toString() );
            // QVariantList{5, 5, 6, 7}
        }
        var = QVariant(varlist);
        for ( int i=0; i < varlist.length(); i++) {
           // qDebug(varlist[i].toList()[0].toInt());
           qDebug("numRows %d:", query.numRowsAffected());
        }
        m_dbase.close();
        sSong = QString::number(tmp.count());
        qDebug("q merda %d", sSong.toInt());
        getIt << getListAlbum(tmp);
    } // FIM SE
    else
      sSong = "LONELY";

    return var;
} // FIM get2Play



bool DataBase::validateDB() {
    QString path(QDir::homePath());
    path.append(QDir::separator()).append(DBASE_NAME);
    path = QDir::toNativeSeparators(path);

    QFile file(path);
    if(file.exists()) {
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
