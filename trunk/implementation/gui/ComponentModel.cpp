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

#include "ComponentModel.h"

#include <game/Component.h>

#include <QList>
#include <QMimeData>
#include <QStringList>

#include <algorithm>

using namespace Gui;

namespace
{
    const QList<QString> Headers = QList<QString>() << "Name" << "Type" << "Lvl" << "HP" << "Cost";

    QVariant data(const Game::Component & component, int column)
    {
        QVariant variant;
        switch (column)
        {
            case 0:
                variant = QString::fromStdString(component.name());
                break;
            case 1:
                variant = QString::fromStdString(Game::Component::typeToString(component.type()));
                break;
            case 2:
                variant = component.level();
                break;
            case 3:
                variant = component.hitPoints();
                break;
            case 4:
                variant = component.cost();
                break;
        }
        return variant;
    }

    void setData(Game::Component & component, int column, const QVariant & value)
    {
        QVariant variant;
        switch (column)
        {
            case 0:
                component.setName(value.toString().toStdString());
                break;
            case 1:
                component.setType(Game::Component::stringToType(value.toString().toStdString()));
                break;
            case 2:
                component.setLevel(value.toUInt());
                break;
            case 3:
                component.setHitPoints(value.toUInt());
                break;
            case 4:
                component.setCost(value.toInt());
                break;
        }
    }
}

ComponentModel::~ComponentModel()
{

}

ComponentModel::ComponentModel(QObject * parent, std::vector<Game::Component> * components)
    : QAbstractTableModel(parent)
    , mComponents(components)
{
    setSupportedDragActions(Qt::CopyAction);
}

Qt::DropActions ComponentModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags ComponentModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
    {
        return Qt::ItemIsDragEnabled | /*Qt::ItemIsDropEnabled | */defaultFlags;
    }
    return Qt::ItemIsDropEnabled | defaultFlags;
}


QVariant ComponentModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return Headers[section];
    }
    return QVariant();
}

int ComponentModel::columnCount(const QModelIndex & parent) const
{
    return Headers.size();
}

int ComponentModel::rowCount(const QModelIndex & parent) const
{
    return mComponents != NULL ? mComponents->size() : 0;
}

QVariant ComponentModel::data(const QModelIndex & index, int role) const
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
        return ::data((*mComponents)[index.row()], index.column());
    }

    return QVariant();
}

bool ComponentModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole)) 
    {
        ::setData((*mComponents)[index.row()], index.column(), value);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool ComponentModel::insertRows(int position, int rows, const QModelIndex & index)
{
    if (mComponents != NULL)
    {
        beginInsertRows(QModelIndex(), position, position+rows-1);
        for (int row = 0; row < rows; ++row) 
        {
            mComponents->insert(mComponents->begin() + position, Game::Component());
        }
        endInsertRows();
        return true;
    }
    return false;
}

bool ComponentModel::removeRows(int position, int rows, const QModelIndex & index)
{
    if (mComponents != NULL)
    {
        beginRemoveRows(QModelIndex(), position, position+rows-1);
        for (int row = 0; row < rows; ++row)
        {
            mComponents->erase(mComponents->begin() + position);
        }
        endRemoveRows();
        return true;
    }
    return false;
}

QStringList ComponentModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData * ComponentModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData * mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex &index, indexes) 
    {
        if (index.isValid()) 
        {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << text;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

bool ComponentModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
    {
        return true;
    }

    if (!data->hasFormat("application/vnd.text.list"))
    {
        return false;
    }

    int beginRow = 0;

    if (row != -1)
    {
        beginRow = row;
    }
    else if (parent.isValid())
    {
        beginRow = parent.row();
    }
    else
    {
        beginRow = rowCount(QModelIndex());
    }

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 1;

    while (!stream.atEnd()) 
    {
        QString text;
        stream >> text;
        newItems << text;
    }

    insertRows(beginRow, rows, QModelIndex());
    int columnAt = 0;
    foreach (const QString &text, newItems) 
    {
        QModelIndex idx = index(beginRow, columnAt, QModelIndex());
        setData(idx, text);
        columnAt++;
    }

    return true;
}