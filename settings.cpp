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

#include "settings.h"
#include <QKeySequence>
#include <QDebug>
#include <QMessageBox>

Settings::Settings(QObject *parent) :
    QAbstractTableModel (parent)
{
    m_keyboard << Keyboard(ENTER_KEY, ENTER_STR, Qt::Key_Enter, KEY_ENTER)
    << Keyboard(BACK_KEY, BACK_STR, Qt::Key_Backspace, KEY_BACK)
    << Keyboard(UP_KEY, UP_STR, Qt::Key_Up, KEY_UP)
    << Keyboard(DOWN_KEY, DOWN_STR, Qt::Key_Down, KEY_DOWN)
    << Keyboard(LEFT_KEY, LEFT_STR, Qt::Key_Left, KEY_LEFT)
    << Keyboard(RIGHT_KEY, RIGHT_STR, Qt::Key_Right, KEY_RIGHT)
    << Keyboard(SKIP_KEY, SKIP_STR, Qt::Key_Space, KEY_SKIP)
    << Keyboard(CONFIG_KEY, CONFIG_STR, Qt::Key_F2, KEY_CONFIG)
    << Keyboard(SEARCH_KEY, SEARCH_STR, Qt::Key_F, KEY_SEARCH)
    << Keyboard(FULL_KEY, FULL_STR, Qt::Key_P, KEY_P)
    << Keyboard(RKEY_KEY, RKEY_STR, Qt::Key_R, KEY_R)
    << Keyboard(DELETE_KEY, DELETE_STR, Qt::Key_Delete, KEY_DELETE)
    << Keyboard(VOLUMEN_UP_KEY, VOLUMEN_UP_STR,  Qt::Key_Plus, KEY_VOLUMENUP)
    << Keyboard(VOLUMEN_DOWN_KEY, VOLUMEN_DOWN_STR,  Qt::Key_Minus, KEY_VOLUMENDOWN)
    << Keyboard("key_0", "", Qt::Key_0, KEY_0)
    << Keyboard("key_1", "", Qt::Key_1, KEY_1)
    << Keyboard("key_2", "", Qt::Key_2, KEY_2)
    << Keyboard("key_3", "", Qt::Key_3, KEY_3)
    << Keyboard("key_4", "", Qt::Key_4, KEY_4)
    << Keyboard("key_5", "", Qt::Key_5, KEY_5)
    << Keyboard("key_6", "", Qt::Key_6, KEY_6)
    << Keyboard("key_7", "", Qt::Key_7, KEY_7)
    << Keyboard("key_8", "", Qt::Key_8, KEY_8)
    << Keyboard("key_9", "", Qt::Key_9, KEY_9);

    loadSettings(); // Primeira coisa q ele faz...
    // saveSettings();
}

//------------------------------------------------------------------------------

int Settings::validateKey(uint key)
{
    QList<Keyboard>::iterator i;

    for(i = m_keyboard.begin(); i != m_keyboard.end(); ++i)
    {
        if(i->getKey() == key)
            return int(i->getCode());
    }
    qDebug() << "validateKey process";

    return 0;
}

//------------------------------------------------------------------------------

void Settings::changeKey(int row, uint key)
{
    beginResetModel();
    m_keyboard[row].setKey(key);
    endResetModel();

    saveSettings();
}

//------------------------------------------------------------------------------

void Settings::loadSettings()
{
    MRockolaSettings;

    QStringList keys = settings.allKeys();

    if(keys.isEmpty())
    {
        saveSettings();
        qDebug() << "[loadSettings] saveSettings empty";
        return;
    }
    qDebug() << "loadSettings from memory";

    settings.beginGroup("Keyboard");

    QList<Keyboard>::iterator i;

    for(i = m_keyboard.begin(); i != m_keyboard.end(); ++i)
    {
       i->setKey(settings.value(i->getHash(), -1).toUInt());
    }

    settings.endGroup();
}

//------------------------------------------------------------------------------

void Settings::saveSettings()
{
    MRockolaSettings;
    settings.beginGroup("Keyboard");

    QList<Keyboard>::iterator i;

    for(i = m_keyboard.begin(); i != m_keyboard.end(); ++i)
    {
       settings.setValue(i->getHash(), i->getKey());
    }
    qDebug() << "[saveSettings] Saved!";

    settings.endGroup();
}


//------------------------------------------------------------------------------

QHash<int, QByteArray> Settings::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[strAction] = "actionStr";
    roles[keyRole] = "actionKey";
    return roles;
}

//------------------------------------------------------------------------------

QVariant Settings::data(const QModelIndex &index, int role) const
{
    // Check boudaries
     if(index.column() < 0 ||
             columnCount() <= index.column() ||
             index.row() < 0 ||
             rowCount() <= index.row())
     {
         qDebug() << "Warning: " << index.row() << ", " << index.column();
         return QVariant();
     }


    const Keyboard &key = m_keyboard.at(index.row());
    switch(role)
    {
        case strAction:
            return key.getActionText();
        case keyRole:
            return key.getKeyString();
    }
    return QVariant();
}

//------------------------------------------------------------------------------

int Settings::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return KEY_0;//m_keyboard.size(); //show only modificable key
}

//------------------------------------------------------------------------------

int Settings::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;  //2 column
}
