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

#include "ShipConfigBuild.h"
#include "ShipConfigModel.h"
#include "ComponentSelection.h"
#include "SubscribablePushButton.h"
#include "MainWindow.h"
#include "QsKineticScroller.h"
#include "TableView.h"

#include <game/Universe.h>
#include <game/Player.h>
#include <game/Sector.h>
#include <game/Ship.h>
#include <game/ShipConfig.h>

#include <QBoxLayout>
#include <QTableView>
#include <QDialog>
#include <QDialogButtonBox>

using namespace Gui;

ShipConfigBuild::~ShipConfigBuild()
{

}

ShipConfigBuild::ShipConfigBuild(QWidget * parent)
    : QFrame(parent)
    , mEditView(NULL)
    , mShipConfigs()
    , mSelectionModel(NULL)
    , mSector(NULL)
{
    class PrivateSubscriberOK
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberOK()
        {
        }

        PrivateSubscriberOK(SubscribablePushButton * pushButton, ShipConfigBuild * shipConfigBuild)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mShipConfigBuild(shipConfigBuild)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (Game::Universe::instance().game().currentPlayer() != NULL && mShipConfigBuild->sector() != NULL)
            {
                const QModelIndexList & indexList = mShipConfigBuild->selectionModel()->selectedRows();
                for (QModelIndexList::const_iterator it = indexList.begin(); it!= indexList.end(); ++it)
                {
                    Game::ShipConfig * shipConfig = static_cast<Game::ShipConfig *>((*it).internalPointer());
                    if (shipConfig != NULL && shipConfig->cost() <= Game::Universe::instance().game().currentPlayer()->money())
                    {
                        Game::Universe::instance().game().currentPlayer()->setMoney(Game::Universe::instance().game().currentPlayer()->money() - shipConfig->cost());
                        Game::Ship * ship = new Game::Ship(mShipConfigBuild->sector(), *shipConfig);
                        ship->setPlayer(Game::Universe::instance().game().currentPlayer());
                        mShipConfigBuild->sector()->addShip(ship);
                    }
                }
            }
            MainWindow::instance().showFrame(MainWindow::MainFrameIndex);
        }

        ShipConfigBuild * mShipConfigBuild;
    };

    class PrivateSubscriberCancel
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberCancel()
        {
        }

        PrivateSubscriberCancel(SubscribablePushButton * pushButton)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
        {
        }

    private:

        void clicked(bool checked)
        {
            MainWindow::instance().showFrame(MainWindow::MainFrameIndex);
        }
    };

    QBoxLayout * topLayout = new QVBoxLayout();
    setLayout(topLayout);
    topLayout->setContentsMargins(0,0,0,0);

    mEditView = new TableView(this);
    ShipConfigModel * shipConfigModel = new ShipConfigModel(mEditView, NULL);
    mEditView->setModel(shipConfigModel);
    mEditView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mEditView->setSelectionMode(QAbstractItemView::SingleSelection);
    mEditView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mEditView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mEditView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    topLayout->addWidget(mEditView);

    QsKineticScroller * kineticScroller = new QsKineticScroller(mEditView);
    kineticScroller->enableKineticScrollFor(mEditView);

    QBoxLayout * buttonLayout = new QHBoxLayout();
    topLayout->addItem(buttonLayout);
    SubscribablePushButton * okButton = new SubscribablePushButton(this, tr("OK"));
    okButton->setObjectName("okButton");
    new PrivateSubscriberOK(okButton, this);
    SubscribablePushButton * cancelButton = new SubscribablePushButton(this, tr("Cancel"));
    cancelButton->setObjectName("cancelButton");
    new PrivateSubscriberCancel(cancelButton);
    buttonLayout->addStretch();   
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
}

Game::Sector * ShipConfigBuild::sector() const
{
    return mSector;
}

void ShipConfigBuild::setSector(Game::Sector * sector)
{
    mSector = sector;
}

const std::vector<Game::ShipConfig> & ShipConfigBuild::shipConfigs() const
{
    return mShipConfigs;
}

void ShipConfigBuild::loadDesigns()
{
    if (Game::Universe::instance().game().currentPlayer() != NULL)
    {
        mShipConfigs = Game::Universe::instance().game().currentPlayer()->shipConfigs();
        delete mEditView->model();
        ShipConfigModel * shipConfigModel = new ShipConfigModel(mEditView, &mShipConfigs);
        mEditView->setModel(shipConfigModel);
    }
}

QItemSelectionModel * ShipConfigBuild::selectionModel() const
{
    return mEditView->selectionModel();
}
