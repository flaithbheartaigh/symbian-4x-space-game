#ifndef SECTORITEMMODEL_H
#define SECTORITEMMODEL_H

#include <QAbstractTableModel>

#include <vector>

namespace Game
{
    class Sector;
    class Planet;
    class Ship;
}

namespace Gui
{
    class SectorItemModel
        : public QAbstractTableModel
    {
        Q_OBJECT

    public:

        ~SectorItemModel();

        SectorItemModel(QObject * parent, Game::Sector * sector = NULL);

        void setSector(Game::Sector * sector);

        Qt::DropActions supportedDropActions() const;

        Qt::ItemFlags flags(const QModelIndex & index) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        int columnCount(const QModelIndex & parent = QModelIndex()) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;

        QVariant data(const QModelIndex & index, int role) const;

        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

        bool insertRows(int position, int rows, const QModelIndex & index = QModelIndex());

        bool removeRows(int position, int rows, const QModelIndex & index = QModelIndex());

        std::vector<Game::Planet *> planets(const QModelIndexList & indexList) const;

        std::vector<Game::Ship *> ships(const QModelIndexList & indexList) const;

    private:

        class Item;

        std::vector<Item> mItems;

        SectorItemModel(const SectorItemModel & other);
        
        SectorItemModel & operator =(const SectorItemModel & other);
        
    };
}

#endif