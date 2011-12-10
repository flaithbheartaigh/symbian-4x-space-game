#ifndef SHIPCONFIGDESIGN_H
#define SHIPCONFIGDESIGN_H

#include "ShipConfigModel.h"

#include <QFrame>

#include <vector>

class QTableView;

namespace Gui
{
    class ShipConfigDesign
        : public QFrame
    {
        Q_OBJECT

    public:

        ~ShipConfigDesign();

        ShipConfigDesign(QWidget * parent);

        const std::vector<ShipConfigModel::Row> & shipConfigs() const;

        void loadDesigns();

    private:

        QTableView * mEditView;

        std::vector<ShipConfigModel::Row> mShipConfigs;

        ShipConfigDesign();

        ShipConfigDesign(const ShipConfigDesign & other);

        ShipConfigDesign & operator =(const ShipConfigDesign & other);

    };
}

#endif