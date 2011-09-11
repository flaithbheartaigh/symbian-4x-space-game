#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include <QAbstractTableModel>

#include <vector>

namespace Game
{
    class Component;
}

namespace Gui
{
    class ComponentModel
        : public QAbstractTableModel
    {
        Q_OBJECT

    public:

        ~ComponentModel();

        ComponentModel(QObject * parent, std::vector<Game::Component> * components);

        Qt::DropActions supportedDropActions() const;

        Qt::ItemFlags flags(const QModelIndex & index) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        int columnCount(const QModelIndex & parent = QModelIndex()) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;

        QVariant data(const QModelIndex & index, int role) const;

        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

        bool insertRows(int position, int rows, const QModelIndex & index = QModelIndex());

        bool removeRows(int position, int rows, const QModelIndex & index = QModelIndex());

        QStringList mimeTypes() const;

        QMimeData * mimeData(const QModelIndexList &indexes) const;

        bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);

    private:

        std::vector<Game::Component> * mComponents;

        ComponentModel(const ComponentModel & other);
        
        ComponentModel & operator =(const ComponentModel & other);
        
    };
}

#endif