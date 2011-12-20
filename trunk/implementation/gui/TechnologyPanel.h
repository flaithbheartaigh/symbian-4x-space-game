#ifndef TECHNOLOGYPANEL_H
#define TECHNOLOGYPANEL_H

#include <QFrame>

#include <vector>

class QTableView;

namespace Game
{
    class Component;
}

namespace Gui
{
    class TechnologyPanel
        : public QFrame
    {
        Q_OBJECT

    public:

        ~TechnologyPanel();

        TechnologyPanel(QWidget * parent);

        const std::vector<Game::Component> & selectedComponents() const;

        QTableView * selectedComponentsView();

        QTableView * allComponentsView();

        void loadComponents();

    private:

        std::vector<Game::Component> mSelectedComponents;

        std::vector<Game::Component> mAvailableComponents;

        QTableView * mEditView;

        QTableView * mListView;

        TechnologyPanel();

        TechnologyPanel(const TechnologyPanel & other);

        TechnologyPanel & operator =(const TechnologyPanel & other);

    };
}

#endif