#ifndef PLANETLISTMODEL_H
#define PLANETLISTMODEL_H

#include <QAbstractTableModel>

#include <vector>

namespace Game
{
    class Planet;
}

namespace Gui
{
    class PlanetListModel
        : public QAbstractTableModel
    {
        Q_OBJECT

    public:

        ~PlanetListModel();

        PlanetListModel(QObject * parent);

        void setPlanets(const std::vector<Game::Planet *> & planets);

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        int columnCount(const QModelIndex & parent = QModelIndex()) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;

        QVariant data(const QModelIndex & index, int role) const;

    private:

        std::vector<Game::Planet *> mPlanets;

        PlanetListModel(const PlanetListModel & other);
        
        PlanetListModel & operator =(const PlanetListModel & other);
        
    };
}

#endif