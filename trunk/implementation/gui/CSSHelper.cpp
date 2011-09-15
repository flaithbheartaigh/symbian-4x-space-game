// Symbian 4X Space Game
// 
// Copyright (C) 2011 Patrick Pelletier
// 'CSSHelper' source file exception: license undefined or Public Domain

#include "CSSHelper.h"
#include <QWidget>
#include <sstream>
#include <string>
#include <QTextStream>
#include <QFile>

#include <game/Parameters.h>

using namespace Gui;

namespace
{
    static const char * CSSPATH = "stylesheets/";
}

CSSHelper::~CSSHelper()
{
}

CSSHelper::CSSHelper()
    : mCSSList()
{

}

void CSSHelper::addStyle(const std::string & filename)
{
    std::ostringstream ss;

    ss << Game::Parameters::instance().getDataFilePath(std::string(CSSPATH)) << filename;
    
    mCSSList.push_back(ss.str());
}

void CSSHelper::apply(QWidget & widget)
{
    std::ostringstream ss;

    for (std::vector<std::string>::iterator it = mCSSList.begin(); it != mCSSList.end(); ++it)
    {
        QFile styleSheetFile(QString::fromStdString(*it));

        if (styleSheetFile.open(QIODevice::ReadOnly))
        {
            QTextStream styleSheetStream(&styleSheetFile);
            ss << styleSheetStream.readAll().toStdString();
        }
    }

    if (ss.str() != "")
    {
        widget.setStyleSheet(QString::fromStdString(ss.str()));
    }
}

std::string CSSHelper::style() const
{
    std::ostringstream ss;

    for (std::vector<std::string>::const_iterator it = mCSSList.begin(); it != mCSSList.end(); ++it)
    {
        QFile styleSheetFile(QString::fromStdString(*it));

        if (styleSheetFile.open(QIODevice::ReadOnly))
        {
            QTextStream styleSheetStream(&styleSheetFile);
            ss << styleSheetStream.readAll().toStdString();
        }
    }

    return ss.str();
}
