/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef PACKAGEWIDGET_H
#define PACKAGEWIDGET_H

#include "newumlrectwidget.h"

class UMLPackage;
class TextItemGroup;

class PackageWidget : public NewUMLRectWidget
{
public:
    PackageWidget(UMLPackage * o);
    virtual ~PackageWidget();

    void saveToXMI(QDomDocument& qDoc, QDomElement& qElement);

    QSizeF sizeHint(Qt::SizeHint which);

    void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w);

protected:
    void updateGeometry();
    void sizeHasChanged(const QSizeF& oldSize);

private:
    enum {
        StereoTypeItemIndex = 0,
        NameItemIndex = 1
    };

    static const qreal Margin;

    QRectF m_topRect;
    QRectF m_packageTextRect;
    QSizeF m_minimumSize;
    TextItemGroup *m_textItemGroup;
};

#endif
