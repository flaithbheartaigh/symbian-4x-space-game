#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H
/*
#include "ShipConfigModel.h"
*/
#include <QFrame>

#include <vector>

class QTableView;

namespace Gui
{
    class TechnologyPanel
        : public QFrame
    {
        Q_OBJECT

    public:

        ~TechnologyPanel();

        TechnologyPanel(QWidget * parent);
/*
        const std::vector<ShipConfigModel::Row> & shipConfigs() const;

        void loadDesigns();
*/
    private:

        QTableView * mEditView;
/*
        std::vector<ShipConfigModel::Row> mShipConfigs;
*/
        TechnologyPanel();

        TechnologyPanel(const TechnologyPanel & other);

        TechnologyPanel & operator =(const TechnologyPanel & other);

    };
}

#endif