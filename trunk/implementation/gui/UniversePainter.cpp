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

#include "UniversePainter.h"

#include <game/Universe.h>
#include <game/StarSystem.h>
#include <game/Sector.h>
#include <game/Star.h>
#include <game/Planet.h>
#include <game/Ship.h>
#include <game/Player.h>
#include <game/Component.h>
#include <Game/Technology.h>
#include <game/Resources.h>

#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QRadialGradient>
#include <QSvgRenderer>

#include <cmath>

using namespace Gui;

UniversePainter::~UniversePainter()
{

}

UniversePainter::UniversePainter()
{

}

void UniversePainter::paintShip(QPainter * painter, Game::Ship * ship, const QSizeF & size, unsigned int index)
{
    if (painter != NULL && ship != NULL)
    {
        QColor color(Qt::gray);
        if (ship->player() != NULL)
        {
            std::vector<int> colorComponents = Game::Player::color(Game::Universe::instance().game().playerIndex(ship->player()));
            color = QColor(colorComponents[0], colorComponents[1], colorComponents[2]);
        }
        unsigned int column = index / 4;
        unsigned int row = index % 4;
        QSizeF sizeRect(size.width() / 4.0f, size.height() / 4.0f);
        QPointF posRect(-size.width() / 2.0f + column * size.width() / 4.0f + sizeRect.width() / 2.0f, -size.height() / 2.0f + row * size.height() / 4.0f + sizeRect.height() / 2.0f);

        painter->translate(posRect);
        paintBadge(painter, ship->player(), sizeRect, false);
        painter->translate(-posRect);
    }
}

void UniversePainter::paintStar(QPainter * painter, Game::Star * star, const QSizeF & size, int detailLevel)
{
    if (painter != NULL && star != NULL)
    {
        float radius = size.height() < size.width() ? size.height() / 2.0f : size.width() / 2.0f;
        QPointF center(0.0f, 0.0f);
        QBrush brush;

        if (detailLevel > 0)
        {
            QPointF focal(0.0f, 0.0f);
            QRadialGradient grad(center, radius, focal);
            grad.setColorAt(0, Qt::white);
            grad.setColorAt(0.94f, QColor(224, 212, 200));
            grad.setColorAt(0.98f, Qt::darkRed);
            grad.setColorAt(1, Qt::black);
            brush = grad;
        }
        else
        {
            brush = QBrush(Qt::white);
        }

        QBrush previousBrush = painter->brush();
        painter->setBrush(brush);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(center, radius, radius);
        painter->setBrush(previousBrush);
    }
}

void UniversePainter::paintPlanet(QPainter * painter, Game::Planet * planet, const QSizeF & size, int detailLevel)
{
    static const QList<QColor> PLANETCOLORS = QList<QColor>() << Qt::darkGray << Qt::darkCyan << Qt::darkMagenta << Qt::darkGreen << Qt::darkRed;
    static const QList<QColor> ATMOSPHERECOLORS = QList<QColor>() << QColor(0,0,0,0) << QColor(95,160,160,95) << QColor(160,95,160,95) << QColor(95,160,95,95) << QColor(160,95,95,95);

    if (painter != NULL && planet != NULL)
    {
        QSizeF planetSize(size.width() * 0.9, size.height() * 0.9);
        if (planet->size() == Game::Planet::Medium)
        {
            planetSize = QSizeF(planetSize.width() * 0.75, planetSize.height() * 0.75);
        }
        else if (planet->size() == Game::Planet::Small)
        {
            planetSize = QSizeF(planetSize.width() * 0.5, planetSize.height() * 0.5);
        }

        float radius = planetSize.height() < planetSize.width() ? planetSize.height() / 2.0f : planetSize.width() / 2.0f;
        QPointF center(0.0f, 0.0f);
        QBrush brush;

        if (detailLevel > 0)
        {
            QPointF focal(radius*1.0f/3.0f, radius*1.0f/3.0f);
            QRadialGradient grad(center, radius*1.33, focal);
            grad.setColorAt(0, PLANETCOLORS[int(planet->atmosphere())]);
            grad.setColorAt(1, Qt::black);
            brush = grad;
        }
        else
        {
            QColor color = PLANETCOLORS[int(planet->atmosphere())];
            color = color.darker();
            brush = QBrush(color);
        }

        QBrush previousBrush = painter->brush();
        painter->setBrush(brush);
        painter->setPen(QPen(ATMOSPHERECOLORS[int(planet->atmosphere())], 1.5));
        painter->drawEllipse(center, radius, radius);
        painter->setBrush(previousBrush);

        if (planet->player() != NULL)
        {
            std::vector<int> colorComponents = Game::Player::color(Game::Universe::instance().game().playerIndex(planet->player()));
            unsigned int column = 1;
            unsigned int row = 1;
            QPointF posRect(size.width() / 2.0f - column * size.width() / 8.0f, size.height() / 2.0f - row * size.height() / 18.0f);
            QSizeF sizeRect(size.width() / 8.0f, size.height() / 18.0f);
            painter->fillRect(QRectF(posRect, sizeRect), QBrush(QColor(colorComponents[0],colorComponents[1],colorComponents[2])));
        }
    }
}

void UniversePainter::paintSector(QPainter * painter, Game::Sector * sector, const QSizeF & size, bool selected, int detailLevel, bool known)
{
    if (painter != NULL && sector != NULL)
    {
        if (sector->star() != NULL)
        {
            paintStar(painter, sector->star(), size, detailLevel);
        }
        //bool known = Game::Universe::instance().game().currentPlayer() == NULL || Game::Universe::instance().game().currentPlayer()->knows(sector->starSystem());
        if (!sector->planets().empty() && known)
        {
            paintPlanet(painter, sector->planets()[0], size, detailLevel);
        }
        if (!sector->ships().empty() || !sector->shipsInTransit().empty())
        {
            std::map<Game::Player *, Game::Ship *> oneShipPerPlayer;
            for (std::vector<Game::Ship *>::const_iterator it = sector->shipsInTransit().begin(); it != sector->shipsInTransit().end(); ++it)
            {
                oneShipPerPlayer[(*it)->player()] = *it;
            }
            for (std::vector<Game::Ship *>::const_iterator it = sector->ships().begin(); it != sector->ships().end(); ++it)
            {
                oneShipPerPlayer[(*it)->player()] = *it;
            }
            if (known)
            {
                if (oneShipPerPlayer.size() == 1 && sector->star() == NULL && sector->planets().empty())
                {
                    paintBadge(painter, oneShipPerPlayer.begin()->first, size, oneShipPerPlayer.begin()->second->isInTransit());
                }
                else
                {
                    int index = 0;
                    for (std::map<Game::Player *, Game::Ship *>::const_iterator it = oneShipPerPlayer.begin(); it != oneShipPerPlayer.end(); ++it)
                    {
                        paintShip(painter, (*it).second, size, index);
                        ++index;
                    }
                }
            }
            else
            {
                for (std::map<Game::Player *, Game::Ship *>::const_iterator it = oneShipPerPlayer.begin(); it != oneShipPerPlayer.end(); ++it)
                {
                    if ((*it).first == Game::Universe::instance().game().currentPlayer())
                    {
                        paintBadge(painter, (*it).first, size, (*it).second->isInTransit());
                        break;
                    }
                }
            }
        }

        if (selected)
        {
            QPen previousPen = painter->pen();
            QPen pen;
            pen.setStyle(Qt::DashDotDotLine);
            pen.setWidth(2);
            pen.setColor(QColor(0,255,255,63));
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            painter->setPen(pen);
            painter->drawRect(QRectF(-QPointF(size.width()/2.0f-1, size.height() / 2.0f-1), QSizeF(size.width()-2, size.height()-2)));
            painter->setPen(previousPen);
        }
    }
}

void UniversePainter::paintBadge(QPainter * painter, Game::Player * player, const QSizeF & size, bool translucent)
{
    if (painter != NULL)
    {
        int index = -1;

        QSizeF badgeSize(size.width(), size.height());
        if (player != NULL)
        {
            index = Game::Universe::instance().game().playerIndex(player);

            QString filename = QString::fromStdString(Game::Resources::instance().getDataFilePath("images/player%1.svg")).arg(index);

            QSvgRenderer svgRenderer(filename);
            svgRenderer.render(painter, QRectF(-QPointF(badgeSize.width()/2.0f, badgeSize.height() / 2.0f), badgeSize));
            if (translucent)
            {
                painter->setPen(QColor(Qt::white));
                painter->drawText(QRectF(-QPointF(badgeSize.width()/2.0f, badgeSize.height() / 2.0f), badgeSize), "T");
            }
        }
    }
}

void UniversePainter::paintComponent(QPainter * painter, Game::Component * component, const QSizeF & size)
{
    if (painter != NULL)
    {
        QSizeF componentSize(size.width(), size.height());

        if (component->type() == Game::Component::Engine)
        {
            QString filename = QString::fromStdString(Game::Resources::instance().getDataFilePath("images/engine.svg"));
            QSvgRenderer svgRenderer(filename);
            svgRenderer.render(painter, QRectF(-QPointF(componentSize.width()/2.0f, componentSize.height() / 2.0f), componentSize));
        }
        if (component->type() == Game::Component::Colony)
        {
            QString filename = QString::fromStdString(Game::Resources::instance().getDataFilePath("images/colony.svg"));
            QSvgRenderer svgRenderer(filename);
            svgRenderer.render(painter, QRectF(-QPointF(componentSize.width()/2.0f, componentSize.height() / 2.0f), componentSize));
        }
        if (component->type() == Game::Component::StarDrive)
        {
            QString filename = QString::fromStdString(Game::Resources::instance().getDataFilePath("images/stardrive.svg"));
            QSvgRenderer svgRenderer(filename);
            svgRenderer.render(painter, QRectF(-QPointF(componentSize.width()/2.0f, componentSize.height() / 2.0f), componentSize));
        }
        if (component->type() == Game::Component::Weapon)
        {
            QString filename = QString::fromStdString(Game::Resources::instance().getDataFilePath("images/laserbeam.svg"));
            QSvgRenderer svgRenderer(filename);
            svgRenderer.render(painter, QRectF(-QPointF(componentSize.width()/2.0f, componentSize.height() / 2.0f), componentSize));
        }
    }
}