/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2012                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#include "cmd_create_classdiag.h"

#include "umldoc.h"
#include "umlview.h"

#include <klocale.h>

namespace Uml
{

    CmdCreateClassDiag::CmdCreateClassDiag(UMLDoc* doc, const QString& name)
      : QUndoCommand(),
        m_pUMLDoc(doc),
        m_pUMLView(0)
    {
        setText(i18n("Create class diagram : %1", name));
    }

    CmdCreateClassDiag::~CmdCreateClassDiag()
    {
    }

    void CmdCreateClassDiag::redo()
    {
        UMLFolder* temp = m_pUMLDoc->rootFolder(Uml::ModelType::Logical);
        m_pUMLView = m_pUMLDoc->createDiagram(temp, Uml::DiagramType::Class);
    }

    void CmdCreateClassDiag::undo()
    {
        if (m_pUMLView) {
            m_pUMLDoc->removeDiagram(m_pUMLView->getID());
        }
    }

}
