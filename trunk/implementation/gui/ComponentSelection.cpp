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
    mEditView->setSelectionMode(QAbstractItemView::SingleSelection);
    mEditView->viewport()->setAcceptDrops(true);
    mEditView->setDropIndicatorShown(true);
    mEditView->setDragDropMode(QAbstractItemView::DropOnly);
    mEditView->hideColumn(1);
    mEditView->resizeColumnsToContents();
    mEditView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mEditView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableLayout->addWidget(mEditView);

    //(new QsKineticScroller(mEditView))->enableKineticScrollFor(mEditView);

    mListView = new TableView(this);
    mListView->setModel(new ComponentModel(mListView, NULL));
    mListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mListView->setSelectionMode(QAbstractItemView::SingleSelection);
    mListView->setDragEnabled(true);
    mListView->setDropIndicatorShown(true);
    mListView->setDragDropMode(QAbstractItemView::DragOnly);
    mListView->resizeColumnsToContents();
    mListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mListView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableLayout->addWidget(mListView);

    //(new QsKineticScroller(mListView))->enableKineticScrollFor(mListView);

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
        delete mEditView->model();//->setModel(new ComponentModel(edit, &mSelectedComponents));
        mEditView->setModel(new ComponentModel(mEditView, &mSelectedComponents));
        mEditView->resizeColumnsToContents();
        mEditView->resizeRowsToContents();
    }

    if (Game::Universe::instance().game().currentPlayer() != NULL)
    {
        mAvailableComponents = Game::Universe::instance().game().currentPlayer()->components();
        delete mListView->model();//->setModel(new ComponentModel(edit, &mSelectedComponents));
        mListView->setModel(new ComponentModel(mListView, &mAvailableComponents));
        mListView->resizeColumnsToContents();
        mListView->resizeRowsToContents();
    }
}
/*
void ComponentSelection::resizeEvent(QResizeEvent * event)
{
    bool switchSize = (event->oldSize().width() >= event->oldSize().height()) && (event->size().width() < event->size().height());
    switchSize = switchSize || ((event->oldSize().width() < event->oldSize().height()) && (event->size().width() >= event->size().height()));

    if (switchSize)
    {
        for (std::set<QBoxLayout *>::const_iterator it = mLayouts.begin(); it != mLayouts.end(); ++it)
        {
            if ((*it)->direction() == QBoxLayout::LeftToRight)
            {
                (*it)->setDirection(QBoxLayout::TopToBottom);
            }
            else if ((*it)->direction() == QBoxLayout::TopToBottom)
            {
                (*it)->setDirection(QBoxLayout::LeftToRight);
            }
        }
    }
}*/
