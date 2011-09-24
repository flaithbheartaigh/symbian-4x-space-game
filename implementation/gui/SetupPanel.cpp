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

#include "SetupPanel.h"
#include "SubscribablePushButton.h"
#include "MainWindow.h"

#include <QCheckBox>
#include <QSlider>
#include <QBoxLayout>
#include <QFormLayout>
#include <QSettings>

using namespace Gui;

SetupPanel::~SetupPanel()
{

}

SetupPanel::SetupPanel(QWidget * parent)
    : QFrame(parent)
{
    class PrivateSubscriberOK
        : public QObject
        , private SubscribablePushButton::Subscriber
    {

    public:

        ~PrivateSubscriberOK()
        {
        }

        PrivateSubscriberOK(SubscribablePushButton * pushButton, QSlider * cacheMode, QSlider * tileResolution, QSlider * detailLevel, QCheckBox * skipEmptyTiles, 
            QCheckBox * viewNPCTurns, QCheckBox * viewUniverse)
            : QObject(pushButton)
            , SubscribablePushButton::Subscriber(pushButton)
            , mCacheMode(cacheMode)
            , mTileResolution(tileResolution)
            , mDetailLevel(detailLevel)
            , mSkipEmptyTiles(skipEmptyTiles)
            , mViewNPCTurns(viewNPCTurns)
            , mViewUniverse(viewUniverse)
        {
        }

    private:

        void clicked(bool checked)
        {
            QSettings settings("Patrick Pelletier","SpaceEmpiresQt");
            settings.setValue("graphics/cacheMode", mCacheMode->value());
            settings.setValue("graphics/tileResolution", mTileResolution->value());
            settings.setValue("graphics/detailLevel", mDetailLevel->value());
            settings.setValue("graphics/skipEmptyTiles", mSkipEmptyTiles->isChecked());
            settings.setValue("game/viewNPCTurns", mViewNPCTurns->isChecked());
            settings.setValue("game/viewUniverse", mViewUniverse->isChecked());
            MainWindow::instance().showFrame(MainWindow::MainFrameIndex);
        }

        QSlider * mCacheMode;

        QSlider * mTileResolution;

        QSlider * mDetailLevel;

        QCheckBox * mSkipEmptyTiles;

        QCheckBox * mViewNPCTurns;

        QCheckBox * mViewUniverse;
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

    QSettings settings("Patrick Pelletier","SpaceEmpiresQt");

    QSlider * cacheMode = new QSlider(Qt::Horizontal);
    cacheMode->setTickPosition(QSlider::TicksBothSides);
    cacheMode->setMinimum(0);
    cacheMode->setMaximum(3); //0- None; 1- Sectors; 2- Sectors+Labels; 3- Sectors+Labels+Systems
    cacheMode->setPageStep(1);
    cacheMode->setValue(settings.value("graphics/cacheMode", MainWindow::Settings_CacheMode).toInt());

    QSlider * tileResolution = new QSlider(Qt::Horizontal);
    tileResolution->setTickPosition(QSlider::TicksBothSides);
    tileResolution->setMinimum(0);
    tileResolution->setMaximum(6); //0- 8x8; 1- 16x16; 2- 24x24; 3- 32x32; 4- 48x48; 5- 64x64; 6- 128x128
    tileResolution->setPageStep(1);
    tileResolution->setValue(settings.value("graphics/tileResolution", MainWindow::Settings_TileResolution).toInt());

    QSlider * detailLevel = new QSlider(Qt::Horizontal);
    detailLevel->setTickPosition(QSlider::TicksBothSides);
    detailLevel->setMinimum(0);
    detailLevel->setMaximum(2); //0- None; 1- Gradients; 2- Extra
    detailLevel->setPageStep(1);
    detailLevel->setValue(settings.value("graphics/detailLevel", MainWindow::Settings_DetailLevel).toInt());

    QCheckBox * skipEmptyTiles = new QCheckBox();
    skipEmptyTiles->setChecked(settings.value("graphics/skipEmptyTiles", MainWindow::Settings_SkipEmptyTiles).toBool());

    QCheckBox * viewNPCTurns = new QCheckBox();
    viewNPCTurns->setChecked(settings.value("game/viewNPCTurns", MainWindow::Settings_ViewUniverse).toBool());

    QCheckBox * viewUniverse = new QCheckBox();
    viewUniverse->setChecked(settings.value("game/viewUniverse", MainWindow::Settings_ViewNPCTurns).toBool());

    QFormLayout * formLayout = new QFormLayout();
    formLayout->addRow(tr("&Cache mode:"), cacheMode);
    formLayout->addRow(tr("&Cache resolution:"), tileResolution);
    formLayout->addRow(tr("&Detail level:"), detailLevel);
    formLayout->addRow(tr("&Skip empty tiles:"), skipEmptyTiles);
    formLayout->addRow(tr("&View NPC turns:"), viewNPCTurns);
    formLayout->addRow(tr("&View entire universe:"), viewUniverse);
    topLayout->addLayout(formLayout);
    topLayout->addStretch();

    QBoxLayout * buttonLayout = new QHBoxLayout();
    topLayout->addItem(buttonLayout);
    SubscribablePushButton * okButton = new SubscribablePushButton(this, tr("OK"));
    okButton->setObjectName("okButton");
    new PrivateSubscriberOK(okButton, cacheMode, tileResolution, detailLevel, skipEmptyTiles, viewNPCTurns, viewUniverse);
    SubscribablePushButton * cancelButton = new SubscribablePushButton(this, tr("Cancel"));
    cancelButton->setObjectName("cancelButton");
    new PrivateSubscriberCancel(cancelButton);
    buttonLayout->addStretch();   
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);   
}