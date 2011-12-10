#ifndef SHIPCONFIGBUILD_H
#define SHIPCONFIGBUILD_H

#include "ShipConfigModel.h"

#include <QFrame>

#include <vector>

class QTableView;
class QItemSelectionModel;

namespace Game
{
    class Sector;
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

        const std::vector<ShipConfigModel::Row> & shipConfigs() const;

        std::vector<ShipConfigModel::Row> & shipConfigs();

        void loadDesigns();

        QItemSelectionModel * selectionModel() const;

    private:

        QTableView * mEditView;

        std::vector<ShipConfigModel::Row> mShipConfigs;

        QItemSelectionModel * mSelectionModel;

        Game::Sector * mSector;

        ShipConfigBuild();

        ShipConfigBuild(const ShipConfigBuild & other);

        ShipConfigBuild & operator =(const ShipConfigBuild & other);

    };
}

#endif