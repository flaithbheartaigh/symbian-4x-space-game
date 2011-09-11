#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

namespace Gui
{
    class TableView
        : public QTableView
    {
        Q_OBJECT

    public:

        virtual ~TableView();

        TableView(QWidget * parent);

        void setModel(QAbstractItemModel * newModel);

        void setOtherTableView(TableView * tableView);

    protected:

        void mousePressEvent(QMouseEvent * event);

        void mouseMoveEvent(QMouseEvent * event);

        void mouseReleaseEvent(QMouseEvent * event);

    private slots:

        void slot_dataChanged(const QModelIndex & from, const QModelIndex & to);

    private:

        TableView * mOtherTableView;

        TableView();

        TableView(const TableView & other);

        TableView & operator =(const TableView & other);

    };
}

#endif