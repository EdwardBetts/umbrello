/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "stereotype.h"
#include "inputdialog.h"
#include "umldoc.h"
#include "uml.h"

#include <klocale.h>
#include <kdebug.h>

UMLStereotype::UMLStereotype(QString name, int id /* = -1 */)
  : UMLObject( name, id ) {
	m_BaseType = ot_Stereotype;
	m_refCount = 0;
}

UMLStereotype::UMLStereotype() : UMLObject() {
	m_BaseType = ot_Stereotype;
	m_refCount = 0;
}

UMLStereotype::~UMLStereotype() {}

bool UMLStereotype::operator==( UMLStereotype &rhs) {
	if (this == &rhs) {
		return true;
	}

	if ( !UMLObject::operator==( rhs ) ) {
		return false;
	}

	return true;
}

void UMLStereotype::copyInto(UMLStereotype *rhs) const
{
	UMLObject::copyInto(rhs);
}

UMLObject* UMLStereotype::clone() const
{
	UMLStereotype *clone = new UMLStereotype();
	copyInto(clone);

	return clone;
}


void UMLStereotype::saveToXMI(QDomDocument& qDoc, QDomElement& qElement) {
	//FIXME: uml13.dtd compliance
	QDomElement stereotypeElement = UMLObject::save("UML:Stereotype", qDoc);
	qElement.appendChild( stereotypeElement );
}

bool UMLStereotype::load(QDomElement& /*element*/) {
	return true;
}

bool UMLStereotype::showPropertiesDialogue(QWidget* parent) {
	bool ok;
	QString name = KInputDialog::getText(i18n("Stereotype"), i18n("Enter name:"), getName(),&ok, parent);
	if (ok) {
		setName(name);
	}
	return ok;
}

void UMLStereotype::incrRefCount() {
	m_refCount++;
}

void UMLStereotype::decrRefCount() {
	m_refCount--;
}

int UMLStereotype::refCount() const {
	return m_refCount;
}

