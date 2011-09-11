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

#include "SubscribablePushButton.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>

namespace
{
    const char * PROPERTY_CHECKED = "checked";
    const char * PROPERTY_CHECKABLE = "checkable";
    const int MaxIgnoredMouseMoves = 0;
}

using namespace Gui;

SubscribablePushButton::Subscriber::~Subscriber()
{
    unsubscribe();
}

SubscribablePushButton::Subscriber::Subscriber(SubscribablePushButton * pushButton)
    : mSubscribablePushButton(pushButton)
{
    if (mSubscribablePushButton != NULL)
    {
        mSubscribablePushButton->subscribe(this);
    }
}

void SubscribablePushButton::Subscriber::clicked(bool checked)
{

}

void SubscribablePushButton::Subscriber::unsubscribe()
{
    if (mSubscribablePushButton != NULL)
    {
        mSubscribablePushButton->unsubscribe(this);
        mSubscribablePushButton = NULL;
    }
}

SubscribablePushButton::~SubscribablePushButton()
{
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->unsubscribe();
    }
}

SubscribablePushButton::SubscribablePushButton(QWidget * parent, const QString & text)
    : QWidget(parent)
    , mWidget(NULL)
    , mSubscribers()
{
    class PrivateLabel
        : public QLabel
    {

    public:

        ~PrivateLabel()
        {

        }

        PrivateLabel(const QString & text, SubscribablePushButton * pushButton)
            : QLabel(text, pushButton)
            , mMovedBy(0)
            , mPressed(false)
            , mBounds()
        {

        }

    protected:

        void mousePressEvent(QMouseEvent * event)
        {
            mPressed = true;
            mMovedBy = 0;
            mBounds = QRect(mapToGlobal(QPoint(0,0)), size());
            QLabel::mousePressEvent(event);
        }

        void mouseMoveEvent(QMouseEvent * event)
        {
            if (!mPressed)
            {
                mPressed = true;
                mMovedBy = 0;
                mBounds = QRect(mapToGlobal(QPoint(0,0)), size());
            }
            else if (!mBounds.contains(event->globalPos()))
            {
                mMovedBy += 1;
            }
            QLabel::mouseMoveEvent(event);
        }

        void mouseReleaseEvent(QMouseEvent * event)
        {
            mPressed = false;
            if (mMovedBy <= MaxIgnoredMouseMoves)
            {
                static_cast<SubscribablePushButton *>(parent())->click();
            }
            mMovedBy = 0;
            QLabel::mouseReleaseEvent(event);
        }

    private:

        int mMovedBy;
        bool mPressed;
        QRect mBounds;
    };

    mWidget = new PrivateLabel(text, this);

    setLayout(new QVBoxLayout());
    layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(mWidget);

    setProperty(PROPERTY_CHECKED, false);
    setProperty(PROPERTY_CHECKABLE, false);
    setStyleSheet(styleSheet());
}

void SubscribablePushButton::setCheckable(bool checkable)
{
    setProperty(PROPERTY_CHECKABLE, checkable);
    setStyleSheet(styleSheet());
}

bool SubscribablePushButton::isCheckable() const
{
    return property(PROPERTY_CHECKABLE).toBool();
}

bool SubscribablePushButton::isChecked() const
{
    return property(PROPERTY_CHECKED).toBool();
}

void SubscribablePushButton::click()
{
    if (isCheckable())
    {
        setProperty(PROPERTY_CHECKED, !isChecked());
        bool a = isChecked();
        setStyleSheet(styleSheet());
    }
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->clicked(isChecked());
    }
}

void SubscribablePushButton::subscribe(Subscriber * mSubscriber)
{
    mSubscribers.insert(mSubscriber);
}

void SubscribablePushButton::unsubscribe(Subscriber * mSubscriber)
{
    mSubscribers.erase(mSubscriber);
}
