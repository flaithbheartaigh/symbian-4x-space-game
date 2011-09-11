#ifndef SHIPCONFIGDESIGN_H
#define SHIPCONFIGDESIGN_H

#include <QFrame>

#include <vector>

class QTableView;

namespace Game
{
    class ShipConfig;
}

namespace Gui
{
    class ShipConfigDesign
        : public QFrame
    {
        Q_OBJECT

    public:

        ~ShipConfigDesign();

        ShipConfigDesign(QWidget * parent);

        const std::vector<Game::ShipConfig> & shipConfigs() const;

        void loadDesigns();

    private:

        QTableView * mEditView;

        std::vector<Game::ShipConfig> mShipConfigs;

        ShipConfigDesign();

        ShipConfigDesign(const ShipConfigDesign & other);

        ShipConfigDesign & operator =(const ShipConfigDesign & other);

    };
}

#endif