#ifndef SUBSCRIBABLEPUSHBUTTON_H
#define SUBSCRIBABLEPUSHBUTTON_H

#include <QPushButton>

#include <set>

namespace Gui
{
    class SubscribablePushButton
        : public QWidget
    {

    Q_OBJECT

    public:

        class Subscriber
        {
       
        public:

            virtual ~Subscriber();

            Subscriber(SubscribablePushButton * pushButton);

            virtual void clicked(bool checked);

        private:

            void unsubscribe();

            friend class SubscribablePushButton;

            SubscribablePushButton * mSubscribablePushButton;

            Subscriber();

            Subscriber(const Subscriber & other);

            Subscriber & operator =(const Subscriber & other);

        };

        ~SubscribablePushButton();

        SubscribablePushButton(QWidget * parent, const QString & text = "");
        
        void setCheckable(bool checkable);

        bool isCheckable() const;

        bool isChecked() const;

        void click();

        void subscribe(Subscriber * mSubscriber);

        void unsubscribe(Subscriber * mSubscriber);

    private:

        QWidget * mWidget;

        std::set<Subscriber *> mSubscribers;

        SubscribablePushButton(const SubscribablePushButton & other);
        
        SubscribablePushButton & operator =(const SubscribablePushButton & other);
        
    };
}

#endif
