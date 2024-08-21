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

#include "keyboard.h"
#include <QKeySequence>

Keyboard::Keyboard()
{

}

Keyboard::Keyboard(QString hash, QString action, Qt::Key key, uint code)
{
    sAction = action;
    uKey = key;
    sHash = hash;
    uCode = code;
}

uint Keyboard::getCode()
{
    return uCode;
}

QString Keyboard::getActionText() const
{
    return sAction;
}

uint Keyboard::getKey()
{
    return uKey;
}

QString Keyboard::getKeyString() const
{
    return QKeySequence(uKey).toString();
}

QString Keyboard::getHash() const
{
    return sHash;
}


void Keyboard::setKey(uint key)
{
    uKey = key;
}
