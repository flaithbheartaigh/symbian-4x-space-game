#ifndef SHIPCONFIGBUILD_H
#define SHIPCONFIGBUILD_H

#include <QFrame>

#include <vector>

class QTableView;
class QItemSelectionModel;

namespace Game
{
    class Sector;
    class ShipConfig;
}

namespace Gui
{
    class ShipConfigBuild
        : public QFrame
    {
        Q_OBJECT

    public:

        ~ShipConfigBuild();

        ShipConfigBuild(QWidget * parent);

        Game::Sector * sector() const;

        void setSector(Game::Sector * sector);

        const std::vector<Game::ShipConfig> & shipConfigs() const;

        void loadDesigns();

        QItemSelectionModel * selectionModel() const;

    private:

        QTableView * mEditView;

        std::vector<Game::ShipConfig> mShipConfigs;

        QItemSelectionModel * mSelectionModel;

        Game::Sector * mSector;

        ShipConfigBuild();

        ShipConfigBuild(const ShipConfigBuild & other);

        ShipConfigBuild & operator =(const ShipConfigBuild & other);

    };
}

#endif