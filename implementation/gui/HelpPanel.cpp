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

#include "HelpPanel.h"
#include "SubscribablePushButton.h"
#include "MainWindow.h"
#include "QsKineticScroller.h"

#include <QFormLayout>
#include <QScrollArea>
#include <QLabel>

using namespace Gui;

HelpPanel::~HelpPanel()
{

}

HelpPanel::HelpPanel(QWidget * parent)
    : QFrame(parent)
{
    class PrivateSubscriberClose
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberClose()
        {
        }

        PrivateSubscriberClose(SubscribablePushButton * pushButton)
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

    QFormLayout * formLayout = new QFormLayout();

    QFrame * buttonsWidget = new QFrame(NULL);
    buttonsWidget->setLayout(formLayout);
    buttonsWidget->setObjectName("helpButtonsWidget");
    buttonsWidget->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    SubscribablePushButton * moveShipButton = new SubscribablePushButton(NULL, tr("M"));
    moveShipButton->setObjectName("move");
    SubscribablePushButton * colonizeButton = new SubscribablePushButton(NULL, tr("C"));
    colonizeButton->setObjectName("colonize");
    SubscribablePushButton * loadButton = new SubscribablePushButton(NULL, tr("L"));
    loadButton->setObjectName("load");
    SubscribablePushButton * unloadButton = new SubscribablePushButton(NULL, tr("U"));
    unloadButton->setObjectName("unload");
    SubscribablePushButton * buildShipButton = new SubscribablePushButton(NULL, tr("B.."));
    buildShipButton->setObjectName("build");
    SubscribablePushButton * nextTurnButton = new SubscribablePushButton(NULL, tr("T"));
    nextTurnButton->setObjectName("turn");
    SubscribablePushButton * shipDesignButton = new SubscribablePushButton(NULL, tr("D.."));
    shipDesignButton->setObjectName("design");
    SubscribablePushButton * researchButton = new SubscribablePushButton(NULL, tr("R.."));
    researchButton->setObjectName("research");
    SubscribablePushButton * newButton = new SubscribablePushButton(NULL, tr("N.."));
    newButton->setObjectName("new");
    SubscribablePushButton * openButton = new SubscribablePushButton(NULL, tr("O.."));
    openButton->setObjectName("open");
    SubscribablePushButton * saveButton = new SubscribablePushButton(NULL, tr("S.."));
    saveButton->setObjectName("save");
    SubscribablePushButton * quitButton = new SubscribablePushButton(NULL, tr("Q"));
    quitButton->setObjectName("quit");
    SubscribablePushButton * helpButton = new SubscribablePushButton(NULL, tr("H.."));
    helpButton->setObjectName("help");
    SubscribablePushButton * setupButton = new SubscribablePushButton(NULL, tr("S.."));
    setupButton->setObjectName("setup");
    SubscribablePushButton * flagButton = new SubscribablePushButton(NULL, tr("F"));
    flagButton->setObjectName("flag");

    formLayout->addRow(newButton, new QLabel("Start a new game."));
    formLayout->addRow(openButton, new QLabel("Open a saved game."));
    formLayout->addRow(saveButton, new QLabel("Save current game."));
    formLayout->addRow(quitButton, new QLabel("Quit the program."));
    formLayout->addRow(helpButton, new QLabel("This help panel."));
    formLayout->addRow(setupButton, new QLabel("Setup panel."));
    formLayout->addRow(flagButton, new QLabel("Center map on your home system."));
    formLayout->addRow(moveShipButton, new QLabel("Move selected ship."));
    formLayout->addRow(loadButton, new QLabel("Load population from planet to selected ship."));
    formLayout->addRow(unloadButton, new QLabel("Unload population from selected ship to planet."));
    formLayout->addRow(colonizeButton, new QLabel("Order selected ship to colonize planet."));
    formLayout->addRow(buildShipButton, new QLabel("Create a build order for an existing ship design."));
    formLayout->addRow(shipDesignButton, new QLabel("Create a new ship design."));
    formLayout->addRow(researchButton, new QLabel("Research new technologies."));
    formLayout->addRow(nextTurnButton, new QLabel("Advance time by one month."));

    QScrollArea * scrollArea = new QScrollArea();
    scrollArea->setWidget(buttonsWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setObjectName("helpButtonsScrollArea");
    topLayout->addWidget(scrollArea);
    QsKineticScroller * kineticScroller = new QsKineticScroller(scrollArea);
    kineticScroller->enableKineticScrollFor(scrollArea);

    QBoxLayout * buttonLayout = new QHBoxLayout();
    topLayout->addItem(buttonLayout);
    SubscribablePushButton * closeButton = new SubscribablePushButton(this, tr("Close"));
    closeButton->setObjectName("closeButton");
    new PrivateSubscriberClose(closeButton);
    buttonLayout->addStretch();   
    buttonLayout->addWidget(closeButton);   
}