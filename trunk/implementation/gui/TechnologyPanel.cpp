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

#include "TechnologyPanel.h"
#include "ComponentModel.h"
#include "SubscribablePushButton.h"
#include "MainWindow.h"
#include "TableView.h"
#include "QsKineticScroller.h"

#include <game/Universe.h>
#include <game/Player.h>
#include <game/Technology.h>
#include <game/Component.h>

#include <QHBoxLayout>
#include <QTableView>
#include <QResizeEvent>

#include <algorithm>

using namespace Gui;

TechnologyPanel::~TechnologyPanel()
{

}

TechnologyPanel::TechnologyPanel(QWidget * parent)
    : QFrame(parent)
    , mSelectedComponents()
    , mAvailableComponents()
    , mEditView(NULL)
    , mListView(NULL)
{
    class PrivateSubscriberAdd
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberAdd()
        {
        }

        PrivateSubscriberAdd(SubscribablePushButton * pushButton, TechnologyPanel * technologyPanel)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mTechnologyPanel(technologyPanel)
        {
        }

    private:

        void clicked(bool checked)
        {
            mTechnologyPanel->selectedComponentsView()->model()->dropMimeData(mTechnologyPanel->allComponentsView()->model()->mimeData(
                mTechnologyPanel->allComponentsView()->selectionModel()->selectedIndexes()), Qt::CopyAction, -1, -1, QModelIndex());

            QModelIndexList indexList = mTechnologyPanel->allComponentsView()->selectionModel()->selectedRows();
            std::set<int> rowSet;
            for (QModelIndexList::const_iterator it = indexList.begin(); it != indexList.end(); ++it)
            {
                rowSet.insert((*it).row());
            }
            for (std::set<int>::const_reverse_iterator it = rowSet.rbegin(); it != rowSet.rend(); ++it)
            {
                mTechnologyPanel->allComponentsView()->model()->removeRow(*it);
            }

            if (Game::Universe::instance().game().currentPlayer() != NULL)
            {
                Game::Universe::instance().game().currentPlayer()->setComponents(mTechnologyPanel->selectedComponents());
            }
        }

        TechnologyPanel * mTechnologyPanel;
    };

    class PrivateSubscriberClose
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberClose()
        {
        }

        PrivateSubscriberClose(SubscribablePushButton * pushButton, TechnologyPanel * technologyPanel)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mTechnologyPanel(technologyPanel)
        {
        }

    private:

        void clicked(bool checked)
        {
            MainWindow::instance().showFrame(MainWindow::MainFrameIndex);
        }

        TechnologyPanel * mTechnologyPanel;
    };

    QBoxLayout * topLayout = new QVBoxLayout();
    setLayout(topLayout);
    topLayout->setContentsMargins(0,0,0,0);

    QBoxLayout * tableLayout = new QHBoxLayout();
    topLayout->addItem(tableLayout);

    mEditView = new TableView(this);
    mEditView->setModel(new ComponentModel(mEditView, NULL));
    mEditView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mEditView->setSelectionMode(QAbstractItemView::NoSelection);
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

const std::vector<Game::Component> & TechnologyPanel::selectedComponents() const
{
    return mSelectedComponents;
}

QTableView * TechnologyPanel::selectedComponentsView()
{
    return mEditView;
}

QTableView * TechnologyPanel::allComponentsView()
{
    return mListView;
}

void TechnologyPanel::loadComponents()
{
    if (Game::Universe::instance().game().currentPlayer() != NULL)
    {
        mSelectedComponents = Game::Universe::instance().game().currentPlayer()->components();
        delete mEditView->model();
        mEditView->setModel(new ComponentModel(mEditView, &mSelectedComponents));
        mEditView->resizeColumnsToContents();
        mEditView->resizeRowsToContents();
    }

    std::vector<Game::Component> components;
    for (unsigned int i = 0; i < Game::Technology::instance().engineModules().size(); ++i)
    {
        if (std::find(mSelectedComponents.begin(), mSelectedComponents.end(), Game::Technology::instance().engineModules()[i].component()) == mSelectedComponents.end())
        {
            components.push_back(Game::Technology::instance().engineModules()[i].component());
        }
    }
    for (unsigned int i = 0; i < Game::Technology::instance().weaponModules().size(); ++i)
    {
        if (std::find(mSelectedComponents.begin(), mSelectedComponents.end(), Game::Technology::instance().weaponModules()[i].component()) == mSelectedComponents.end())
        {
            components.push_back(Game::Technology::instance().weaponModules()[i].component());
        }
    }
    for (unsigned int i = 0; i < Game::Technology::instance().starDriveModules().size(); ++i)
    {
        if (std::find(mSelectedComponents.begin(), mSelectedComponents.end(), Game::Technology::instance().starDriveModules()[i].component()) == mSelectedComponents.end())
        {
            components.push_back(Game::Technology::instance().starDriveModules()[i].component());
        }
    }
    for (unsigned int i = 0; i < Game::Technology::instance().colonyModules().size(); ++i)
    {
        if (std::find(mSelectedComponents.begin(), mSelectedComponents.end(), Game::Technology::instance().colonyModules()[i].component()) == mSelectedComponents.end())
        {
            components.push_back(Game::Technology::instance().colonyModules()[i].component());
        }
    }
    mAvailableComponents = components;

    delete mListView->model();
    mListView->setModel(new ComponentModel(mListView, &mAvailableComponents));
    mListView->resizeColumnsToContents();
    mListView->resizeRowsToContents();
}