#ifndef SECTORLISTVIEW_H
#define SECTORLISTVIEW_H

#include <QListView>

namespace Gui
{
    class SectorListView
        : public QListView
    {
        Q_OBJECT

    public:

        virtual ~SectorListView();

        SectorListView(QWidget * parent);

        void setModel(QAbstractItemModel * newModel);

    protected:

        void mousePressEvent(QMouseEvent * event);

        void mouseMoveEvent(QMouseEvent * event);

        void mouseReleaseEvent(QMouseEvent * event);

    private slots:

        void slot_layoutChanged();

    private:

        SectorListView();

        SectorListView(const SectorListView & other);

        SectorListView & operator =(const SectorListView & other);

    };
}

#endif