/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qcheckbox.h>
#include <qlayout.h>
#include <qlabel.h>

#include <klocale.h>
#include <kdebug.h>

#include "overwritedialogue.h"

OverwriteDialogue::OverwriteDialogue(QString fileName, QString outputDirectory, bool applyToAllRemaining, QWidget* parent, const char* name):
	KDialogBase(Plain, i18n("Destination file already exists"), Ok|Apply|Cancel, Yes, parent, name) {

	QVBoxLayout* layout = new QVBoxLayout( plainPage(), 0, spacingHint() );

	QLabel* dialogueLabel = new QLabel(i18n("The file %1 already exists in %2.\n\nUmbrello can overwrite the file, generate a similar\nfile name or not generate this file.").arg(fileName).arg(outputDirectory), plainPage() );
	layout->addWidget(dialogueLabel);

	m_applyToAllRemaining = new QCheckBox( i18n("&Apply to all remaining files"), plainPage() );
	m_applyToAllRemaining->setChecked(applyToAllRemaining);
	layout->addWidget(m_applyToAllRemaining);

	setButtonText(KDialogBase::Ok, i18n("Overwrite"));
	setButtonText(KDialogBase::Apply, i18n("Generate a similar file name"));
	setButtonText(KDialogBase::Cancel, i18n("Do not generate file"));
}

OverwriteDialogue::~OverwriteDialogue() {
}

void OverwriteDialogue::slotOk() {
	done(Yes);
}

void OverwriteDialogue::slotApply() {
	done(No);
}

void OverwriteDialogue::slotCancel() {
	done(Cancel);
}

bool OverwriteDialogue::applyToAllRemaining() {
	return m_applyToAllRemaining->isChecked();
}
