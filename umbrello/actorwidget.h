/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ACTORWIDGET_H
#define ACTORWIDGET_H

#include "umlwidget.h"

#define A_WIDTH 20
#define A_HEIGHT 40
#define A_MARGIN 5
/**
 *	This class is the graphical version of a UML Actor.  An ActorWidget is created
 *	by a @ref UMLView.  An ActorWidget belongs to only one @ref UMLView instance.
 *	When the @ref UMLView instance that this class belongs to, it will be automatically deleted.
 *
 *	If the @ref Actor class that this ActorWidget is displaying is deleted, the @ref UMLView will
 *	make sure that this instance is also deleted.
 *
 *	The ActorWidget class inherits from the @ref UMLWidget class which adds most of the functionality
 *	 to this class.
 *
 *	@short	A graphical version of a UML Actor.
 *	@author Paul Hensgen <phensgen@techie.com>
 *	@version	1.0
 *	@see	UMLWidget
 *
 *	@see	UMLView
 */

class ActorWidget : public UMLWidget {
public:
	/**
	 *	Constructs an ActorWidget.
	 *
	 * @param view 	The parent of this ActorWidget.
	 * @param pData The ActorWidgetData to represent.
	 */
	ActorWidget(UMLView * view, UMLObject *o, UMLWidgetData* pData);

	/**
	 *	Constructs an ActorWidget.
	 *
	 * @param view 	The parent of this ActorWidget.
	 * @param o 	The Actor class this ActorWidget will display.
	 */
	ActorWidget(UMLView * view, UMLObject *o);

	/**
	 *	Constructs an ActorWidget.
	 *
	 * @param parent The parent of this ActorWidget.
	 */
	ActorWidget(UMLView * view);

	/**
	 *	Standard deconstructor.
	 */
	~ActorWidget();


	/**
	 * Returns the amount of bytes needed to serialize an instance object to the clipboard
	 */
	virtual long getClipSizeOf();

	/**
	 * Synchronizes the Widget's m_pData member with its display
	 * properties, for example: the X and Y positions of the
	 * widget, etc
	 */
	virtual void synchronizeData();

	/**
	 *	Overrides the standard paint event.
	 */
	void draw(QPainter & p, int offsetX, int offsetY);

protected:
	/**
	 *	Calculates the size of the widget.
	 */
	void calculateSize();
};

#endif
