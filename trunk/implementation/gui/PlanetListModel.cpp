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

#include "PlanetListModel.h"
#include "UniversePainter.h"

#include <game/Sector.h>
#include <game/Star.h>
#include <game/Planet.h>
#include <game/Ship.h>
#include <game/UniverseVisitor.h>

#include <QList>
#include <QPainter>
#include <QPixmap>
#include <QAbstractItemView>

#include <algorithm>

using namespace Gui;

namespace
{
    const QList<QString> Headers = QList<QString>() << "Name";
}

PlanetListModel::~PlanetListModel()
{

}

PlanetListModel::PlanetListModel(QObject * parent)
    : QAbstractTableModel(parent)
    , mPlanets()
{

}

void PlanetListModel::setPlanets(const std::vector<Game::Planet *> & planets)
{
    mPlanets = planets;
    layoutChanged();
}

QVariant PlanetListModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return Headers[section];
    }
    return QVariant();
}

int PlanetListModel::columnCount(const QModelIndex & parent) const
{
    return Headers.size();
}

int PlanetListModel::rowCount(const QModelIndex & parent) const
{
    return mPlanets.size();
}

QVariant PlanetListModel::data(const QModelIndex & index, int role) const
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
                QString name = QString::fromStdString(mPlanets[index.row()]->name());
				if (mPlanets[index.row()]->player() != NULL)
				{
					name.append(QString("\n%1M").arg(mPlanets[index.row()]->population(),0,'f',1));
				}
				else
				{
					name.append("\nuninhabited");
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
				UniversePainter painter;
				QPixmap pic(24,24); 
				QPainter p(&pic);   
				p.fillRect(QRect(0,0,pic.width(),pic.height()), QBrush(Qt::black));
				p.translate(12,12);
				painter.paintPlanet(&p, mPlanets[index.row()], pic.size());
				p.end(); 
				variant = pic;
                break;
        }
        return variant;
    }

    return QVariant();
}