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

#include "Soundtrack.h"

#include <QFileInfo>
#include <QUrl>

#include <game/Parameters.h>

#ifdef Q_OS_SYMBIAN
#include <QMediaPlayer>
#include <QMediaPlaylist>
#endif

namespace
{
    QUrl relativePathToUrl(const std::string & path)
    {
        return QUrl::fromLocalFile(QFileInfo(QString::fromStdString(path)).absoluteFilePath());
    }
}

using namespace Gui;

Soundtrack * Soundtrack::_instance = NULL;

Soundtrack & Soundtrack::instance()
{
    if (_instance == NULL)
    {
        _instance = new Soundtrack(); 
    }
    return *_instance;
}

void Soundtrack::increaseVolume()
{
    if (mPlayer != NULL)
    {
#ifdef Q_OS_SYMBIAN
        mPlayer->setVolume(mPlayer->volume()+10);
        if (mPlayer->volume() >= 10 && mPlayer->state() == QMediaPlayer::PausedState)
        {
            mPlayer->play();
        }
#endif
    }
}

void Soundtrack::decreaseVolume()
{
    if (mPlayer != NULL)
    {
#ifdef Q_OS_SYMBIAN
        mPlayer->setVolume(mPlayer->volume()-10);
        if (mPlayer->volume() < 10 && mPlayer->state() == QMediaPlayer::PlayingState)
        {
            mPlayer->pause();
        }
#endif
    }
}

Soundtrack::~Soundtrack()
{

}

Soundtrack::Soundtrack()
    : QObject(NULL)
    , mPlayer(NULL)
{
#ifdef Q_OS_SYMBIAN
    mPlayer = new QMediaPlayer(this);
    QMediaPlaylist * playlist = new QMediaPlaylist(mPlayer);
    playlist->addMedia(relativePathToUrl(Game::Parameters::instance().getDataFilePath("sounds/Track1.mp3")));
    playlist->addMedia(relativePathToUrl(Game::Parameters::instance().getDataFilePath("sounds/Track2.mp3")));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    mPlayer->setPlaylist(playlist);
    mPlayer->setVolume(10);
    mPlayer->play();
#endif
}
