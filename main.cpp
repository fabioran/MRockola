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
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: inge_lopez@yahoo.com
 */
#include "sources/database.h"
#include "sources/mediaplayer.h"
#include "sources/settings.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QDebug>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<DataBase>("MRockola.DBase", 1, 0, "DataBase");
    qmlRegisterType<MediaPlayer>("MRockola.MPlayer", 1, 0, "MediaPlayer");
    qmlRegisterType<Settings>("MRockola.Settings", 1, 0, "Settings");

    static QTranslator qTranslator;
    qTranslator.load("mrockola_" + QLocale::system().name(), ":/languages/");
    app.installTranslator(&qTranslator);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main/MRockola.qml")));

    return app.exec();
}
