#ifndef SHIPCONFIGMODEL_H
#define SHIPCONFIGMODEL_H

#include <game/ShipConfig.h>

#include <QAbstractTableModel>

#include <vector>

namespace Game
{
    class ShipConfig;
}

namespace Gui
{
    class ShipConfigModel
        : public QAbstractTableModel
    {
        Q_OBJECT

    public:

        struct Row
        {
            Row()
                : config()
                , count(0)
            {
            }
            Game::ShipConfig config;
            unsigned int count;
        };

        ~ShipConfigModel();

        ShipConfigModel(QObject * parent, std::vector<Row> * shipConfigs, bool buy = false);

        Qt::DropActions supportedDropActions() const;

        Qt::ItemFlags flags(const QModelIndex & index) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        int columnCount(const QModelIndex & parent = QModelIndex()) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;

        QVariant data(const QModelIndex & index, int role) const;

        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

        bool insertRows(int position, int rows, const QModelIndex & index = QModelIndex());

        bool removeRows(int position, int rows, const QModelIndex & index = QModelIndex());

        QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

    private:

        std::vector<Row> * mShipConfigs;

        bool mBuy;

        ShipConfigModel(const ShipConfigModel & other);
        
        ShipConfigModel & operator =(const ShipConfigModel & other);
        
    };
}

#endif