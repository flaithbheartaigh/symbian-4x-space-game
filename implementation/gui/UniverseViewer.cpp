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
#include "UniversePainter.h"
#include "MainWindow.h"
#include "QsKineticScroller.h"
#include "SectorListView.h"

#include <game/Universe.h>
#include <game/StarSystem.h>
#include <game/Sector.h>
#include <game/Star.h>
#include <game/Player.h>
#include <game/Messages.h>

#include <QGridLayout>
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <QFontMetricsF>
#include <QCoreApplication>
#include <QGraphicsSceneMouseEvent>
#include <QSlider>
#include <QListView>
#include <QSettings>
#include <QLabel>
#include <cmath>

#include <map>

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

    double ReferenceSize;

    class StarSystemGraphicsItem;
    class SectorGraphicsItem;

    class TextGraphicsItem
        : public QGraphicsItem
    {

    public:

        virtual ~TextGraphicsItem();

        TextGraphicsItem(QGraphicsItem * parent, const std::string & text);

        void setText(const std::string & text);

        QRectF boundingRect() const;

        virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    private:

        std::string mText;

        double mWidth;

        double mHeight;

        TextGraphicsItem(const TextGraphicsItem & other);
        
        TextGraphicsItem & operator =(const TextGraphicsItem & other);
        
    };

    class StarSystemTextGraphicsItem
        : public TextGraphicsItem
    {

    public:

        ~StarSystemTextGraphicsItem();

        StarSystemTextGraphicsItem(StarSystemGraphicsItem * parent);

        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    private:

        StarSystemTextGraphicsItem(const StarSystemTextGraphicsItem & other);
        
        StarSystemTextGraphicsItem & operator =(const StarSystemTextGraphicsItem & other);
        
    };

    class StarSystemGraphicsItem
        : public QGraphicsItem
        , private Game::StarSystem::Subscriber
    {

    public:

        ~StarSystemGraphicsItem();

        StarSystemGraphicsItem(QGraphicsItem * parent, Game::StarSystem * starSystem);

        QRectF boundingRect() const;

        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    private:

        void sectorAdded(Game::Sector * sector);

        void sectorRemoved(Game::Sector * sector);

        void becameKnown();

        friend class StarSystemTextGraphicsItem;

        std::map<Game::Sector *, SectorGraphicsItem *> mItems;

        StarSystemGraphicsItem(const StarSystemGraphicsItem & other);
        
        StarSystemGraphicsItem & operator =(const StarSystemGraphicsItem & other);
        
    };

    class SectorGraphicsItem
        : public QGraphicsItem
        , public Game::Sector::Subscriber
    {

    public:

        ~SectorGraphicsItem();

        SectorGraphicsItem(QGraphicsItem * parent, Game::Sector * sector);

        QRectF boundingRect() const;

        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    private:

        void contentsChanged(bool forcedRedraw);

        void selected();

        void deselected();

        bool mIsSelected;

        SectorGraphicsItem(const SectorGraphicsItem & other);
        
        SectorGraphicsItem & operator =(const SectorGraphicsItem & other);
        
    };

    TextGraphicsItem::~TextGraphicsItem()
    {

    }

    TextGraphicsItem::TextGraphicsItem(QGraphicsItem * parent, const std::string & text)
        : QGraphicsItem(parent)
        , mText(text)
        , mWidth(0.0)
        , mHeight(0.0)
    {
        QFontMetricsF metrics(scene() != NULL ? scene()->font() : QFont());
        mWidth = metrics.width(QString::fromStdString(mText.length() > QCoreApplication::tr("unexplored").toStdString().length() ? mText : QCoreApplication::tr("unexplored").toStdString()));
        mHeight = metrics.height();

        setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        
        if (Gui::MainWindow::Settings_CacheMode >= 2)
        {
            setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(boundingRect().size().width(),boundingRect().size().height()));
        }
    }

    void TextGraphicsItem::setText(const std::string & text)
    {
        if (text != mText)
        {
            mText = text;
            
            QFontMetricsF metrics(scene() != NULL ? scene()->font() : QFont());
            mWidth = metrics.width(QString::fromStdString(mText.length() > QCoreApplication::tr("unexplored").toStdString().length() ? mText : QCoreApplication::tr("unexplored").toStdString()));
            mHeight = metrics.height();

            if (Gui::MainWindow::Settings_CacheMode >= 2)
            {
                setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(boundingRect().size().width(),boundingRect().size().height()));
            }
        }
    }

    QRectF TextGraphicsItem::boundingRect() const
    {
        static const double Overlap = 1.1;
        return QRectF(-0.5*Overlap*mWidth,-0.5*Overlap*mHeight,1*Overlap*mWidth,1*Overlap*mHeight);
    }

    void TextGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
    {
        if (Gui::MainWindow::Settings_CacheMode < 2)
        {
            painter->setClipRect(option->exposedRect);
        }
        else
        {
            painter->setClipRect(boundingRect());
        }
        painter->drawText(boundingRect(), QString::fromStdString(mText));
    }

    StarSystemTextGraphicsItem::~StarSystemTextGraphicsItem()
    {

    }

    StarSystemTextGraphicsItem::StarSystemTextGraphicsItem(StarSystemGraphicsItem * parent)
        : TextGraphicsItem(parent, (parent != NULL && parent->starSystem() != NULL) ? parent->starSystem()->name() : "")
    {
    
    }

    void StarSystemTextGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
    {
        QPen previousPen = painter->pen();
        painter->setPen(Qt::lightGray);

        StarSystemGraphicsItem * starSystemGraphicsItem = static_cast<StarSystemGraphicsItem *>(parentItem());
        if (starSystemGraphicsItem != NULL && starSystemGraphicsItem->starSystem() != NULL)
        {
            std::set<Game::Player *> players = starSystemGraphicsItem->starSystem()->players();
            if (players.size() == 1)
            {
                std::vector<int> colorComponents = Game::Player::color(Game::Universe::instance().game().playerIndex(*players.begin()));
                QColor color(colorComponents[0], colorComponents[1], colorComponents[2]);
                painter->setPen(color);
            }
            else if (players.size() > 1)
            {
                painter->setPen(Qt::white);
            }

            if (!Gui::MainWindow::Settings_ViewUniverse && (Game::Universe::instance().game().currentPlayer() != NULL && !Game::Universe::instance().game().currentPlayer()->knows(starSystemGraphicsItem->starSystem())))
            {
                painter->setPen(Qt::lightGray);
                setText(QCoreApplication::tr("unexplored").toStdString());
            }
            else
            {
                setText(starSystemGraphicsItem->starSystem()->name());
            }
        }

        TextGraphicsItem::paint(painter, option, widget);
        painter->setPen(previousPen);
    }

    StarSystemGraphicsItem::~StarSystemGraphicsItem()
    {

    }

    StarSystemGraphicsItem::StarSystemGraphicsItem(QGraphicsItem * parent, Game::StarSystem * starSystem)
        : QGraphicsItem(parent)
        , Game::StarSystem::Subscriber(starSystem)
        , mItems()
    {
        if (starSystem != NULL)
        {
            StarSystemTextGraphicsItem * starSystemTextGraphicsItem = new StarSystemTextGraphicsItem(this);
            starSystemTextGraphicsItem->setZValue(1);
            starSystemTextGraphicsItem->setY(ReferenceSize/1.5);
        }        

        if (Gui::MainWindow::Settings_CacheMode >= 3)
        {
            setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(boundingRect().size().width(),boundingRect().size().height()));
        }
    }

    QRectF StarSystemGraphicsItem::boundingRect() const
    {
        return QRectF(-Game::StarSystem::Size/2.0*ReferenceSize,-Game::StarSystem::Size/2.0*ReferenceSize,Game::StarSystem::Size*ReferenceSize, Game::StarSystem::Size*ReferenceSize);
    }

    void StarSystemGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
    {
        if (starSystem() != NULL)
        {
            if (Gui::MainWindow::Settings_CacheMode < 2)//3
            {
                painter->setClipRect(option->exposedRect);
            }
            else
            {
                //painter->setClipRect(boundingRect());
            }

            if (Gui::MainWindow::Settings_ViewUniverse || Game::Universe::instance().game().currentPlayer() == NULL || Game::Universe::instance().game().currentPlayer()->knows(starSystem()))
            {
                std::set<Game::Player *> players = starSystem()->players();
                for (std::set<Game::Player *>::iterator it = players.begin(); it != players.end(); ++it)
                {
                    QPen previousPen = painter->pen();
                    std::vector<int> colorComponents = Game::Player::color(Game::Universe::instance().game().playerIndex(*it));
                    QColor color(colorComponents[0], colorComponents[1], colorComponents[2]);
                    if (Gui::MainWindow::Settings_DetailLevel > 1)
                    {
                        color.setAlphaF(0.1f);
                        painter->fillRect(boundingRect(), QBrush(color));
                    }
                    color.setAlphaF(0.35f);
                    painter->setPen(QPen(color, 9));
                    painter->drawRect(boundingRect());
                    painter->setPen(previousPen);
                }
            }
        }
    }

    void StarSystemGraphicsItem::sectorAdded(Game::Sector * sector)
    {
        if (sector != NULL)
        {
            SectorGraphicsItem * sectorGraphicsItem = new SectorGraphicsItem(this, sector);
            sectorGraphicsItem->setPos(sector->x()*ReferenceSize, sector->y()*ReferenceSize);
            mItems[sector] = sectorGraphicsItem;
        }
    }

    void StarSystemGraphicsItem::sectorRemoved(Game::Sector * sector)
    {
        if (sector != NULL && mItems[sector] != NULL)
        {
            mItems[sector]->setParentItem(NULL);
            delete mItems[sector];
            mItems.erase(sector);
        }
    }

    void StarSystemGraphicsItem::becameKnown()
    {
        if (Gui::MainWindow::Settings_CacheMode >= 3)
        {
            QList<QGraphicsItem *> children = childItems();
            for (int i = 0; i < children.size(); ++i)
            {
                children[i]->update();
            }
        }
    }

    SectorGraphicsItem::~SectorGraphicsItem()
    {

    }

    SectorGraphicsItem::SectorGraphicsItem(QGraphicsItem * parent, Game::Sector * sector)
        : QGraphicsItem(parent)
        , Game::Sector::Subscriber(sector)
        , mIsSelected(false)
    {
        if (Gui::MainWindow::Settings_SkipEmptyTiles)
        {
            setFlag(QGraphicsItem::ItemHasNoContents, sector->isEmpty() && !mIsSelected);
        }
        if (Gui::MainWindow::Settings_CacheMode >= 1)
        {
            setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(boundingRect().size().width(),boundingRect().size().height()));
        }
    }

    void SectorGraphicsItem::contentsChanged(bool forcedRedraw)
    {
        update();
        if (Gui::MainWindow::Settings_SkipEmptyTiles)
        {
            if (!scene()->views()[0]->property("NoForce").toBool() || Game::Universe::instance().game().currentPlayer()->isHuman())
            {
                setFlag(QGraphicsItem::ItemHasNoContents, false);
                QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
            }
            setFlag(QGraphicsItem::ItemHasNoContents, sector()->isEmpty() && !mIsSelected);
        }
        else if (forcedRedraw)
        {
            if (!scene()->views()[0]->property("NoForce").toBool() || Game::Universe::instance().game().currentPlayer()->isHuman())
            {
                QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
            }
        }
    }

    void SectorGraphicsItem::selected()
    {
        mIsSelected = true;

        if (Gui::MainWindow::Settings_SkipEmptyTiles)
        {
            setFlag(QGraphicsItem::ItemHasNoContents, false);
        }

        update();
    }

    void SectorGraphicsItem::deselected()
    {
        mIsSelected = false;

        update();

        if (Gui::MainWindow::Settings_SkipEmptyTiles)
        {
            if (!scene()->views()[0]->property("NoForce").toBool())
            {
                setFlag(QGraphicsItem::ItemHasNoContents, false);
                QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
            }
            setFlag(QGraphicsItem::ItemHasNoContents, sector()->isEmpty() && !mIsSelected);
        }
    }

    QRectF SectorGraphicsItem::boundingRect() const
    {
        return QRectF(-0.5*ReferenceSize,-0.5*ReferenceSize,1*ReferenceSize,1*ReferenceSize);
    }

    void SectorGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
    {
        if (Gui::MainWindow::Settings_CacheMode < 1)
        {
            painter->setClipRect(option->exposedRect);
        }
        else
        {
            painter->setClipRect(boundingRect());
        }
        Gui::UniversePainter().paintSector(painter, sector(), boundingRect().size(), mIsSelected, Gui::MainWindow::Settings_DetailLevel, 
            Gui::MainWindow::Settings_ViewUniverse || Game::Universe::instance().game().currentPlayer() == NULL || Game::Universe::instance().game().currentPlayer()->knows(sector()->starSystem()));
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
        , mZoomSlider(zoomSlider)
    {
        double referenceSize = ReferenceSize;
        setSceneRect(QRectF(-7*referenceSize*Game::StarSystem::Size, -7*referenceSize*Game::StarSystem::Size, 13*referenceSize*Game::StarSystem::Size, 13*referenceSize*Game::StarSystem::Size));

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
        double referenceSize = ReferenceSize;

        centerOn(mapToScene(event->pos()));
        QTransform viewTransform = transform();
        viewTransform.scale((32.0/referenceSize)/viewTransform.m11(), (32.0/referenceSize)/viewTransform.m22());
        setTransform(viewTransform);
        QGraphicsView::mouseDoubleClickEvent(event);

        QTransform scaleTransform = transform();
        int value = int(10.0*log(scaleTransform.m11())/log(2.0));
        bool blocked = mZoomSlider->blockSignals(true);
        mZoomSlider->setValue(value);
        mZoomSlider->blockSignals(blocked);
    }

    void ScalableGraphicsView::playerActivated(Game::Player * player)
    {
        if (Gui::MainWindow::Settings_CacheMode >= 1)
        {
            QList<QGraphicsItem *> sceneItems = items();
            for (int i = 0; i < sceneItems.count(); ++i)
            {
                SectorGraphicsItem * sectorItem = dynamic_cast<SectorGraphicsItem *>(sceneItems[i]);
                if (sectorItem != NULL)
                {
                    sectorItem->update();
                }
                else if (Gui::MainWindow::Settings_CacheMode >= 2)
                {
                    TextGraphicsItem * textItem = dynamic_cast<TextGraphicsItem *>(sceneItems[i]);
                    if (textItem != NULL)
                    {
                        textItem->update();
                    }
                }
            }
        }
        if (player != NULL)
        {
            QList<QGraphicsItem *> sceneItems = items();
            for (int i = 0; i < sceneItems.count(); ++i)
            {
                SectorGraphicsItem * sectorItem = dynamic_cast<SectorGraphicsItem *>(sceneItems[i]);
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

    class PrivateGraphicsScene
        : public QGraphicsScene
        , private Game::Universe::Subscriber
    {

    public:

        ~PrivateGraphicsScene()
        {
        }

        PrivateGraphicsScene(UniverseViewer * universeViewer)
            : QGraphicsScene(universeViewer)
            , Game::Universe::Subscriber()
            , mItems()
            , mW(0)
            , mH(0)
        {
            mW = QImage(QSize(16,16), QImage::Format_Mono).physicalDpiX() / 4;
            mH = QImage(QSize(16,16), QImage::Format_Mono).physicalDpiY() / 4;
            if (mW < 4 || mW > 1000 || mH < 4 || mH > 1000)
            {
                mW = 32;
                mH = 32;
            }
        }

    protected:

        void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
        {
            if ((mouseEvent->buttonDownScenePos(Qt::LeftButton) - mouseEvent->lastScenePos()).manhattanLength() < mW+mH)
            {
                QList<QGraphicsItem *> selectedItems = items(mouseEvent->lastScenePos());
                if (!selectedItems.empty())
                {
                    SectorGraphicsItem * sectorItem = dynamic_cast<SectorGraphicsItem *>(selectedItems.first());
                    if (sectorItem != NULL && Game::Universe::instance().game().currentPlayer() != NULL)
                    {
                        if (sectorItem->sector() != Game::Universe::instance().game().currentPlayer()->selectedSector().sector())
                        {
                            Game::Universe::instance().game().currentPlayer()->setSelectedSector(sectorItem->sector());
                        }
                    }
                }
            }
            QGraphicsScene::mouseReleaseEvent(mouseEvent);
        }

    private:

        void starSystemAdded(Game::StarSystem * starSystem)
        {
            if (starSystem != NULL)
            {
                double referenceSize = ReferenceSize;

                StarSystemGraphicsItem * starSystemGraphicsItem = new StarSystemGraphicsItem(NULL, starSystem);
                starSystemGraphicsItem->setPos(starSystem->x()*Game::StarSystem::Size*referenceSize, starSystem->y()*Game::StarSystem::Size*referenceSize);
                addItem(starSystemGraphicsItem);
                mItems[starSystem] = starSystemGraphicsItem;
            }
        }

        void starSystemRemoved(Game::StarSystem * starSystem)
        {
            if (starSystem != NULL)
            {
                removeItem(mItems[starSystem]);
                delete mItems[starSystem];
                mItems.erase(starSystem);
            }
        }

        std::map<Game::StarSystem *, StarSystemGraphicsItem *> mItems;

        int mW;

        int mH;
    };

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

    mGraphicsScene = new PrivateGraphicsScene(this);
    QGraphicsView * view = new ScalableGraphicsView(mGraphicsScene, this, mZoomSlider);
    setLayout(new QGridLayout());
    layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(view);

    double referenceSize = ReferenceSize;

    view->setTransform(QTransform::fromScale(32.0/referenceSize,32.0/referenceSize));

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

    double referenceSize = ReferenceSize;

    QTransform viewTransform = mGraphicsScene->views()[0]->transform();
    viewTransform.scale((32.0/referenceSize)/viewTransform.m11(), (32.0/referenceSize)/viewTransform.m22());
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

    //QSize messageBoxSize(event->size().width() / 2.0, event->size().height() / 8.0);
    //mMessageBox->resize(messageBoxSize);
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