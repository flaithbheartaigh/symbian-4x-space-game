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
#include <Game/Technology.h>

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
    , mPlayerNamesStr()
{
    class PrivateSubscriberOK
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberOK()
        {
        }

        PrivateSubscriberOK(SubscribablePushButton * pushButton, QBoxLayout * playerNames, QSpinBox * npcCount, QSpinBox * universeSize, QSpinBox * universeDensity)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mPlayerNames(playerNames)
            , mNPCCount(npcCount)
            , mUniverseSize(universeSize)
            , mUniverseDensity(universeDensity)
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
            for (unsigned int i = 0; i < 1/*Game::Technology::instance().engineModules().size()*/; ++i)
            {
                components.push_back(Game::Technology::instance().engineModules()[i].component());
            }
            for (unsigned int i = 0; i < 1/*Game::Technology::instance().weaponModules().size()*/; ++i)
            {
                components.push_back(Game::Technology::instance().weaponModules()[i].component());
            }
            for (unsigned int i = 0; i < 1/*Game::Technology::instance().starDriveModules().size()*/; ++i)
            {
                components.push_back(Game::Technology::instance().starDriveModules()[i].component());
            }
            for (unsigned int i = 0; i < 1/*Game::Technology::instance().colonyModules().size()*/; ++i)
            {
                components.push_back(Game::Technology::instance().colonyModules()[i].component());
            }



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

                Game::Universe::instance().generate(mUniverseDensity->value(), mUniverseSize->value());
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

        QSpinBox * mUniverseSize;

        QSpinBox * mUniverseDensity;
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

    QSpinBox * universeSize = new QSpinBox();
    universeSize->setMinimum(2);
    universeSize->setMaximum(20);
    universeSize->setValue(4);

    QSpinBox * universeDensity = new QSpinBox();
    universeDensity->setMinimum(1);
    universeDensity->setMaximum(20);
    universeDensity->setValue(1);

    QHBoxLayout * firstLayout = new QHBoxLayout();
    topLayout->addLayout(firstLayout);

    QVBoxLayout * leftBoxFirstLayout = new QVBoxLayout();
    firstLayout->addLayout(leftBoxFirstLayout);

    QFormLayout * leftFormFirstLayout = new QFormLayout();
    leftFormFirstLayout->addRow(tr("&Universe size:"), universeSize);
    leftBoxFirstLayout->addLayout(leftFormFirstLayout);

    QVBoxLayout * rightBoxFirstLayout = new QVBoxLayout();
    firstLayout->addLayout(rightBoxFirstLayout);

    QFormLayout * rightFormFirstLayout = new QFormLayout();
    rightFormFirstLayout->addRow(tr("&Universe density:"), universeDensity);
    rightBoxFirstLayout->addLayout(rightFormFirstLayout);





    QSpinBox * humanCount = new QSpinBox();
    humanCount->setMinimum(0);
    humanCount->setMaximum(10);
    connect(humanCount, SIGNAL(valueChanged(int)), SLOT(slot_valueChanged(int)));
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
    new PrivateSubscriberOK(okButton, mPlayerNames, npcCount, universeSize, universeDensity);
    SubscribablePushButton * cancelButton = new SubscribablePushButton(this, tr("Cancel"));
    cancelButton->setObjectName("cancelButton");
    new PrivateSubscriberCancel(cancelButton);
    buttonLayout->addStretch();   
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);   

    Data::NamesData("empires.json", &mPlayerNamesStr, Data::NamesData::Load);

    humanCount->setValue(1);
}

void NewGamePanel::slot_valueChanged(int)
{
    QStringList items;
    for (std::vector<std::string>::iterator it = mPlayerNamesStr.begin(); it != mPlayerNamesStr.end(); ++it)
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