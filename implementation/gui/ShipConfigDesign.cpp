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

#include "ShipConfigDesign.h"
#include "ComponentSelection.h"
#include "SubscribablePushButton.h"
#include "MainWindow.h"
#include "QsKineticScroller.h"
#include "TableView.h"

#include <game/Universe.h>
#include <game/Player.h>
#include <game/ShipConfig.h>

#include <QBoxLayout>
#include <QTableView>
#include <QDialog>
#include <QDialogButtonBox>

using namespace Gui;

ShipConfigDesign::~ShipConfigDesign()
{

}

ShipConfigDesign::ShipConfigDesign(QWidget * parent)
    : QFrame(parent)
    , mEditView(NULL)
    , mShipConfigs()
{
    class PrivateSubscriberAddRow
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberAddRow()
        {
        }

        PrivateSubscriberAddRow(SubscribablePushButton * pushButton, QTableView * tableVew)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mTableView(tableVew)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (mTableView != NULL && mTableView->model() != NULL)
            {
                mTableView->model()->insertRow(0);
            }
        }

        QTableView * mTableView;
    };

    class PrivateSubscriberEditRow
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberEditRow()
        {
        }

        PrivateSubscriberEditRow(SubscribablePushButton * pushButton, QTableView * tableVew)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mTableView(tableVew)
        {
        }

    private:

        void clicked(bool checked)
        {
            Game::ShipConfig * shipConfig = NULL;
            if (mTableView != NULL && mTableView->currentIndex().internalPointer() != NULL)
            {
                shipConfig = static_cast<Game::ShipConfig *>(mTableView->currentIndex().internalPointer());
            }
            if (shipConfig != NULL)
            {
                QWidget * widget = MainWindow::instance().getFrame(MainWindow::ComponentsIndex);
                ComponentSelection * componentSelection = dynamic_cast<ComponentSelection *>(widget);
                if (componentSelection == NULL)
                {
                    componentSelection = widget->findChild<ComponentSelection *>();
                }
                if (componentSelection != NULL)
                {
                    componentSelection->setShipConfig(shipConfig);
                    componentSelection->loadComponents();
                }
                MainWindow::instance().showFrame(MainWindow::ComponentsIndex);
            }
        }

        QTableView * mTableView;
    };

    class PrivateSubscriberClose
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberClose()
        {
        }

        PrivateSubscriberClose(SubscribablePushButton * pushButton, ShipConfigDesign * shipConfigDesign)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mShipConfigDesign(shipConfigDesign)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (Game::Universe::instance().game().currentPlayer() != NULL)
            {
                const std::vector<ShipConfigModel::Row> & shipConfigs = mShipConfigDesign->shipConfigs();
                std::vector<Game::ShipConfig> configs;
                for (std::vector<ShipConfigModel::Row>::const_iterator it = shipConfigs.begin(); it != shipConfigs.end(); ++it)
                {
                    configs.push_back((*it).config);
                }
                Game::Universe::instance().game().currentPlayer()->setShipConfigs(configs);
            }
            MainWindow::instance().showFrame(MainWindow::MainFrameIndex);
        }

        ShipConfigDesign * mShipConfigDesign;
    };

    QBoxLayout * topLayout = new QVBoxLayout();
    setLayout(topLayout);
    topLayout->setContentsMargins(0,0,0,0);

    mEditView = new TableView(this);
    /*
    ShipConfigModel * shipConfigModel = new ShipConfigModel(mEditView, NULL);
    mEditView->setModel(shipConfigModel);
    */
    mEditView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mEditView->setSelectionMode(QAbstractItemView::SingleSelection);
    mEditView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mEditView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    mEditView->resizeColumnsToContents();
    topLayout->addWidget(mEditView);

    QsKineticScroller * kineticScroller = new QsKineticScroller(mEditView);
    kineticScroller->enableKineticScrollFor(mEditView);

    QBoxLayout * buttonLayout = new QHBoxLayout();
    topLayout->addItem(buttonLayout);

    SubscribablePushButton * addRowButton = new SubscribablePushButton(this, tr("Add"));
    addRowButton->setObjectName("addRowButton");
    new PrivateSubscriberAddRow(addRowButton, mEditView);   
    SubscribablePushButton * editRowButton = new SubscribablePushButton(this, tr("Edit.."));
    editRowButton->setObjectName("editRowButton");
    new PrivateSubscriberEditRow(editRowButton, mEditView);   
    SubscribablePushButton * closeButton = new SubscribablePushButton(this, tr("Close"));
    closeButton->setObjectName("closeButton");
    new PrivateSubscriberClose(closeButton, this);  
    buttonLayout->addStretch();   
    buttonLayout->addWidget(addRowButton);
    buttonLayout->addWidget(editRowButton);
    buttonLayout->addWidget(closeButton);
}

const std::vector<ShipConfigModel::Row> & ShipConfigDesign::shipConfigs() const
{
    return mShipConfigs;
}

void ShipConfigDesign::loadDesigns()
{
    mShipConfigs.clear();
    if (Game::Universe::instance().game().currentPlayer() != NULL)
    {
        const std::vector<Game::ShipConfig> & shipConfigs = Game::Universe::instance().game().currentPlayer()->shipConfigs();
        for (std::vector<Game::ShipConfig>::const_iterator it = shipConfigs.begin(); it != shipConfigs.end(); ++it)
        {
            ShipConfigModel::Row row;
            row.config = *it;
            row.count = 0;
            mShipConfigs.push_back(row);
        }
        delete mEditView->model();
        ShipConfigModel * shipConfigModel = new ShipConfigModel(mEditView, &mShipConfigs);
        mEditView->setModel(shipConfigModel);
        mEditView->resizeColumnsToContents();
        mEditView->resizeRowsToContents();
        mEditView->hideColumn(2);
    }
}