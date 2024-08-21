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

#ifndef DATABASE_H
#define DATABASE_H
#include "constants.h"
#include "mediaitem.h"

#include <QAbstractListModel>
#include <QtSql/QSqlDatabase>
#include <QFileInfo>
#include <QVector>
#include <QSqlRecord>
#include <QVariant>
#include <dirent.h>

/*
class Wifi : public QAbstractListModel
{
  Q_OBJECT
  public:
    // Wifi();
    explicit Wifi(QObject *parent = 0);
    Q_INVOKABLE QVariant scan();
//    Q_INVOKABLE int checkIfConnected();

//    QStringList* getDevice();
};
*/

class DataBase : public QAbstractListModel
{
    Q_OBJECT

public:

    enum AnimalRoles {
        OrderRole = Qt::UserRole + 1,
        IdRole,
        albumRole,
        artistRole,
        imageRole,
        TypeRole,
        PlayListRole
    };


    explicit DataBase(QObject *parent = 0);
    void updateDataBase(QStringList path, QList<bool> folderOption, int type);

    Q_INVOKABLE bool validateDB();
    Q_INVOKABLE void loadCovers();
    Q_INVOKABLE QString getPathTrack(int disc, int song);
    Q_INVOKABLE void setPaths(QStringList list, QList<bool> option);
    Q_INVOKABLE int getFoundAlbum(QString index, int type);
    Q_INVOKABLE QString storeList(int disc, QString song, QString nameList, QVariant varArray);
    Q_INVOKABLE QString storeHistory(int disc, QString song, QString nameList, int pos);
    Q_INVOKABLE QVariant get2PlayTable(int indexSong);
    Q_INVOKABLE QString autoplaylist(QString AutoList);
    Q_INVOKABLE QVariant scan(QVariant myList);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

signals:
    void stepLoading(int step);
    void stepMaxCount(int max);
    void stepLoadingDone();
    void setConfigText(bool b);
	
protected:
    QHash<int, QByteArray> roleNames() const;

private:
    bool artistFound(QString name);
    bool createDB();
    void deleteDB();
    bool filesIntoFolder(QString path);
    void initDB();
    void storeAll(QString path, MRockola::MediaType mType);
    void storeAll(QString path, QString albumName = "", MRockola::MediaType mType = MRockola::MediaNONE);
    void storeAlbum(QString name, QString path, QString imagen, int type);
    void storeArtist(QString name, int genere = 0);
    void storeTracks(QFileInfoList fileList);
    void searchMedia(QFileInfo folder, QString artist, MRockola::MediaType mediaType, int type);
    void searchMedia2 (DIR *parent, int level, QString path, QString artist, int type);

private:
    int             albumCount;
    int             artistIndex;
    QSqlDatabase    m_dbase;

    QVariantList        artistList; // Aponta para a tabela artist < populated under loadCovers() >
    QList<mediaItem>    m_covers; // Aponta pra alguma tabela...
    QList<getListAlbum> getIt;

};

#endif // DATABASE_H
