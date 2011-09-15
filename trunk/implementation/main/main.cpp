// Symbian 4X Space Game
// 
// Copyright (C) 2011 Patrick Pelletier
// 
// This program is free software: you can redistribute it and/or modify it
// under the  terms of the GNU  General Public License as published by the
// Free Software Foundation, either version 3 of the License, or any later
// version.
// 
// This  program  is distributed  in  the  hope  that  it will  be  useful,
// but  WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
// Public License for more details.
// 
// You should have received a copy  of the GNU General Public License along
// with this program. See <http://www.opensource.org/licenses/gpl-3.0.html>

#include <game/Universe.h>
#include <game/Events.h>
#include <game/Player.h>
#include <game/Parameters.h>

#include <gui/CSSHelper.h>
#include <gui/MainWindow.h>

#include <data/AssetSerializer.h>

#include <QApplication>

int main(int argc, char **)
{
    bool ret = 0;

#ifdef Q_OS_SYMBIAN
    Game::Parameters::instance().setDataFilesPath("./data");
#else
    Game::Parameters::instance().setDataFilesPath("../data");
#endif

    Data::AssetSerializer::load(Game::Parameters::instance().getDataFilePath("configs/parameters.json"), Game::Parameters::instance());

    QApplication a(argc, NULL);

    Game::Universe::instance();
    Game::Player::Current::instance();
    Gui::MainWindow::instance();

    Gui::CSSHelper css;
    css.addStyle("mainwindow.css");
    //css.apply(Gui::MainWindow::instance());

    QString styleSheet = QString::fromStdString(css.style());//Gui::MainWindow::instance().styleSheet();
    styleSheet.replace("%images%", QString::fromStdString(Game::Parameters::instance().getDataFilePath("images")));
    Gui::MainWindow::instance().setStyleSheet(styleSheet);

#ifdef Q_OS_SYMBIAN
    Gui::MainWindow::instance().showFullScreen();
#else
    Gui::MainWindow::instance().resize(640,360);
    Gui::MainWindow::instance().show();
#endif

    ret = a.exec();

    delete &Gui::MainWindow::instance();
    delete &Game::Player::Current::instance();
    delete &Game::Universe::instance();
    delete &Game::Events::instance();
    delete &Game::Parameters::instance();

    return ret;
}
