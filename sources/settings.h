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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QAbstractTableModel>
#include <QMap>
#include <QSettings>
#include "keyboard.h"

#define  MRockolaSettings  QSettings settings("Mghik Technology","MRockola")


class Settings : public QAbstractTableModel
{
    Q_OBJECT

public:

    explicit Settings(QObject *parent = 0);
    void saveSettings();

    Q_INVOKABLE int validateKey(uint key);
    Q_INVOKABLE void changeKey(int row, uint key);

    Q_ENUMS(KEYBOARD)
    enum KEYBOARD {
        KEY_ENTER = 1,
        KEY_BACK,
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_SKIP,
        KEY_CONFIG,
        KEY_SEARCH,
        KEY_VOLUMENUP,
        KEY_VOLUMENDOWN,
        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_DELETE
    };

    enum AnimalRoles {
        strAction = Qt::UserRole,
        keyRole
    };


    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    void loadSettings();
    QHash<int, QByteArray> m_roleNames;

    QList<Keyboard> m_keyboard;
};

#endif // SETTINGS_H
