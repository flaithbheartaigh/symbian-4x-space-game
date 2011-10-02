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

#include "NewGamePanel.h"
#include "SubscribablePushButton.h"
#include "MainWindow.h"
#include "UniverseViewer.h"

#include <Game/Universe.h>
#include <Game/Component.h>
#include <Game/Player.h>
#include <Game/AI.h>
#include <Game/Parameters.h>

#include <data/NamesData.h>
#include <data/AssetSerializer.h>

#include <QBoxLayout>
#include <QSpinBox>
#include <QFormLayout>
#include <QComboBox>
#include <QSpacerItem>

#include <algorithm>

using namespace Gui;

NewGamePanel::~NewGamePanel()
{

}

NewGamePanel::NewGamePanel(QWidget * parent)
    : QFrame(parent)
    , mPlayerNames(new QVBoxLayout())
{
    class PrivateSubscriberOK
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberOK()
        {
        }

        PrivateSubscriberOK(SubscribablePushButton * pushButton, QBoxLayout * playerNames, QSpinBox * npcCount)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mPlayerNames(playerNames)
            , mNPCCount(npcCount)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (MainWindow::instance().getViewer() != NULL)
            {
                MainWindow::instance().getViewer()->noForce(true);
            }

            std::vector<std::string> starNames;
            Data::NamesData("stars.json", &starNames, Data::NamesData::Load);
            std::vector<std::string> playerNames;
            Data::NamesData("empires.json", &playerNames, Data::NamesData::Load);
            std::vector<Game::Component> components;
            Data::AssetSerializer::load(Game::Parameters::instance().getDataFilePath("configs/components.json"), components);

            //if (mPlayerNames->count() > 0 || mNPCCount->value() > 0)
            {
                Game::Universe::instance().clear();
                Game::Universe::instance().names().setAvailableStarNames(starNames);

                for (int i = 0; i < mPlayerNames->count(); ++i)
                {
                    QComboBox * box = dynamic_cast<QComboBox *>(mPlayerNames->itemAt(i)->widget());
                    playerNames.erase(std::remove(playerNames.begin(), playerNames.end(), box->currentText().toStdString()), playerNames.end()); 

                    Game::Player * human = new Game::Player();
                    human->setName(box->currentText().toStdString());
                    human->setComponents(components);
                    human->setAI(new Game::AI(human));
                    Game::Universe::instance().game().addPlayer(human);
                }

                Game::Universe::instance().names().setAvailablePlayerNames(playerNames);

                for (int i = 0; i < mNPCCount->value(); i++)
                {
                    Game::Player * comp = new Game::Player();
                    comp->setName(Game::Universe::instance().names().nextAvailablePlayerName());
                    comp->setComponents(components);
                    comp->setAI(new Game::NPC(comp));
                    Game::Universe::instance().game().addPlayer(comp);
                }

                Game::Universe::instance().generate();
                Game::Universe::instance().game().setCurrentPlayerIndex(0);
            }
            
            if (MainWindow::instance().getViewer() != NULL)
            {
                MainWindow::instance().getViewer()->noForce(false);
            }
            MainWindow::instance().showFrame(MainWindow::MainFrameIndex);
        }

        QBoxLayout * mPlayerNames;

        QSpinBox * mNPCCount;
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

    QSpinBox * humanCount = new QSpinBox();
    humanCount->setMinimum(0);
    humanCount->setMaximum(10);
    connect(humanCount, SIGNAL(editingFinished()), SLOT(slot_editingFinished()));
    QSpinBox * npcCount = new QSpinBox();
    npcCount->setMinimum(0);
    npcCount->setMaximum(10);

    QHBoxLayout * middleLayout = new QHBoxLayout();
    topLayout->addLayout(middleLayout);
    topLayout->addStretch();

    QVBoxLayout * leftBoxLayout = new QVBoxLayout();
    middleLayout->addLayout(leftBoxLayout);

    QFormLayout * leftFormLayout = new QFormLayout();
    leftFormLayout->addRow(tr("&Human players:"), humanCount);
    leftBoxLayout->addLayout(leftFormLayout);
    leftBoxLayout->addLayout(mPlayerNames);
    leftBoxLayout->addStretch();

    QVBoxLayout * rightBoxLayout = new QVBoxLayout();
    middleLayout->addLayout(rightBoxLayout);

    QFormLayout * rightFormLayout = new QFormLayout();
    rightFormLayout->addRow(tr("&NPC players:"), npcCount);
    rightBoxLayout->addLayout(rightFormLayout);
    rightBoxLayout->addStretch();

    QBoxLayout * buttonLayout = new QHBoxLayout();
    topLayout->addItem(buttonLayout);
    SubscribablePushButton * okButton = new SubscribablePushButton(this, tr("OK"));
    okButton->setObjectName("okButton");
    new PrivateSubscriberOK(okButton, mPlayerNames, npcCount);
    SubscribablePushButton * cancelButton = new SubscribablePushButton(this, tr("Cancel"));
    cancelButton->setObjectName("cancelButton");
    new PrivateSubscriberCancel(cancelButton);
    buttonLayout->addStretch();   
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);   
}

void NewGamePanel::slot_editingFinished()
{
    std::vector<std::string> playerNames;
    Data::NamesData("empires.json", &playerNames, Data::NamesData::Load);
    QStringList items;
    for (std::vector<std::string>::iterator it = playerNames.begin(); it != playerNames.end(); ++it)
    {
        items << QString::fromStdString(*it);
    }

    QSpinBox * s = dynamic_cast<QSpinBox *>(sender());
    for (int i = mPlayerNames->count(); i > s->value(); --i)
    {
        delete mPlayerNames->takeAt(i-1)->widget();
    }
    for (int i = mPlayerNames->count(); i < s->value(); ++i)
    {
        QComboBox * box = new QComboBox();
        box->addItems(items);
        box->setCurrentIndex(i);
        mPlayerNames->addWidget(box);
    }
}