/***************************************************************************
                          javawriter.cpp  -  description
                             -------------------
    begin                : Mon Jun 17 2002
    copyright            : (C) 2002 by Luis De la Parra Blum
    email                : luis@delaparra.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "javawriter.h"

#include <kdebug.h>

#include <klocale.h>
#include <kmessagebox.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>

#include "../concept.h"
#include "../operation.h"
#include "../umlnamespace.h"
#include "../umlview.h"    //hmm...I dont think this 3 includes should
#include "../umlwidget.h"  //be needed here, but the way I have understood the code
#include "../associationwidget.h" //so far, there is no other way of getting the associations



JavaWriter::JavaWriter( QObject *parent, const char *name )
		:CodeGenerator( parent, name) {}

JavaWriter::~JavaWriter() {}

void JavaWriter::writeClass(UMLConcept *c) {


	if(!c) {
		kdDebug()<<"Cannot write class of NULL concept!\n";
		return;
	}



	/*I dont really understand how the associations are implemented, so this is probably
	  a very ugly way of doing things... to me it�d be more natural to have the associations
	  direct in the document, or even better, in the concept object itself and only the association
	  widget in the view, the same as with umlobject and umlwidget.
	*/
	UMLView *view;
	view = m_doc->getCurrentView();

	AssociationWidgetList associations;
	associations.setAutoDelete(false);
	AssociationWidgetList aggregations;
	aggregations.setAutoDelete(false);
	AssociationWidgetList compositions;
	compositions.setAutoDelete(false);
	QList<UMLObject> generalizations;
	generalizations.setAutoDelete(false);

	view->getWidgetAssocs(c,associations);

	//find out associations for this class
	//only base class needs to know about generalization, and
	//only the "whole" needs to know about the "parts" in aggregation / composition
	for(AssociationWidget *a = associations.first(); a ; a=associations.next())
		switch(a->getAssocType()) {
			case Uml::at_Generalization:
				if(a->getData()->getWidgetAID()==c->getID())
					generalizations.append(m_doc->findUMLObject(a->getData()->getWidgetBID()));
				break;
			case Uml::at_Aggregation:
				if(a->getData()->getWidgetBID()==c->getID())
					aggregations.append(a);
				break;
			case Uml::at_Composition:
				if(a->getData()->getWidgetBID()==c->getID())
					compositions.append(a);
				break;
			default: //we dont support any other associations for the moment, =(
				break;
		}



	QString classname = cleanName(c->getName());
	QString fileName = c->getName().lower();


	//find an appropiate name for our file
	fileName = findFileName(c,".java");
	if (!fileName) {
		emit codeGenerated(c, false);
		return;
	}

	QFile file;
	if( !openFile(file,fileName+".java") ) {
		emit codeGenerated(c, false);
		return;
	}

	//Start generating the code!!

	QTextStream java(&file);
	//try to find a heading file (license, coments, etc)
	QString str;
	str = getHeadingFile(".java");
	if(!str.isEmpty()) {
		str.replace(QRegExp("%filename%"),fileName);
		str.replace(QRegExp("%filepath%"),file.name());
		java<<str<<endl;
	}

	if(!c->getPackage().isEmpty())
		java<<"package "<<c->getPackage()<<";\n";

	java<<"\nimport java.util.*;\n";

	//only import classes in a different package as this class
	QList<UMLConcept> imports;
	findObjectsRelated(c,imports);
	for(UMLConcept *con = imports.first(); con ; con = imports.next())
		if(con->getPackage() != c->getPackage())
			java<<"import "<<con->getPackage()<<"."<<cleanName(con->getName())<<";\n";


	java<<endl;

	//Write class Documentation if there is somthing or if force option
	if(forceDoc() || !c->getDoc().isEmpty()) {
		java<<"\n/**\n";
		java<<"  * class "<<classname<<endl;
		java<<formatDoc(c->getDoc(),"  * ");
		java<<"  */\n\n";
	}

	java<<(c->getAbstract()?QString("abstract "):QString(""))
	<<"public class "<<classname<<(generalizations.count()>0?" extends ":"");
	int i;
	UMLObject *obj;
	for(obj = generalizations.first(), i = generalizations.count();
	        obj && i;
	        obj = generalizations.next(), i--)
		java<<cleanName(obj->getName())<<(i>1?", ":"");

	java<<"\n{\n";

	writeAttributes(c,java);
	java<<endl;
	writeOperations(c,java);



	if( forceSections() || !aggregations.isEmpty() ) {
		java<<"\n/**Aggregations: */\n";
		for(AssociationWidget *a = aggregations.first(); a; a = aggregations.next()) {
			if(a->getMultiA().isEmpty())
				java<<"private "<<cleanName(a->getWidgetA()->getName())<<" m_"<<cleanName(a->getWidgetA()->getName())<<";\n";
			else
				java<<"private Vector "<<
				cleanName(a->getWidgetA()->getName()).lower()<<"Vector;\n";
		}
	}


	if( forceSections() || !compositions.isEmpty()) {
		java<<"\n/**Compositions: */\n";
		for(AssociationWidget *a = compositions.first(); a; a = compositions.next()) {
			if(a->getMultiA().isEmpty())
				java<<"private "<<cleanName(a->getWidgetA()->getName())<<" m_"<<cleanName(a->getWidgetA()->getName())<<";\n";
			else
				java<<"private Vector "<<
				cleanName(a->getWidgetA()->getName()).lower()<<"Vector;\n";
		}
	}

	java<<"\n}\n";

	file.close();
	emit codeGenerated(c, true);
}


void JavaWriter::writeAttributes(UMLConcept *c, QTextStream &java) {
	QList<UMLAttribute> *atl;
	QList <UMLAttribute>  atpub, atprot, atpriv;
	atpub.setAutoDelete(false);
	atprot.setAutoDelete(false);
	atpriv.setAutoDelete(false);

	UMLAttribute *at;
	//sort attributes by scope and see if they have a default value
	atl = c->getAttList();
	for(at=atl->first(); at ; at=atl->next()) {
		switch(at->getScope()) {
			case Uml::Public:
				atpub.append(at);
				break;
			case Uml::Protected:
				atprot.append(at);
				break;
			case Uml::Private:
				atpriv.append(at);
				break;
		}
	}

	if(forceSections() || atl->count())
		java<<"\n/**Attributes: */\n"<<endl;

	for(at=atpub.first(); at; at=atpub.next())
		java<<"\tpublic "<<at->getTypeName()<<" "<<cleanName(at->getName())
		<<(at->getInitialValue().isEmpty()?QString(""):QString(" = ") + at->getInitialValue())
		<<";\n";

	for(at=atprot.first();at;at=atprot.next())
		java<<"\tprotected "<<at->getTypeName()<<" "<<cleanName(at->getName())
		<<(at->getInitialValue().isEmpty()?QString(""):QString(" = ") + at->getInitialValue())
		<<";\n";

	for(at=atpriv.first();at;at=atpriv.next())
		java<<"\tprivate "<<at->getTypeName()<<" "<<cleanName(at->getName())
		<<(at->getInitialValue().isEmpty()?QString(""):QString(" = ") + at->getInitialValue())
		<<";\n";



}

void JavaWriter::writeOperations(UMLConcept *c, QTextStream &java) {
	QList<UMLOperation> *opl;
	QList <UMLOperation> oppub,opprot,oppriv;
	oppub.setAutoDelete(false);
	opprot.setAutoDelete(false);
	oppriv.setAutoDelete(false);

	UMLOperation *op;
	//sort operations by scope first and see if there are abstrat methods
	opl = c->getOpList();
	for(op = opl->first(); op ; op=opl->next()) {
		switch(op->getScope()) {
			case Uml::Public:
				oppub.append(op);
				break;
			case Uml::Protected:
				opprot.append(op);
				break;
			case Uml::Private:
				oppriv.append(op);
				break;
		}
	}

	if(forceSections() || oppub.count())
		java<<"\n/** Public methods: */\n";
	writeOperations(oppub,java);

	if(forceSections() || opprot.count())
		java<<"\n/** Protected methods: */\n";
	writeOperations(opprot,java);

	if(forceSections() || oppriv.count())
		java<<"\n/** Private methods: */\n";
	writeOperations(oppriv,java);

}

void JavaWriter::writeOperations(QList<UMLOperation> &oplist, QTextStream &java) {
	UMLOperation *op;
	QList<UMLAttribute> *atl;
	UMLAttribute *at;
	int i,j;
	QString str;
	for( op=oplist.first(); op ;op=oplist.next()) {
		str = (op->getAbstract() ? "abstract ":"");
		switch(op->getScope()) {
			case Uml::Public:
				str += "public ";
				break;
			case Uml::Protected:
				str += "protected ";
				break;
			case Uml::Private:
				str += "private ";
				break;
		}
		str += op->getReturnType() + " " +cleanName(op->getName()) + "( ";

		atl = op->getParmList();
		i= atl->count();
		j=0;
		for( at = atl->first(); at ;at = atl->next(),j++) {
			str += at->getTypeName() + " " + cleanName(at->getName())+
			       (!(at->getInitialValue().isEmpty()) ?
			        (QString(" = ")+at->getInitialValue()) :
			        QString(""))
			       + ((j < i-1)?", ":"");
		}
		str+= " )\n\t{\n\n\t}\n\n";
		java<<str;
	}

}




