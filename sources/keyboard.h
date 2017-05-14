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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>

#define ENTER_KEY   "key_enter"
#define BACK_KEY    "key_back"
#define UP_KEY      "key_up"
#define DOWN_KEY    "key_down"
#define LEFT_KEY    "key_left"
#define RIGHT_KEY   "key_rigth"
#define SKIP_KEY    "key_skip"
#define CONFIG_KEY  "key_config"
#define SEARCH_KEY  "key_search"
#define DELETE_KEY  "key_delete"
#define VOLUMEN_UP_KEY  "key_volumen_up"
#define VOLUMEN_DOWN_KEY  "key_volumen_down"


#define ENTER_STR   "Enter"
#define BACK_STR    "Back"
#define UP_STR      "Up"
#define DOWN_STR    "Down"
#define LEFT_STR    "Left"
#define RIGHT_STR   "Rigth"
#define SKIP_STR    "Skip"
#define CONFIG_STR  "Config"
#define SEARCH_STR  "Search"
#define DELETE_STR  "Delete"
#define VOLUMEN_UP_STR  "Volumen up"
#define VOLUMEN_DOWN_STR  "Volumen down"

class Keyboard
{

public:
    explicit Keyboard();
    Keyboard( QString hash, QString action, Qt::Key key, uint code);

    uint getCode();
    QString getActionText() const;
    uint getKey();
    QString getKeyString() const;
    QString getHash() const;

    void setKey(uint key);

private:
    QString sAction;
    uint    uKey;
    QString sHash;
    uint    uCode;
};

#endif // KEYBOARD_H
