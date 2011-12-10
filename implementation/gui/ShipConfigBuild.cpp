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
#include <QSpinBox>
#include <QItemDelegate>

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
                for (std::vector<ShipConfigModel::Row>::iterator it = mShipConfigBuild->shipConfigs().begin(); it!= mShipConfigBuild->shipConfigs().end(); ++it)
                {
                    while ((*it).count > 0 && (*it).config.cost() <= Game::Universe::instance().game().currentPlayer()->money())
                    {
                        --(*it).count;
                        Game::Universe::instance().game().currentPlayer()->setMoney(Game::Universe::instance().game().currentPlayer()->money() - (*it).config.cost());
                        Game::Ship * ship = new Game::Ship(mShipConfigBuild->sector(), (*it).config);
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
    //ShipConfigModel * shipConfigModel = new ShipConfigModel(mEditView, NULL);
    //mEditView->setModel(shipConfigModel);
    mEditView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mEditView->setSelectionMode(QAbstractItemView::SingleSelection);
    mEditView->setEditTriggers(QAbstractItemView::AllEditTriggers);
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

    class SpinBoxDelegate : public QItemDelegate
    {
    public:
        SpinBoxDelegate(QObject *parent)
     : QItemDelegate(parent)
 {
 }

        QWidget *createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     QSpinBox *editor = new QSpinBox(parent);
     editor->setMinimum(0);
     editor->setMaximum(100);

     return editor;
 }

        void setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     int value = index.model()->data(index, Qt::DisplayRole).toInt();

     QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
     spinBox->setValue(value);
 }
        void setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
     spinBox->interpretText();
     int value = spinBox->value();

     model->setData(index, value, Qt::EditRole);
 }

        void updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }
    };

SpinBoxDelegate * delegate = new SpinBoxDelegate(mEditView);
     mEditView->setItemDelegateForColumn(2, delegate);
}

Game::Sector * ShipConfigBuild::sector() const
{
    return mSector;
}

void ShipConfigBuild::setSector(Game::Sector * sector)
{
    mSector = sector;
}

const std::vector<ShipConfigModel::Row> & ShipConfigBuild::shipConfigs() const
{
    return mShipConfigs;
}

std::vector<ShipConfigModel::Row> & ShipConfigBuild::shipConfigs()
{
    return mShipConfigs;
}

void ShipConfigBuild::loadDesigns()
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
        ShipConfigModel * shipConfigModel = new ShipConfigModel(mEditView, &mShipConfigs, true);
        mEditView->setModel(shipConfigModel);
    }
}