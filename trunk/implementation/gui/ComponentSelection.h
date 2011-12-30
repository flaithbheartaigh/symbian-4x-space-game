#ifndef COMPONENTSELECTION_H
#define COMPONENTSELECTION_H

#include <QFrame>

#include <vector>

class QTableView;

namespace Game
{
    class ShipConfig;
    class Component;
}

namespace Gui
{
    class ComponentSelection
        : public QFrame
    {
        Q_OBJECT

    public:

        ~ComponentSelection();

        ComponentSelection(QWidget * parent);

        const std::vector<Game::Component> & selectedComponents() const;

        QTableView * selectedComponentsView();

        QTableView * allComponentsView();

        Game::ShipConfig * shipConfig() const;

        void setShipConfig(Game::ShipConfig * shipConfig);

        void loadComponents();

    private:

        std::vector<Game::Component> mSelectedComponents;

        std::vector<Game::Component> mAvailableComponents;

        QTableView * mEditView;

        QTableView * mListView;

        Game::ShipConfig * mShipConfig;

        ComponentSelection();

        ComponentSelection(const ComponentSelection & other);

        ComponentSelection & operator =(const ComponentSelection & other);

    };
}

#endif