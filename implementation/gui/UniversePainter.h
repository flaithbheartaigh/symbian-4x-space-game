#ifndef UNIVERSEPAINTER_H
#define UNIVERSEPAINTER_H

#include <QSizeF>

class QPainter;

namespace Game
{
    class Sector;
    class Planet;
    class Star;
    class Ship;
    class Player;
    class Component;
}

namespace Gui
{
    class UniversePainter
    {

    public:

        ~UniversePainter();

        UniversePainter();

        void paintShip(QPainter * painter, Game::Ship * ship, const QSizeF & size, unsigned int index);

        void paintStar(QPainter * painter, Game::Star * star, const QSizeF & size, int detailLevel = 2);

        void paintPlanet(QPainter * painter, Game::Planet * planet, const QSizeF & size, int detailLevel = 2);

        void paintSector(QPainter * painter, Game::Sector * sector, const QSizeF & size, bool selected, int detailLevel = 2, bool known = true);

        void paintBadge(QPainter * painter, Game::Player * player, const QSizeF & size, bool translucent);

        void paintComponent(QPainter * painter, Game::Component * component, const QSizeF & size);

    private:

        UniversePainter(const UniversePainter & other);
        
        UniversePainter & operator =(const UniversePainter & other);
        
    };
}

#endif
