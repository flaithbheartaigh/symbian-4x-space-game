#ifndef CSSHELPER_H
#define CSSHELPER_H

#include <string>
#include <vector>

class QWidget;

namespace Gui
{
    class CSSHelper 
    {

    public:

        /// Destructor
        ///
        ~CSSHelper();

        /// Constructor
        ///
        CSSHelper();
        
        void addStyle(const std::string & styleSheet);

        void apply(QWidget & parent);

    private:

        std::vector<std::string> mCSSList;
       
        /// Copy constructor should not be used
        /// 
        CSSHelper(const CSSHelper & other);
        
        /// Assignment operator should not be used
        /// 
        CSSHelper & operator= (const CSSHelper & rhs);
    };
}

#endif // CSSHELPER_H