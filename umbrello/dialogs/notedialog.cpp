/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qlayout.h>
#include <qlabel.h>
//kde includes
#include <klocale.h>
#include <kmessagebox.h>
//app includes
#include "notedialog.h"


NoteDialog::NoteDialog( QWidget * parent, NoteWidget * pNote ) : KDialogBase(Plain, i18n("Note Documentation"), Help | Ok | Cancel , Ok, parent, "_NOTEDIALOG_", true, true) {
	m_pNoteWidget = pNote;
	int margin = fontMetrics().height();

	m_pDocGB = new QGroupBox(i18n("Documenation"), plainPage());
	QVBoxLayout * mainLayout = new QVBoxLayout(plainPage());
	mainLayout -> addWidget(m_pDocGB);
	mainLayout -> setSpacing(10);
	mainLayout -> setMargin(margin);

	QHBoxLayout * docLayout = new QHBoxLayout(m_pDocGB);
	docLayout -> setSpacing(10);
	docLayout -> setMargin(margin);

	m_pDocTE = new QTextEdit( m_pDocGB );
	m_pDocTE -> setFocus();
	docLayout -> addWidget( m_pDocTE );
	m_pDocTE -> setText( pNote -> getDoc() );
	setMinimumSize(330, 160);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
NoteDialog::~NoteDialog() {}

void NoteDialog::slotOk() {
	m_pNoteWidget -> setDoc( m_pDocTE -> text() );
	accept();
}

