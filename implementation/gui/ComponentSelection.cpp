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

#include "ComponentSelection.h"
#include "ComponentModel.h"
#include "SubscribablePushButton.h"
#include "MainWindow.h"
#include "TableView.h"
#include "QsKineticScroller.h"

#include <game/Universe.h>
#include <game/Player.h>
#include <game/Component.h>
#include <game/ShipConfig.h>

#include <QHBoxLayout>
#include <QTableView>
#include <QResizeEvent>

using namespace Gui;

ComponentSelection::~ComponentSelection()
{

}

ComponentSelection::ComponentSelection(QWidget * parent)
    : QFrame(parent)
    , mSelectedComponents()
    , mAvailableComponents()
    , mEditView(NULL)
    , mListView(NULL)
    , mShipConfig(NULL)
{
    class PrivateSubscriberAdd
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberAdd()
        {
        }

        PrivateSubscriberAdd(SubscribablePushButton * pushButton, ComponentSelection * componentSelection)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mComponentSelection(componentSelection)
        {
        }

    private:

        void clicked(bool checked)
        {
            mComponentSelection->selectedComponentsView()->model()->dropMimeData(mComponentSelection->allComponentsView()->model()->mimeData(
                mComponentSelection->allComponentsView()->selectionModel()->selectedIndexes()), Qt::CopyAction, -1, -1, QModelIndex());
        }

        ComponentSelection * mComponentSelection;
    };

    class PrivateSubscriberRemove
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberRemove()
        {
        }

        PrivateSubscriberRemove(SubscribablePushButton * pushButton, ComponentSelection * componentSelection)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mComponentSelection(componentSelection)
        {
        }

    private:

        void clicked(bool checked)
        {
            QModelIndexList indexList = mComponentSelection->selectedComponentsView()->selectionModel()->selectedRows();
            std::set<int> rowSet;
            for (QModelIndexList::const_iterator it = indexList.begin(); it != indexList.end(); ++it)
            {
                rowSet.insert((*it).row());
            }
            for (std::set<int>::const_reverse_iterator it = rowSet.rbegin(); it != rowSet.rend(); ++it)
            {
                mComponentSelection->selectedComponentsView()->model()->removeRow(*it);
            }
        }

        ComponentSelection * mComponentSelection;
    };

    class PrivateSubscriberClose
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberClose()
        {
        }

        PrivateSubscriberClose(SubscribablePushButton * pushButton, ComponentSelection * componentSelection)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mComponentSelection(componentSelection)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (mComponentSelection->shipConfig() != NULL)
            {
                mComponentSelection->shipConfig()->setComponents(mComponentSelection->selectedComponents());
            }
            MainWindow::instance().showFrame(MainWindow::ShipDesignIndex);
        }

        ComponentSelection * mComponentSelection;
    };

    QBoxLayout * topLayout = new QVBoxLayout();
    setLayout(topLayout);
    topLayout->setContentsMargins(0,0,0,0);

    QBoxLayout * tableLayout = new QHBoxLayout();
    topLayout->addItem(tableLayout);

    mEditView = new TableView(this);
    mEditView->setModel(new ComponentModel(mEditView, NULL));
    mEditView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mEditView->setSelectionMode(QAbstractItemView::MultiSelection);
    mEditView->viewport()->setAcceptDrops(true);
    mEditView->setDropIndicatorShown(true);
    mEditView->setDragDropMode(QAbstractItemView::NoDragDrop);
    mEditView->hideColumn(1);
    mEditView->resizeColumnsToContents();
    mEditView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mEditView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableLayout->addWidget(mEditView);

    (new QsKineticScroller(mEditView))->enableKineticScrollFor(mEditView);

    QVBoxLayout * copyButtonsLayout = new QVBoxLayout();
    copyButtonsLayout->addStretch();
    SubscribablePushButton * addButton = new SubscribablePushButton(this, tr("<<"));
    addButton->setObjectName("addButton");
    new PrivateSubscriberAdd(addButton, this);  
    copyButtonsLayout->addWidget(addButton);
    SubscribablePushButton * removeButton = new SubscribablePushButton(this, tr(">>"));
    removeButton->setObjectName("removeButton");
    new PrivateSubscriberRemove(removeButton, this);  
    copyButtonsLayout->addWidget(removeButton);
    copyButtonsLayout->addStretch();
    tableLayout->addLayout(copyButtonsLayout);

    mListView = new TableView(this);
    mListView->setModel(new ComponentModel(mListView, NULL));
    mListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mListView->setSelectionMode(QAbstractItemView::MultiSelection);
    mListView->setDragEnabled(true);
    mListView->setDropIndicatorShown(true);
    mListView->setDragDropMode(QAbstractItemView::NoDragDrop);
    mListView->resizeColumnsToContents();
    mListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mListView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableLayout->addWidget(mListView);

    (new QsKineticScroller(mListView))->enableKineticScrollFor(mListView);

    QBoxLayout * buttonLayout = new QHBoxLayout();
    topLayout->addItem(buttonLayout);

    SubscribablePushButton * closeButton = new SubscribablePushButton(this, tr("Close"));
    closeButton->setObjectName("closeButton");
    new PrivateSubscriberClose(closeButton, this);  
    buttonLayout->addStretch();   
    buttonLayout->addWidget(closeButton);
}

const std::vector<Game::Component> & ComponentSelection::selectedComponents() const
{
    return mSelectedComponents;
}

QTableView * ComponentSelection::selectedComponentsView()
{
    return mEditView;
}

QTableView * ComponentSelection::allComponentsView()
{
    return mListView;
}

Game::ShipConfig * ComponentSelection::shipConfig() const
{
    return mShipConfig;
}

void ComponentSelection::setShipConfig(Game::ShipConfig * shipConfig)
{
    mShipConfig = shipConfig;
}

void ComponentSelection::loadComponents()
{
    if (mShipConfig != NULL)
    {
        mSelectedComponents = mShipConfig->components();
        delete mEditView->model();
        mEditView->setModel(new ComponentModel(mEditView, &mSelectedComponents));
        mEditView->resizeColumnsToContents();
        mEditView->resizeRowsToContents();
    }

    if (Game::Universe::instance().game().currentPlayer() != NULL)
    {
        mAvailableComponents = Game::Universe::instance().game().currentPlayer()->components();
        delete mListView->model();
        mListView->setModel(new ComponentModel(mListView, &mAvailableComponents));
        mListView->resizeColumnsToContents();
        mListView->resizeRowsToContents();
    }
}