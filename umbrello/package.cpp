/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "package.h"
#include "umldoc.h"
#include <kdebug.h>
#include <klocale.h>

UMLPackage::UMLPackage(UMLDoc * parent, const QString & name, int id)
  : UMLObject(parent, name, id) {
	init();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
UMLPackage::~UMLPackage() {
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void UMLPackage::init() {
	m_BaseType = ot_Package;
}

bool UMLPackage::saveToXMI(QDomDocument& qDoc, QDomElement& qElement) {
	QDomElement packageElement = qDoc.createElement("UML:Package");
	bool status = UMLObject::saveToXMI(qDoc, packageElement);
	qElement.appendChild(packageElement);
	return status;
}

bool UMLPackage::loadFromXMI(QDomElement& element) {
	if ( !UMLObject::loadFromXMI(element) ) {
		return false;
	}
	return true;
}

#include "package.moc"
