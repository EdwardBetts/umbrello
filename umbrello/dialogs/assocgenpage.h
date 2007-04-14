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

#ifndef ASSOCGENPAGE_H
#define ASSOCGENPAGE_H

//quicktime class includes
#include <qwidget.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qmultilineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qvaluelist.h>

//my class includes
#include "../umlobject.h"
#include "../objectwidget.h"
#include "../umldoc.h"
#include "../associationwidget.h"

class KComboBox;


/**
 * Displays properties of a UMLObject in a dialog box.  This is not usually directly
 * called.  The class @ref AssocPropDlg will set this up for you.
 *
 * @short Display properties on a UMLObject.
 * @author Paul Hensgen <phensgen@techie.com>
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class AssocGenPage : public QWidget {
    Q_OBJECT
public:

    /**
     *  Sets up the AssocGenPage.
     *
     *  @param  d       The UMLDoc which controls controls object creation.
     *  @param  parent  The parent to the AssocGenPage.
     *  @param  o       The ObjectWidget to display the properties of.
     */
    AssocGenPage(UMLDoc *d, QWidget *parent, AssociationWidget *a);

    /**
     *  Standard deconstructor.
     */
    ~AssocGenPage();

    /**
     *  Will move information from the dialog into the object.
     *  Call when the ok or apply button is pressed.
     */
    void updateObject();

private:
    QLineEdit * m_pAssocNameLE;
    KComboBox *m_pTypeCB;

    /* Choices for the QComboBox, and we store ints and strings
       so we can translate both ways */
    QValueList<Uml::Association_Type> m_AssocTypes;
    QStringList m_AssocTypeStrings;

    QMultiLineEdit * m_pDoc;

    AssociationWidget *m_pAssociationWidget;
    UMLDoc * m_pUmldoc;
    ObjectWidget * m_pWidget;

    void constructWidget();

public slots:
    /**
     *  When the draw as actor check box is toggled, the draw
     *  as multi instance need to be enabled/disabled.  They
     *  both can't be available at the same time.
     */
    // void slotActorToggled( bool state );
};

#endif
