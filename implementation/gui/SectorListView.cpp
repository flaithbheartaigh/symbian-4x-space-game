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

#include "SectorListView.h"
#include "SectorItemModel.h"
#include "QsKineticScroller.h"

#include <game/Player.h>
#include <game/Sector.h>

using namespace Gui;

SectorListView::~SectorListView()
{

}

SectorListView::SectorListView(QWidget * parent)
    : QListView(parent)
{
    class PrivateSectorViewSectorSubscriber
        : public QObject
        , private Game::Player::Current::Subscriber
    {

    public:

        ~PrivateSectorViewSectorSubscriber()
        {
            delete mSectorSubscriber;
            mSectorSubscriber = NULL;
        }

        PrivateSectorViewSectorSubscriber(QListView * listView, Gui::SectorItemModel * itemModel)
            : QObject(itemModel)
            , Game::Player::Current::Subscriber()
            , mListView(listView)
            , mItemModel(itemModel)
            , mSectorSubscriber(NULL)
        {
        }

    private:

        void selectedSectorChanged(Game::Sector * sector)
        {
            if (mSectorSubscriber == NULL || mSectorSubscriber->sector() != sector)
            {
                class PrivateSectorSubscriber
                    : public Game::Sector::Subscriber
                {

                public:

                    ~PrivateSectorSubscriber()
                    {
                    }

                    PrivateSectorSubscriber(Gui::SectorItemModel * itemModel, Game::Sector * sector)
                        : Game::Sector::Subscriber(sector)
                        , mItemModel(itemModel)
                    {
                    }

                    void contentsChanged(bool)
                    {
                        if (mItemModel != NULL)
                        {
                            mItemModel->setSector(sector());
                        }
                    }

                private:

                    Gui::SectorItemModel * mItemModel;
                };

                delete mSectorSubscriber;
                mSectorSubscriber = new PrivateSectorSubscriber(mItemModel, sector);

                if (mItemModel != NULL)
                {
                    mItemModel->setSector(sector);
                    if (mListView != NULL && !mListView->currentIndex().isValid())
                    {
                        mListView->setCurrentIndex(mItemModel->index(0,0));
                    }
                }
            }
        }

        Gui::SectorItemModel * mItemModel;

        QListView * mListView;

        Game::Sector::Subscriber * mSectorSubscriber;

    };

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setSelectionMode(QAbstractItemView::MultiSelection);
    QsKineticScroller * kineticScroller2 = new QsKineticScroller(this);
    kineticScroller2->enableKineticScrollFor(this);
    SectorItemModel * sectorItemModel = new SectorItemModel(this);
    setModel(sectorItemModel);
    new PrivateSectorViewSectorSubscriber(this, sectorItemModel);    
}

void SectorListView::setModel(QAbstractItemModel * newModel)
{
    if (newModel == model())
        return;
    //let's disconnect from the old model
    if (model() != NULL) 
    {
        QObject::disconnect(model(), SIGNAL(layoutChanged()), this, SLOT(slot_layoutChanged()));
    }
    if (newModel != NULL) 
    {
        QObject::connect(newModel, SIGNAL(layoutChanged()), this, SLOT(slot_layoutChanged()));
    }
    QListView::setModel(newModel);
}

void SectorListView::mousePressEvent(QMouseEvent * event)
{
    QListView::mousePressEvent(event);
}

void SectorListView::mouseMoveEvent(QMouseEvent * event)
{
    QListView::mouseMoveEvent(event);
}

void SectorListView::mouseReleaseEvent(QMouseEvent * event)
{
    QListView::mouseReleaseEvent(event);
}

void SectorListView::slot_layoutChanged()
{
	int height = 0;
	const Gui::SectorItemModel * itemModel = static_cast<const Gui::SectorItemModel *>(model());
	if (itemModel != NULL)
	{
		height = sizeHintForRow(1) * itemModel->rowCount();
	}
    if (height > static_cast<QWidget *>(parent())->height())
    {
        height = static_cast<QWidget *>(parent())->height();
    }
    resize(0, height);
}