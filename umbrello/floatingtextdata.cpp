/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kdebug.h>
#include <qdatastream.h>
#include "dialogs/settingsdlg.h"
#include "floatingtextdata.h"

FloatingTextData::FloatingTextData(): UMLWidgetData() {
	init();
}

FloatingTextData::~FloatingTextData() {
	init();
}

FloatingTextData::FloatingTextData(FloatingTextData & Other) : UMLWidgetData(Other) {
	*this = Other;
}

FloatingTextData & FloatingTextData::operator=(FloatingTextData & Other) {
	*((UMLWidgetData*)this) = (UMLWidgetData)Other;

	init();
	m_Text = Other.m_Text;
	m_Operation = Other.m_Operation;
	m_SeqNum = Other.m_SeqNum;
	m_Role = Other.m_Role;
	return *this;
}

void FloatingTextData::init( ) {
	m_PreText = "";
	m_Text = "";
	m_PostText = "";
	m_SeqNum = "";
	m_Operation = "";
	m_Role = Uml::tr_Floating;
	m_Type = Uml::wt_Text;
}

bool FloatingTextData::operator==(FloatingTextData & Other) {
	if( this == &Other )
		return true;

	if(!(*((UMLWidgetData*)this) == (UMLWidgetData)Other))
		return false;

	if(m_Text != Other.m_Text)
		return false;

	if( m_SeqNum != Other.m_SeqNum )
		return false;

	if( m_Operation != Other.m_Operation )
		return false;

	if( m_Role != Other.m_Role )
		return false;

	return true;
}

QString FloatingTextData::getText() {
	return m_Text;
}

QString FloatingTextData::getPreText() {
	return m_PreText;
}

QString FloatingTextData::getPostText() {
	return m_PostText;
}

QString FloatingTextData::getDisplayText()
{
	QString displayText = m_Text;
	displayText.prepend(m_PreText);
	displayText.append(m_PostText);
	return displayText;
}

void FloatingTextData::setText( QString Text) {
	m_Text = Text;
}

void FloatingTextData::setPreText( QString Text) {
	m_PreText = Text;
}

void FloatingTextData::setPostText ( QString Text) {
	m_PostText = Text;
}

void FloatingTextData::print2cerr() {

	UMLWidgetData::print2cerr();
	kdDebug() << "m_Text = " << m_Text << endl;
}

void FloatingTextData::setSeqNum( QString number ) {
	QString temp;
	m_SeqNum = number;

	temp = m_SeqNum;
	temp.append( ": " );
	temp += m_Operation;

	if(m_SeqNum.length() == 0 && m_Operation.length() == 0)
		m_Text = "";
	else
		m_Text = temp;
}

void FloatingTextData::setOperation( QString operation ) {
	QString temp;
	m_Operation = operation;

	temp = m_SeqNum;
	temp.append( ": " );
	temp += m_Operation;

	if(m_SeqNum.length() == 0 && m_Operation.length() == 0)
		m_Text = "";
	else
		m_Text = temp;
}
void FloatingTextData::setRole(Uml::Text_Role Role) {
	m_Role = Role;
}

Uml::Text_Role FloatingTextData::getRole() {
	return m_Role;
}

bool FloatingTextData::saveToXMI( QDomDocument & qDoc, QDomElement & qElement ) {
	QDomElement textElement = qDoc.createElement( "UML:FloatingTextWidget" );
	bool status = UMLWidgetData::saveToXMI( qDoc, textElement );
	textElement.setAttribute( "text", m_Text );
	textElement.setAttribute( "pretext", m_PreText );
	textElement.setAttribute( "posttext", m_PostText );
	textElement.setAttribute( "operation", m_Operation );
	textElement.setAttribute( "seqnum", m_SeqNum );
	textElement.setAttribute( "role", m_Role );
	qElement.appendChild( textElement );
	return status;
}

bool FloatingTextData::loadFromXMI( QDomElement & qElement ) {
	if( !UMLWidgetData::loadFromXMI( qElement ) )
		return false;
	m_Text = qElement.attribute( "text", "" );
	m_PreText = qElement.attribute( "pretext", "" );
	m_PostText = qElement.attribute( "posttext", "" );
	m_Operation  = qElement.attribute( "operation", "" );
	m_SeqNum = qElement.attribute( "seqnum", "" );
	QString role = qElement.attribute( "role", "" );

	if( !role.isEmpty() )
		m_Role = (Uml::Text_Role)role.toInt();

	return true;
}







