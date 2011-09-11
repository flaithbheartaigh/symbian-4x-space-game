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

#include "TableView.h"

using namespace Gui;

TableView::~TableView()
{

}

TableView::TableView(QWidget * parent)
    : QTableView(parent)
    , mOtherTableView(NULL)
{

}

void TableView::setModel(QAbstractItemModel * newModel)
{
    if (newModel == model())
        return;
    //let's disconnect from the old model
    if (model() != NULL) {
        disconnect(model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(slot_dataChanged(QModelIndex,QModelIndex)));
    }
    if (newModel != NULL) { //and connect to the new one
        connect(newModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(slot_dataChanged(QModelIndex,QModelIndex)));
    }
    QTableView::setModel(newModel);
}

void TableView::setOtherTableView(TableView * tableView)
{
    mOtherTableView = tableView;
}

void TableView::mousePressEvent(QMouseEvent * event)
{
    QTableView::mousePressEvent(event);
}

void TableView::mouseMoveEvent(QMouseEvent * event)
{
    QTableView::mouseMoveEvent(event);
}

void TableView::mouseReleaseEvent(QMouseEvent * event)
{
    QTableView::mouseReleaseEvent(event);
}

void TableView::slot_dataChanged(const QModelIndex & from, const QModelIndex & to)
{
    resizeColumnToContents(from.column());
    resizeRowToContents(from.row());
}