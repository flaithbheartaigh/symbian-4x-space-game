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

#include "SectorItemModel.h"
#include "UniversePainter.h"
#include "MainWindow.h"

#include <game/StarSystem.h>
#include <game/Sector.h>
#include <game/Star.h>
#include <game/Planet.h>
#include <game/Warp.h>
#include <game/Player.h>
#include <game/Ship.h>
#include <game/UniverseVisitor.h>

#include <QList>
#include <QPainter>
#include <QPixmap>
#include <QAbstractItemView>
#include <QPixmapCache>

#include <algorithm>

using namespace Gui;

namespace
{
    const QList<QString> Headers = QList<QString>() << "Name";
}

class SectorItemModel::Item
{

public:

    ~Item()
    {

    }

    Item()
        : mName()
        , mStar(NULL)
        , mPlanet(NULL)
        , mWarp(NULL)
        , mShip(NULL)
    {

    }

    Item(Game::Star * star)
        : mName()
        , mStar(star)
        , mPlanet(NULL)
        , mWarp(NULL)
        , mShip(NULL)
    {
        if (mStar != NULL)
        {
            mName = mStar->name();
        }
    }

    Item(Game::Planet * planet)
        : mName()
        , mStar(NULL)
        , mPlanet(planet)
        , mWarp(NULL)
        , mShip(NULL)
    {
        if (mPlanet != NULL)
        {
            mName = mPlanet->name();
        }
    }

    Item(Game::Ship * ship)
        : mName()
        , mStar(NULL)
        , mPlanet(NULL)
        , mWarp(NULL)
        , mShip(ship)
    {
        if (mShip != NULL)
        {
            mName = mShip->name();
        }
    }

    Item(Game::Warp * warp)
        : mName()
        , mStar(NULL)
        , mPlanet(NULL)
        , mWarp(warp)
        , mShip(NULL)
    {
        if (mWarp != NULL && mWarp->destination().sector() != NULL && mWarp->destination().sector()->starSystem() != NULL)
        {
            mName = warp->destination().sector()->starSystem()->name();
        }
    }

    const std::string & name() const
    {
        return mName;
    }

    Game::Planet * planet() const
    {
        return mPlanet;
    }

    Game::Star * star() const
    {
        return mStar;
    }

    Game::Ship * ship() const
    {
        return mShip;
    }

    Game::Warp * warp() const
    {
        return mWarp;
    }

private:

    std::string mName;

    Game::Star * mStar;

    Game::Planet * mPlanet;

    Game::Warp * mWarp;

    Game::Ship * mShip;
};

SectorItemModel::~SectorItemModel()
{

}

SectorItemModel::SectorItemModel(QObject * parent, Game::Sector * sector)
    : QAbstractTableModel(parent)
    , mItems()
{
    setSupportedDragActions(Qt::CopyAction);
    setSector(sector);
}

void SectorItemModel::setSector(Game::Sector * sector)
{
    mItems.clear();

    class PrivateSectorVisitor
        : private Game::UniverseVisitor
    {

    public:

        ~PrivateSectorVisitor()
        {

        }

        PrivateSectorVisitor(Game::Sector * sector, std::vector<Item> * items)
            : Game::UniverseVisitor()
            , mItems(items)
        {
            if (sector != NULL)
            {
                sector->accept(this);
            }
        }

    private:

        void visit(Game::Star * star)
        {
            if (mItems != NULL)
            {
                mItems->push_back(Item(star));
            }
        }
        
        void visit(Game::Planet * planet)
        {
            if (mItems != NULL && (MainWindow::Settings_ViewUniverse || Game::Universe::instance().game().currentPlayer()->knows(planet->sector()->starSystem())))
            {
                mItems->push_back(Item(planet));
            }
        }

        void visit(Game::Warp * warp)
        {
            if (mItems != NULL && (MainWindow::Settings_ViewUniverse || Game::Universe::instance().game().currentPlayer()->knows(warp->sector()->starSystem())))
            {
                mItems->push_back(Item(warp));
            }
        }

        void visit(Game::Ship * ship)
        {
            if (mItems != NULL && (MainWindow::Settings_ViewUniverse || ship->player() == Game::Universe::instance().game().currentPlayer() || Game::Universe::instance().game().currentPlayer()->knows(ship->sector()->starSystem())))
            {
                mItems->push_back(Item(ship));
            }
        }

        std::vector<Item> * mItems;

    } visitor(sector, &mItems);

    layoutChanged();
}

Qt::DropActions SectorItemModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags SectorItemModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
    {
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    }
    return Qt::ItemIsDropEnabled | defaultFlags;
}


QVariant SectorItemModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return Headers[section];
    }
    return QVariant();
}

int SectorItemModel::columnCount(const QModelIndex & parent) const
{
    return Headers.size();
}

int SectorItemModel::rowCount(const QModelIndex & parent) const
{
    return mItems.size();
}

QVariant SectorItemModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() >= rowCount())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        QVariant variant;
        switch (index.column())
        {
            case 0:
                QString name = QString::fromStdString(mItems[index.row()].name());
                if (mItems[index.row()].planet() != NULL)
                {
                    if (mItems[index.row()].planet()->player() != NULL)
                    {
                        name.append(QString("\n%1M").arg(mItems[index.row()].planet()->population(),0,'f',1));
                    }
                    else
                    {
                        name.append("\nuninhabited");
                    }
                }
                else if (mItems[index.row()].ship() != NULL)
                {
                    name.append(QString("\n%1/%2d %3/%4m").arg(mItems[index.row()].ship()->damage()).arg(mItems[index.row()].ship()->hitPoints()).arg(mItems[index.row()].ship()->movement()).arg(mItems[index.row()].ship()->config().maximumMovement()));
                }
                variant = name;
                break;
        }
        return variant;
    }
    else if (role == Qt::DecorationRole)
    {
        QVariant variant;
        switch (index.column())
        {
            case 0:
                if (mItems[index.row()].star() != NULL)
                {
                    UniversePainter painter;
                    QPixmap pic(24,24); 
                    QPainter p(&pic);   
                    p.fillRect(QRect(0,0,pic.width(),pic.height()), QBrush(Qt::black));
                    p.translate(12,12);
                    painter.paintStar(&p, mItems[index.row()].star(), pic.size());
                    p.end(); 
                    variant = pic;
                }
                else if (mItems[index.row()].planet() != NULL)
                {
                    UniversePainter painter;
                    QPixmap pic(24,24); 
                    QPainter p(&pic);   
                    p.fillRect(QRect(0,0,pic.width(),pic.height()), QBrush(Qt::black));
                    p.translate(12,12);
                    painter.paintPlanet(&p, mItems[index.row()].planet(), pic.size());
                    p.end(); 
                    variant = pic;
                }
                else if (mItems[index.row()].warp() != NULL)
                {
                    UniversePainter painter;
                    QPixmap pic(24,24); 
                    QPainter p(&pic);   
                    p.fillRect(QRect(0,0,pic.width(),pic.height()), QBrush(Qt::black));
                    p.translate(12,12);
                    painter.paintWarp(&p, mItems[index.row()].warp(), pic.size());
                    p.end(); 
                    variant = pic;
                }
                else if (mItems[index.row()].ship() != NULL)
                {
                    if (mItems[index.row()].ship()->isInTransit())
                    {
                        static std::map<unsigned int, QPixmapCache::Key> mPixmapCache;
                        QPixmap pixmap(24,24);
                        Game::Player * player = mItems[index.row()].ship()->player();
                        if (player != NULL)
                        {
                            if (mPixmapCache.find(Game::Universe::instance().game().playerIndex(player)) == mPixmapCache.end())
                            {
                                Gui::UniversePainter painter;
                                QPainter p(&pixmap);   
                                p.translate(24/2,24/2);
                                p.setOpacity(0.6);
                                painter.paintBadge(&p, player, pixmap.size());
                                p.setOpacity(1.0);
                                p.end(); 

                                mPixmapCache[Game::Universe::instance().game().playerIndex(player)] = QPixmapCache::insert(pixmap);
                            }
                            QPixmapCache::find(mPixmapCache[Game::Universe::instance().game().playerIndex(player)], &pixmap);
                        }
                        variant = pixmap;
                    }
                    else
                    {
                        static std::map<unsigned int, QPixmapCache::Key> mPixmapCache;
                        QPixmap pixmap(24,24);
                        Game::Player * player = mItems[index.row()].ship()->player();
                        if (player != NULL)
                        {
                            if (mPixmapCache.find(Game::Universe::instance().game().playerIndex(player)) == mPixmapCache.end())
                            {
                                Gui::UniversePainter painter;
                                QPainter p(&pixmap);   
                                p.translate(24/2,24/2);
                                painter.paintBadge(&p, player, pixmap.size());
                                p.end(); 

                                mPixmapCache[Game::Universe::instance().game().playerIndex(player)] = QPixmapCache::insert(pixmap);
                            }
                            QPixmapCache::find(mPixmapCache[Game::Universe::instance().game().playerIndex(player)], &pixmap);
                        }
                        variant = pixmap;
                    }
                }
                break;
        }
        return variant;
    }

    return QVariant();
}

bool SectorItemModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole)) 
    {
        QVariant variant;
        switch (index.column())
        {
            case 0:
                //throw "Unhandled Operation\n";
                //mItems[index.row()].name = value.toString().toStdString();
                break;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool SectorItemModel::insertRows(int position, int rows, const QModelIndex & index)
{
    //position+=1;
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row) 
    {
        mItems.insert(mItems.begin() + position, Item());
    }
    endInsertRows();
    return true;
}

bool SectorItemModel::removeRows(int position, int rows, const QModelIndex & index)
{
    //position+=1;
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row)
    {
        mItems.erase(mItems.begin() + position);
    }
    endRemoveRows();
    return true;
}

std::vector<Game::Planet *> SectorItemModel::planets(const QModelIndexList & indexList) const
{
    std::vector<Game::Planet *> result;
    for (QModelIndexList::const_iterator it = indexList.begin(); it!= indexList.end(); ++it)
    {
        if (int(mItems.size()) > (*it).row() && mItems[(*it).row()].planet() != NULL)
        {
            result.push_back(mItems[(*it).row()].planet());
        }
    }
    return result;
}

std::vector<Game::Ship *> SectorItemModel::ships(const QModelIndexList & indexList) const
{
    std::vector<Game::Ship *> result;
    for (QModelIndexList::const_iterator it = indexList.begin(); it!= indexList.end(); ++it)
    {
        if (int(mItems.size()) > (*it).row() && mItems[(*it).row()].ship() != NULL)
        {
            result.push_back(mItems[(*it).row()].ship());
        }
    }
    return result;
}
