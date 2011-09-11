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

#include "MainWindow.h"
#include "UniverseViewer.h"
#include "ShipConfigDesign.h"
#include "ShipConfigBuild.h"
#include "ComponentSelection.h"
#include "SubscribablePushButton.h"
#include "SectorItemModel.h"
#include "UniversePainter.h"
#include "QsKineticScroller.h"
#include "PlanetListModel.h"
#include "SetupPanel.h"

#include <data/NamesData.h>
#include <data/AssetSerializer.h>

#include <game/Component.h>
#include <game/Parameters.h>
#include <game/Universe.h>
#include <game/Sector.h>
#include <game/Planet.h>
#include <game/Ship.h>
#include <game/Player.h>
#include <game/AI.h>
#include <game/SectorReference.h>
#include <game/DeserializeVisitor.h>
#include <game/StatsVisitor.h>

#include <QDialogButtonBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QResizeEvent>
#include <QListView>
#include <QPainter>
#include <QStackedWidget>
#include <QMessageBox>

#include <QScrollArea>

#include <algorithm>

namespace
{
    class PrivateStatsFrame
        : public QFrame
    {

    public:

        ~PrivateStatsFrame()
        {
        }

        PrivateStatsFrame(QWidget * parent)
            : QFrame(parent)
            , mName(new QLabel(this))
            , mMoney(new QLabel(this))
            , mPlanets(new QLabel(this))
            , mPopulation(new QLabel(this))
            , mShips(new QLabel(this))
            , mSystemsExplored(new QLabel(this))
            , mPlanetList(new QListView(this))
        {
            setObjectName("StatsFrame");
            setLayout(new QVBoxLayout());
            QFormLayout * formLayout = new QFormLayout();
            formLayout->addRow(tr("&Name:"), mName);
            formLayout->addRow(tr("&Money:"), mMoney);
            formLayout->addRow(tr("&Planets:"), mPlanets);
            formLayout->addRow(tr("&Population:"), mPopulation);
            formLayout->addRow(tr("&Ships:"), mShips);
            formLayout->addRow(tr("&Systems explored:"), mSystemsExplored);
            static_cast<QBoxLayout *>(layout())->addLayout(formLayout);
            layout()->addWidget(mPlanetList);
        }

    protected:

        void paintEvent(QPaintEvent *)
        {
            Game::Player * player = Game::Universe::instance().game().currentPlayer();
            if (player != NULL)
            {
                Game::StatsVisitor stats(player);
                Game::Universe::instance().accept(&stats);
                mName->setText(QString::fromStdString(player->name()));
                mMoney->setText(QString("%1").arg(player->money()));
                mPlanets->setText(QString("%1").arg(stats.mPlanets.size()));
                mPopulation->setText(QString("%1").arg(stats.mPopulation));
                mShips->setText(QString("%1").arg(stats.mShips.size()));
                mSystemsExplored->setText(QString("%1").arg(player->knownSystems().size()));
                if (mPlanetList->property("Player").toString().toStdString() != player->name()
                    || mPlanetList->property("Turn").toUInt() != Game::Universe::instance().currentTurn())
                {
                    mPlanetList->setProperty("Player", QString::fromStdString(player->name()));
                    mPlanetList->setProperty("Turn", Game::Universe::instance().currentTurn());
                    Gui::PlanetListModel * model = new Gui::PlanetListModel(mPlanetList);
                    model->setPlanets(stats.mPlanets);
                    delete mPlanetList->model();
                    mPlanetList->setModel(model);
                }
            }
        }

    private:

        QLabel * mName;

        QLabel * mMoney;

        QLabel * mPlanets;

        QLabel * mPopulation;

        QLabel * mShips;

        QLabel * mSystemsExplored;

        QListView * mPlanetList;
    };

    class PrivateCurrentPlayer
        : public QFrame
    {

    public:

        ~PrivateCurrentPlayer()
        {
        }

        PrivateCurrentPlayer(QWidget * parent, Gui::UniverseViewer * universeViewer)
            : QFrame(parent)
            , mUniverseViewer(universeViewer)
        {
            setObjectName("CurrentPlayer");

            class PrivateGameSubscriber
                : private QObject
                , private Game::Universe::Game::Subscriber
            {

            public:

                ~PrivateGameSubscriber()
                {

                }

                PrivateGameSubscriber(QWidget * parent)
                    : QObject(parent)
                    , Game::Universe::Game::Subscriber()
                    , mWidget(parent)
                {

                }

                void playerActivated(Game::Player *)
                {
                    if (mWidget != NULL)
                    {
                        mWidget->update();
                    }
                }

            private:

                QWidget * mWidget;

            };     

            new PrivateGameSubscriber(this);
        }

    protected:

        void mousePressEvent(QMouseEvent *)
        {
            if (mUniverseViewer != NULL)
            {
                Game::Player * player = Game::Universe::instance().game().currentPlayer();
                if (player != NULL)
                {
                    mUniverseViewer->centerOn(player->homeSector().sector());
                }
            }
        }

        void paintEvent(QPaintEvent *)
        {
            Game::Player * player = Game::Universe::instance().game().currentPlayer();
            if (player != NULL)
            {
                QPainter painter(this);
                painter.translate(contentsRect().x()+contentsRect().width()/2,contentsRect().y()+contentsRect().height()/2);
                Gui::UniversePainter universePainter;
                universePainter.paintBadge(&painter, player, contentsRect().size(), false);
            }
        }

    private:

        Gui::UniverseViewer * mUniverseViewer;

    };

    class PrivateSubscriberSetVisible
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberSetVisible()
        {
        }

        PrivateSubscriberSetVisible(Gui::SubscribablePushButton * pushButton, QWidget * widget)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
            , mWidget(widget)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (mWidget != NULL)
            {
                mWidget->setVisible(checked);
            }
        }

        QWidget * mWidget;
    };

    class PrivateSubscriberQuit
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberQuit()
        {
        }

        PrivateSubscriberQuit(Gui::SubscribablePushButton * pushButton, QWidget * parent)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
            , mParent(parent)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (mParent != NULL && QMessageBox::question(NULL, tr("Quit"), tr("Are you sure you want to quit?"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok)
            {
                mParent->close();
            }
        }

        QWidget * mParent;
    };

    class PrivateSubscriberNewGame
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberNewGame()
        {
        }

        PrivateSubscriberNewGame(Gui::SubscribablePushButton * pushButton)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
        {
        }

    private:

        void clicked(bool checked)
        {
            std::vector<std::string> starNames;
            Data::NamesData("stars.json", &starNames, Data::NamesData::Load);
            std::vector<std::string> playerNames;
            Data::NamesData("empires.json", &playerNames, Data::NamesData::Load);
            std::vector<Game::Component> components;
            Data::AssetSerializer::load(Game::Parameters::instance().getDataFilePath("configs/components.json"), components);

            QStringList items;
            for (std::vector<std::string>::iterator it = playerNames.begin(); it != playerNames.end(); ++it)
            {
                items << QString::fromStdString(*it);
            }
            bool ok = false;
            QString item = QInputDialog::getItem(NULL, tr("Select an Empire"), tr("Name:"), items, 0, false, &ok);
            if (ok && !item.isEmpty())
            {
                playerNames.erase(std::remove(playerNames.begin(), playerNames.end(), item.toStdString()), playerNames.end());  

                Game::Universe::instance().clear();
                Game::Universe::instance().names().setAvailableStarNames(starNames);
                Game::Universe::instance().names().setAvailablePlayerNames(playerNames);

                Game::Player * human = new Game::Player();
                human->setName(item.toStdString());
                human->setComponents(components);
                Game::Universe::instance().game().addPlayer(human);

                Game::Player * comp = new Game::Player();
                comp->setName(Game::Universe::instance().names().nextAvailablePlayerName());
                comp->setComponents(components);
                comp->setAI(new Game::AI(comp));
                Game::Universe::instance().game().addPlayer(comp);
                Game::Universe::instance().generate();
                Game::Universe::instance().game().setCurrentPlayerIndex(0);
            }
        }
    };

    class PrivateSubscriberOpenGame
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberOpenGame()
        {
        }

        PrivateSubscriberOpenGame(Gui::SubscribablePushButton * pushButton)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
        {
        }

    private:

        void clicked(bool checked)
        {
            QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open Game"), QString(), tr("Json (*.* *.json)"));
            if (!fileName.isEmpty())
            {
                Game::Universe::instance().clear();
                Data::AssetSerializer::load(fileName.toStdString(), Game::Universe::instance());
                Game::Universe::instance().accept(&Game::DeserializeVisitor());
            }
        }
    };

    class PrivateSubscriberSaveGame
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberSaveGame()
        {
        }

        PrivateSubscriberSaveGame(Gui::SubscribablePushButton * pushButton)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
        {
        }

    private:

        void clicked(bool checked)
        {
            QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save Game As"), QString(), tr("Json (*.json)"));
            if (!fileName.isEmpty())
            {
                Data::AssetSerializer::save(fileName.contains('.') ? fileName.toStdString() : fileName.append(".json").toStdString(), Game::Universe::instance());
            }
        }
    };

    class PrivateSubscriberShipDesign
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberShipDesign()
        {
        }

        PrivateSubscriberShipDesign(Gui::SubscribablePushButton * pushButton, QStackedWidget * stack)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
            , mStackedWidget(stack)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (mStackedWidget != NULL)
            {
                mStackedWidget->setCurrentIndex(Gui::MainWindow::ShipDesignIndex);
            }
            QWidget * widget = Gui::MainWindow::instance().getFrame(Gui::MainWindow::ShipDesignIndex);
            Gui::ShipConfigDesign * shipConfigDesign = dynamic_cast<Gui::ShipConfigDesign *>(widget);
            if (shipConfigDesign == NULL)
            {
                shipConfigDesign = widget->findChild<Gui::ShipConfigDesign *>();
            }
            if (shipConfigDesign != NULL)
            {
                shipConfigDesign->loadDesigns();
            }
        }

        QStackedWidget * mStackedWidget;
    };

    class PrivateSubscriberNextTurn
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberNextTurn()
        {
        }

        PrivateSubscriberNextTurn(Gui::SubscribablePushButton * pushButton, Gui::UniverseViewer * universeViewer)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
            , mUniverseViewer(universeViewer)
        {
        }

    private:

        void clicked(bool checked)
        {
            mUniverseViewer->noForce(true);
            Game::Universe::instance().update();
            mUniverseViewer->noForce(false);
        }

        Gui::UniverseViewer * mUniverseViewer;
    };

    class PrivateSubscriberShipBuild
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberShipBuild()
        {
        }

        PrivateSubscriberShipBuild(Gui::SubscribablePushButton * pushButton, QStackedWidget * stack)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
            , mStackedWidget(stack)
        {
        }

    private:

        void clicked(bool checked)
        {
            Game::Sector * sector = NULL;
            if (Game::Universe::instance().game().currentPlayer() != NULL)
            {
                sector = Game::Universe::instance().game().currentPlayer()->selectedSector().sector();
            }
            if (sector != NULL)
            {
                if (mStackedWidget != NULL)
                {
                    mStackedWidget->setCurrentIndex(Gui::MainWindow::ShipBuildIndex);
                }
                QWidget * widget = Gui::MainWindow::instance().getFrame(Gui::MainWindow::ShipBuildIndex);
                Gui::ShipConfigBuild * shipConfigBuild = dynamic_cast<Gui::ShipConfigBuild *>(widget);
                if (shipConfigBuild == NULL)
                {
                    shipConfigBuild = widget->findChild<Gui::ShipConfigBuild *>();
                }
                if (shipConfigBuild != NULL)
                {
                    shipConfigBuild->setSector(sector);
                    shipConfigBuild->loadDesigns();
                }
            }
        }

        QStackedWidget * mStackedWidget;
    };

    class PrivateSubscriberMoveShip
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
        , private Game::Player::Current::Subscriber
    {

    public:

        ~PrivateSubscriberMoveShip()
        {
        }

        PrivateSubscriberMoveShip(Gui::SubscribablePushButton * pushButton, QItemSelectionModel * selectionModel)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
            , Game::Player::Current::Subscriber()
            , mMoveButton(pushButton)
            , mSelectionModel(selectionModel)
            , mSector()
            , mSectorFrom()
            , mSelectedRows()
        {
        }

    private:

        void clicked(bool checked)
        {
            if (checked)
            {
                mSectorFrom = mSector;
                if (mSelectionModel != NULL && Game::Universe::instance().game().currentPlayer() != NULL)
                {
                    const Gui::SectorItemModel * itemModel = static_cast<const Gui::SectorItemModel *>(mSelectionModel->model());
                    if (itemModel != NULL)
                    {
                        mSelectedRows = mSelectionModel->selectedRows();
                    }
                }
            }
            else
            {
                mSectorFrom = Game::SectorReference();
                mSelectedRows.clear();
            }
        }

        void selectedSectorChanged(Game::Sector * sector)
        {
            mSector = Game::SectorReference(sector);
            if (mMoveButton != NULL && mMoveButton->isChecked())
            {
                if (mSector.isValid() && mSectorFrom.isValid() && Game::Universe::instance().game().currentPlayer() != NULL)
                {
                    if (mSelectedRows.count() > 0)
                    {
                        Gui::SectorItemModel itemModel(NULL, mSectorFrom.sector());
                        std::vector<Game::Ship *> ships = itemModel.ships(mSelectedRows);
                        for (unsigned int i = 0; i < ships.size(); ++i)
                        {
                            if (ships[i] != NULL && ships[i]->player() == Game::Universe::instance().game().currentPlayer())
                            {
                                if (ships[i]->canMoveTo(mSector.sector()))
                                {
                                    ships[i]->moveTo(mSector.sector());
                                }
                            }
                        }
                    }
                }

                mMoveButton->click();
            }
        }

        Gui::SubscribablePushButton * mMoveButton;

        QItemSelectionModel * mSelectionModel;

        Game::SectorReference mSector;

        Game::SectorReference mSectorFrom;

        QModelIndexList mSelectedRows;
    };

    class PrivateSubscriberColonize
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberColonize()
        {
        }

        PrivateSubscriberColonize(Gui::SubscribablePushButton * pushButton, QItemSelectionModel * selectionModel)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
            , mSelectionModel(selectionModel)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (mSelectionModel != NULL)
            {
                const Gui::SectorItemModel * itemModel = static_cast<const Gui::SectorItemModel *>(mSelectionModel->model());
                if (itemModel != NULL)
                {
                    std::vector<Game::Ship *> ships = itemModel->ships(mSelectionModel->selectedRows());
                    for (unsigned int i = 0; i < ships.size(); ++i)
                    {
                        if (ships[i] != NULL && ships[i]->player() == Game::Universe::instance().game().currentPlayer())
                        {
                            if (ships[i]->canColonize())
                            {
                                ships[i]->colonize();
                            }
                        }
                    }
                }
            }
        }

        QItemSelectionModel * mSelectionModel;
    };

    class PrivateSubscriberShowPanel
        : public QObject
        , private Gui::SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberShowPanel()
        {
        }

        PrivateSubscriberShowPanel(Gui::SubscribablePushButton * pushButton, QStackedWidget * stack, int index)
            : QObject(pushButton)
            , Gui::SubscribablePushButton::Subscriber(pushButton)
            , mStackedWidget(stack)
            , mIndex(index)
        {
        }

    private:

        void clicked(bool checked)
        {
            if (mStackedWidget != NULL)
            {
                mStackedWidget->setCurrentIndex(mIndex);
            }
        }

        QStackedWidget * mStackedWidget;

        int mIndex;
    };
}

using namespace Gui;

const int MainWindow::MainFrameIndex = 0;
const int MainWindow::StatsFrameIndex = 1;
const int MainWindow::ShipDesignIndex = 2;
const int MainWindow::ShipBuildIndex = 3;
const int MainWindow::ComponentsIndex = 4;
const int MainWindow::SetupIndex = 5;

MainWindow * MainWindow::_instance = NULL;

MainWindow & MainWindow::instance()
{
    if (_instance == NULL)
    {
        _instance = new MainWindow(); 
    }
    return *_instance;
}

MainWindow::~MainWindow()
{
    _instance = NULL;
}

MainWindow::MainWindow()
#ifdef Q_OS_SYMBIAN
    : QFrame(NULL, Qt::FramelessWindowHint)
#else
    : QFrame(NULL, NULL)
#endif
    , mStack(new QStackedWidget())
{
    setLayout(new QHBoxLayout());
    layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(mStack);

    QFrame * mainFrame = new QFrame();
    mStack->insertWidget(MainFrameIndex, mainFrame);
    mStack->insertWidget(StatsFrameIndex, new PrivateStatsFrame(NULL));
    mStack->insertWidget(ShipDesignIndex, new ShipConfigDesign(NULL));
    mStack->insertWidget(ShipBuildIndex, new ShipConfigBuild(NULL));
    mStack->insertWidget(ComponentsIndex, new ComponentSelection(NULL));
    mStack->insertWidget(SetupIndex, new SetupPanel(NULL));

    UniverseViewer * universeViewer = new UniverseViewer(mainFrame);

    SubscribablePushButton * moveShipButton = new SubscribablePushButton(NULL, tr("M"));
    moveShipButton->setObjectName("move");
    moveShipButton->setCheckable(true);

    SubscribablePushButton * colonizeButton = new SubscribablePushButton(NULL, tr("C"));
    colonizeButton->setObjectName("colonize");

    SubscribablePushButton * buildShipButton = new SubscribablePushButton(NULL, tr("B.."));
    buildShipButton->setObjectName("build");

    SubscribablePushButton * nextTurnButton = new SubscribablePushButton(NULL, tr("T"));
    nextTurnButton->setObjectName("turn");

    SubscribablePushButton * shipDesignButton = new SubscribablePushButton(NULL, tr("D.."));
    shipDesignButton->setObjectName("design");

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

    QVBoxLayout * buttonsLayout = new QVBoxLayout();
    buttonsLayout->setContentsMargins(0,0,0,0);
    buttonsLayout->addWidget(newButton);
    buttonsLayout->addWidget(openButton);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(quitButton);
    buttonsLayout->addWidget(helpButton);
    buttonsLayout->addWidget(setupButton);
    buttonsLayout->addWidget(new PrivateCurrentPlayer(NULL, universeViewer));
    buttonsLayout->addWidget(moveShipButton);
    buttonsLayout->addWidget(colonizeButton);
    buttonsLayout->addWidget(buildShipButton);
    buttonsLayout->addWidget(shipDesignButton);
    buttonsLayout->addWidget(nextTurnButton);
    buttonsLayout->addStretch();
    QFrame * buttonsWidget = new QFrame(NULL);
    buttonsWidget->setLayout(buttonsLayout);
    buttonsWidget->setObjectName("buttonsWidget");
    buttonsWidget->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    mainFrame->setLayout(new QHBoxLayout());
    mainFrame->layout()->setContentsMargins(0,0,0,0);
    mainFrame->layout()->setSpacing(0);
    mainFrame->layout()->addWidget(universeViewer);

    QScrollArea * scrollArea = new QScrollArea();
    scrollArea->setWidget(buttonsWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setObjectName("buttonsScrollArea");
    scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    mainFrame->layout()->addWidget(scrollArea);
    QsKineticScroller * kineticScroller = new QsKineticScroller(scrollArea);
    kineticScroller->enableKineticScrollFor(scrollArea);

    new PrivateSubscriberMoveShip(moveShipButton, universeViewer->sectorSelectionModel());
    new PrivateSubscriberColonize(colonizeButton, universeViewer->sectorSelectionModel());
    new PrivateSubscriberShipBuild(buildShipButton, mStack);
    new PrivateSubscriberNextTurn(nextTurnButton, universeViewer);
    new PrivateSubscriberShipDesign(shipDesignButton, mStack);
    new PrivateSubscriberNewGame(newButton);
    new PrivateSubscriberOpenGame(openButton);
    new PrivateSubscriberSaveGame(saveButton);
    new PrivateSubscriberQuit(quitButton, this);
    new PrivateSubscriberShowPanel(setupButton, mStack, SetupIndex);
}

void MainWindow::showFrame(int index)
{
    if (mStack->currentIndex() != index)
    {
        mStack->setCurrentIndex(index);
    }
}

QWidget * MainWindow::getFrame(int index) const
{
    return mStack->widget(index);
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QFrame::resizeEvent(event);
    if (event->size().width() < event->size().height())
    {
        if (mStack->currentIndex() == MainFrameIndex)
        {
            mStack->setCurrentIndex(StatsFrameIndex);
        }
    }
    else
    {
        if (mStack->currentIndex() == StatsFrameIndex)
        {
            mStack->setCurrentIndex(MainFrameIndex);
        }
    }
}