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

#include "ShipConfigModel.h"

#include <QList>
#include <QTableView>

#include <algorithm>

using namespace Gui;

namespace
{
    const QList<QString> Headers = QList<QString>() << "Name" << "HP" << "Mvt" << "Star" << "Pop" << "Cost" << "Buy";

    QVariant data(const ShipConfigModel::Row & shipConfig, int column)
    {
        QVariant variant;
        switch (column)
        {
            case 0:
                variant = QString::fromStdString(shipConfig.config.name());
                break;
            case 1:
                variant = shipConfig.config.maximumHitPoints();
                break;
            case 2:
                variant = shipConfig.config.maximumMovement();
                break;
            case 3:
                variant = shipConfig.config.has(Game::Component::StarDrive) ? "X" : " ";
                break;
            case 4:
                variant = shipConfig.config.maximumPopulation();
                break;
            case 5:
                variant = shipConfig.config.cost();
                break;
            case 6:
                variant = shipConfig.count;
                break;
        }
        return variant;
    }

    void setData(ShipConfigModel::Row & shipConfig, int column, const QVariant & value)
    {
        QVariant variant;
        switch (column)
        {
            case 0:
                shipConfig.config.setName(value.toString().toStdString());
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                shipConfig.count = value.toUInt();
                break;
        }
    }
}

ShipConfigModel::~ShipConfigModel()
{

}

ShipConfigModel::ShipConfigModel(QObject * parent, std::vector<Row> * shipConfigs, bool buy)
    : QAbstractTableModel(parent)
    , mShipConfigs(shipConfigs)
    , mBuy(buy)
{
    setSupportedDragActions(Qt::CopyAction);
}

Qt::DropActions ShipConfigModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags ShipConfigModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
    {
        if (mBuy)
        {
            if (index.column() == 6)
            {
                return defaultFlags | Qt::ItemIsEditable;
            }
        }
        else
        {
            if (index.column() == 0)
            {
                return defaultFlags | Qt::ItemIsEditable;
            }
        }
    }
    return defaultFlags;
}


QVariant ShipConfigModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return Headers[section];
    }
    return QVariant();
}

int ShipConfigModel::columnCount(const QModelIndex & parent) const
{
    return Headers.size();
}

int ShipConfigModel::rowCount(const QModelIndex & parent) const
{
    return mShipConfigs != NULL ? mShipConfigs->size() : 0;
}

QVariant ShipConfigModel::data(const QModelIndex & index, int role) const
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
        return ::data((*mShipConfigs)[index.row()], index.column());
    }

    return QVariant();
}

bool ShipConfigModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole)) 
    {
        ::setData((*mShipConfigs)[index.row()], index.column(), value);
        emit dataChanged(index, index);
        //static_cast<QTableView *>(QObject::parent())->resizeColumnToContents(index.column());
        //static_cast<QTableView *>(QObject::parent())->resizeRowToContents(index.row());
        return true;
    }
    return false;
}

bool ShipConfigModel::insertRows(int position, int rows, const QModelIndex & index)
{
    //position+=1;
    if (mShipConfigs != NULL)
    {
        beginInsertRows(QModelIndex(), position, position+rows-1);
        for (int row = 0; row < rows; ++row) 
        {
            mShipConfigs->insert(mShipConfigs->begin() + position, Row()/*Game::ShipConfig()*/);
        }
        endInsertRows();
        return true;
    }
    else
    {
        return false;
    }
}

bool ShipConfigModel::removeRows(int position, int rows, const QModelIndex & index)
{
    //position+=1;
    if (mShipConfigs != NULL)
    {
        beginRemoveRows(QModelIndex(), position, position+rows-1);
        for (int row = 0; row < rows; ++row)
        {
            mShipConfigs->erase(mShipConfigs->begin() + position);
        }
        endRemoveRows();
        return true;
    }
    else
    {
        return false;
    }
}

QModelIndex ShipConfigModel::index(int row, int column, const QModelIndex & parent) const
{
    const Row * shipConfig = NULL;
    if (mShipConfigs != NULL && static_cast<int>(mShipConfigs->size()) > row)
    {
        shipConfig = &mShipConfigs->at(row);
    }
    return createIndex(row, column, const_cast<Row *>(shipConfig));
}