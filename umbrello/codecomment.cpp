
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Wed Jun 18 2003
 */

#include "codecomment.h"
#include "codedocument.h"
#include <kdebug.h>

CodeComment::CodeComment (CodeDocument * doc, const QString & comment )
        : TextBlock ( doc, comment )
{
}

CodeComment::~CodeComment ( )
{
}

void CodeComment::saveToXMI ( QDomDocument & doc, QDomElement & root )
{
    QDomElement blockElement = doc.createElement( "codecomment" );
    setAttributesOnNode(doc, blockElement); // as we added no additional fields to this class we may
    // just use parent TextBlock method
    root.appendChild( blockElement );
}

void CodeComment::loadFromXMI ( QDomElement & root )
{
    setAttributesFromNode(root);
}


#include "codecomment.moc"
