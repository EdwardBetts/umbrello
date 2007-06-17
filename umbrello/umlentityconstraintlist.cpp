/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#include "umlentityconstraintlist.h"
#include "entityconstraint.h"
#include <kdebug.h>
#include <klocale.h>

void UMLEntityConstraintList::copyInto(UMLEntityConstraintList* rhs) const {
    // Don't copy yourself.
    if (rhs == this) return;

    rhs->clear();

    // Suffering from const; we shall not modify our object.
    UMLEntityConstraintList* tmp = new UMLEntityConstraintList(*this);

    UMLEntityConstraint* item;
    for (item = tmp->first(); item; item = tmp->next() ) {
        rhs->append((UMLEntityConstraint*)item->clone());
    }
    delete tmp;
}


UMLEntityConstraintList* UMLEntityConstraintList::clone() const {
    UMLEntityConstraintList *clone = new UMLEntityConstraintList();
    copyInto(clone);
    return clone;
}
