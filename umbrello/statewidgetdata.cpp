/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "statewidgetdata.h"

StateWidgetData::StateWidgetData(SettingsDlg::OptionState optionState):UMLWidgetData(optionState) {
	m_StateType = StateWidget::Normal;
	m_Name = "State";
	m_Doc = "";
	m_Type = Uml::wt_State;
}

StateWidgetData::StateWidgetData(StateWidgetData & Other) : UMLWidgetData(Other) {
	*this = Other;
}

StateWidgetData::~StateWidgetData() {
}

StateWidgetData & StateWidgetData::operator=(StateWidgetData & Other) {
	*((UMLWidgetData*)this) = (UMLWidgetData)Other;
	m_StateType = Other.m_StateType;
	m_Name = Other.m_Name;
	return *this;
}

bool StateWidgetData::operator==(StateWidgetData & Other) {
	if(!(*((UMLWidgetData*)this) == (UMLWidgetData)Other)) {
		return false;
	}
	if( m_StateType != Other.m_StateType )
		return false;
	if( m_Name != Other.m_Name )
		return false;
	return true;
}

bool StateWidgetData::saveToXMI( QDomDocument & qDoc, QDomElement & qElement ) {
	QDomElement stateElement = qDoc.createElement( "UML:StateWidget" );
	bool status = UMLWidgetData::saveToXMI( qDoc, stateElement );
	stateElement.setAttribute( "statename", m_Name );
	stateElement.setAttribute( "documentation", m_Doc );
	stateElement.setAttribute( "statetype", m_StateType );
	//save states activities
	QDomElement activitiesElement = qDoc.createElement( "Activities" );
	for( QStringList::Iterator it = m_Activities.begin(); it != m_Activities.end(); ++it ) {
		QDomElement tempElement = qDoc.createElement( "Activity" );
		tempElement.setAttribute( "name", *it );
		activitiesElement.appendChild( tempElement );
	}//end for
	stateElement.appendChild( activitiesElement );
	qElement.appendChild( stateElement );
	return status;
}

bool StateWidgetData::loadFromXMI( QDomElement & qElement ) {
	if( !UMLWidgetData::loadFromXMI( qElement ) )
		return false;
	m_Name = qElement.attribute( "statename", "" );
	m_Doc = qElement.attribute( "documentation", "" );
	QString type = qElement.attribute( "statetype", "1" );
	m_StateType = (StateWidget::StateType)type.toInt();
	//load states activities
	QDomNode node = qElement.firstChild();
	QDomElement tempElement = node.toElement();
	if( !tempElement.isNull() && tempElement.tagName() == "Activities" ) {
		QDomNode node = tempElement.firstChild();
		QDomElement activityElement = node.toElement();
		while( !activityElement.isNull() ) {
			if( activityElement.tagName() == "Activity" ) {
				QString name = activityElement.attribute( "name", "" );
				if( !name.isEmpty() )
					m_Activities.append( name );
			}//end if
			node = node.nextSibling();
			activityElement = node.toElement();
		}//end while
	}//end if
	return true;
}

bool StateWidgetData::addActivity( QString activity ) {
	m_Activities.append( activity );
	return true;
}

bool StateWidgetData::removeActivity( QString activity ) {
	int index = - 1;
	if( ( index = m_Activities.findIndex( activity ) ) == -1 )
		return false;
	m_Activities.remove( m_Activities.at( index ) );
	return true;
}

void StateWidgetData::setActivities( QStringList & list ) {
	m_Activities = list;
}

bool StateWidgetData::renameActivity( QString activity, QString newName ) {
	int index = - 1;
	if( ( index = m_Activities.findIndex( activity ) ) == -1 )
		return false;
	m_Activities[ index ] = newName;
	return true;
}






