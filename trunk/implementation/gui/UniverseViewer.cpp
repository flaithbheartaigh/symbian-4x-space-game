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

#include "UniverseViewer.h"
#include "MainWindow.h"
#include "QsKineticScroller.h"
#include "SectorGraphicsItem.h"
#include "UniverseScene.h"
#include "SectorListView.h"

#include <game/Player.h>
#include <game/StarSystem.h>
#include <game/Messages.h>

#include <QMouseEvent>
#include <QSlider>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QLabel>
#include <QLayout>
#include <QListView>
#include <QGridLayout>

#ifdef Q_OS_BLACKBERRY
# include <math.h>
#endif
#include <cmath>

#define USE_CLIPPING 0
#define SHOW_DRAWTIME 0

namespace
{
    double _referenceSize(const int qSetting)
    {
        switch (qSetting)
        {
        case 0:
            return 8.0;
        case 1:
            return 16.0;
        case 2:
            return 24.0;
        case 3:
            return 32.0;
        case 4:
            return 48.0;
        case 5:
            return 64.0;
        case 6:
            return 128.0;
        default:
            return 32.0;
        }
    }

    class ScalableGraphicsView
        : public QGraphicsView
        , private Game::Universe::Game::Subscriber
    {

    public:

        ~ScalableGraphicsView();

        ScalableGraphicsView(QGraphicsScene * scene, QWidget * parent, QSlider * zoomSlider);

    protected:

        void wheelEvent(QWheelEvent * event);

        void mouseDoubleClickEvent(QMouseEvent  * event);

        void paintEvent(QPaintEvent * event);

    private:

        void playerActivated(Game::Player * player);

        QSlider * mZoomSlider;

        ScalableGraphicsView(const ScalableGraphicsView & other);
        
        ScalableGraphicsView & operator =(const ScalableGraphicsView & other);
        
    };

    ScalableGraphicsView::~ScalableGraphicsView()
    {

    }

    ScalableGraphicsView::ScalableGraphicsView(QGraphicsScene * scene, QWidget * parent, QSlider * zoomSlider)
        : QGraphicsView(scene, parent)
        , Game::Universe::Game::Subscriber()
        , mZoomSlider(zoomSlider)
    {
        setSceneRect(QRectF(
            -7*Gui::UniverseViewer::ReferenceSize*Game::StarSystem::Size, 
            -7*Gui::UniverseViewer::ReferenceSize*Game::StarSystem::Size, 
            13*Gui::UniverseViewer::ReferenceSize*Game::StarSystem::Size, 
            13*Gui::UniverseViewer::ReferenceSize*Game::StarSystem::Size));

        setDragMode(QGraphicsView::ScrollHandDrag);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QsKineticScroller * kineticScroller = new QsKineticScroller(this);
        kineticScroller->enableKineticScrollFor(this);

        setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
        setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);
    }

    void ScalableGraphicsView::wheelEvent(QWheelEvent * event)
    {
        if (event->orientation() == Qt::Vertical)
        {
            if (event->delta() > 0)
            {
                scale(1.1,1.1);
            }
            else if (event->delta() < 0)
            {
                scale(0.9, 0.9);
            }

            QTransform scaleTransform = transform();
            int value = int(10.0*log(scaleTransform.m11())/log(2.0));
            bool blocked = mZoomSlider->blockSignals(true);
            mZoomSlider->setValue(value);
            mZoomSlider->blockSignals(blocked);
        }
        else
        {
            QGraphicsView::wheelEvent(event);
        }
    }

    void ScalableGraphicsView::mouseDoubleClickEvent(QMouseEvent * event)
    {
        centerOn(mapToScene(event->pos()));
        QTransform viewTransform = transform();
        viewTransform.scale((32.0/Gui::UniverseViewer::ReferenceSize)/viewTransform.m11(), (32.0/Gui::UniverseViewer::ReferenceSize)/viewTransform.m22());
        setTransform(viewTransform);
        QGraphicsView::mouseDoubleClickEvent(event);

        QTransform scaleTransform = transform();
        int value = int(10.0*log(scaleTransform.m11())/log(2.0));
        bool blocked = mZoomSlider->blockSignals(true);
        mZoomSlider->setValue(value);
        mZoomSlider->blockSignals(blocked);
    }

    void ScalableGraphicsView::paintEvent(QPaintEvent * event)
    {
#if SHOW_DRAWTIME
        int start_s = clock();
#endif
	    QGraphicsView::paintEvent(event);
#if SHOW_DRAWTIME
        int stop_s = clock();
        int result = (stop_s-start_s)/double(CLOCKS_PER_SEC) * 1000;
        std::stringstream ss;
        ss << result;
        Game::Messages::instance().post(ss.str());
#endif
    }

    void ScalableGraphicsView::playerActivated(Game::Player * player)
    {
        if (Gui::MainWindow::Settings_CacheMode >= 1)
        {
            QList<QGraphicsItem *> sceneItems = items();
            for (int i = 0; i < sceneItems.count(); ++i)
            {
                Gui::SectorGraphicsItem * sectorItem = dynamic_cast<Gui::SectorGraphicsItem *>(sceneItems[i]);
                if (sectorItem != NULL)
                {
                    sectorItem->update();
                }
            }
        }
        if (player != NULL)
        {
            QList<QGraphicsItem *> sceneItems = items();
            for (int i = 0; i < sceneItems.count(); ++i)
            {
                Gui::SectorGraphicsItem * sectorItem = dynamic_cast<Gui::SectorGraphicsItem *>(sceneItems[i]);
                if (sectorItem != NULL && sectorItem->sector() == player->selectedSector().sector())
                {
                    centerOn(sectorItem);
                    break;
                }
            }
        }
        update();
    }
}

using namespace Gui;

double UniverseViewer::ReferenceSize;

UniverseViewer::~UniverseViewer()
{

}

UniverseViewer::UniverseViewer(QWidget * parent)
    : QWidget(parent)
    , mGraphicsScene(NULL)
    , mZoomSlider(new QSlider(Qt::Horizontal, this))
    , mMessageBox(new QLabel(tr(""), this))
    , mSectorView(NULL)
{
    ReferenceSize = _referenceSize(MainWindow::Settings_TileResolution);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    class PrivateMessagesSubscriber
        : public QObject
        , private Game::Messages::Subscriber
    {

    public:

        ~PrivateMessagesSubscriber()
        {
        }

        PrivateMessagesSubscriber(QLabel * label)
            : QObject(label)
            , Game::Messages::Subscriber()
            , mLabel(label)
        {

        }

    private:

        void messagePosted(const std::string & message)
        {
            QFontMetricsF metrics(mLabel->font());
            int mWidth = metrics.width(QString::fromStdString(message));
            int mHeight = metrics.height();

            mLabel->setText(QString::fromStdString(message));
            mLabel->resize(mWidth, mHeight);
        }

        QLabel * mLabel;
    };

    new PrivateMessagesSubscriber(mMessageBox);

    mGraphicsScene = new UniverseScene(this);
    QGraphicsView * view = new ScalableGraphicsView(mGraphicsScene, this, mZoomSlider);
    setLayout(new QGridLayout());
    layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(view);

    view->setTransform(QTransform::fromScale(32.0/UniverseViewer::ReferenceSize,32.0/UniverseViewer::ReferenceSize));

    mZoomSlider->raise();
    mZoomSlider->setMinimum(-4*10);
    mZoomSlider->setMaximum(2*10);
    mZoomSlider->setTracking(true);
    mZoomSlider->setPageStep(2*10);
    connect(mZoomSlider, SIGNAL(valueChanged(int)), SLOT(slot_valueChanged(int)));

    mMessageBox->raise();

    QTransform scaleTransform = view->transform();
    int value = int(10.0*log(scaleTransform.m11())/log(2.0));
    bool blocked = mZoomSlider->blockSignals(true);
    mZoomSlider->setValue(value);
    mZoomSlider->blockSignals(blocked);

    mSectorView = new SectorListView(this);
}

void UniverseViewer::centerOn(Game::Sector * sector)
{
    QList<QGraphicsItem *> sceneItems = mGraphicsScene->views()[0]->items();
    for (int i = 0; i < sceneItems.count(); ++i)
    {
        SectorGraphicsItem * sectorItem = dynamic_cast<SectorGraphicsItem *>(sceneItems[i]);
        if (sectorItem != NULL && sectorItem->sector() == sector)
        {
            mGraphicsScene->views()[0]->centerOn(sectorItem);
            break;
        }
    }

    QTransform viewTransform = mGraphicsScene->views()[0]->transform();
    viewTransform.scale((32.0/UniverseViewer::ReferenceSize)/viewTransform.m11(), (32.0/UniverseViewer::ReferenceSize)/viewTransform.m22());
    mGraphicsScene->views()[0]->setTransform(viewTransform);

    int value = int(10.0*log(viewTransform.m11())/log(2.0));
    bool blocked = mZoomSlider->blockSignals(true);
    mZoomSlider->setValue(value);
    mZoomSlider->blockSignals(blocked);
}

QItemSelectionModel * UniverseViewer::sectorSelectionModel() const
{
    return mSectorView->selectionModel();
}

void UniverseViewer::noForce(bool noForce)
{
    mGraphicsScene->views()[0]->setProperty("NoForce", noForce);
}

void UniverseViewer::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    QSize sliderSize(event->size().width(), event->size().height() / 8.0);
    mZoomSlider->resize(sliderSize);
    mZoomSlider->move(0, event->size().height() - sliderSize.height());

    mMessageBox->move(event->size().width() / 4.0, 0);
}

void UniverseViewer::slot_valueChanged(int value)
{
    if (!mGraphicsScene->views()[0]->items().empty())
    {
        QTransform scaleTransform = mGraphicsScene->views()[0]->transform();
        scaleTransform.scale(pow(2.0, value/10.0) / scaleTransform.m11(), pow(2.0, value/10.0) / scaleTransform.m22());
        mGraphicsScene->views()[0]->setTransform(scaleTransform);
    }
}
