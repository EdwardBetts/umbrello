/***************************************************************************
                          cppwriter.cpp  -  description
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

#include "cppwriter.h"


#include <kdebug.h>

#include <klocale.h>
#include <kmessagebox.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>

#include "../concept.h"
#include "../operation.h"
#include "../umlnamespace.h"
#include "../umlview.h"           //hmm...I dont think this 3 includes should
#include "../umlwidget.h"         //be needed here, but the way I have understood the code
#include "../associationwidget.h" //so far, there is no other way of getting the associations



CppWriter::CppWriter( QObject *parent, const char *name )
		:CodeGenerator( parent, name) {}

CppWriter::~CppWriter() {}


void CppWriter::writeClass(UMLConcept *c) {
	if(!c) {
		kdDebug()<<"Cannot write class of NULL concept!\n";
		return;
	}


	/*to me it�d be more natural to have the associations direct in the document,
	  or even better, in the concept object itself and only the association
	  widget in the view, the same as with umlobject and umlwidget... but I didnt write
	  the association code, so lets try to use it here
	*/
	UMLView *view;
	view = m_doc->getCurrentView();

	AssociationWidgetList associations;
	associations.setAutoDelete(false);
	AssociationWidgetList aggregations;
	aggregations.setAutoDelete(false);
	AssociationWidgetList compositions;
	compositions.setAutoDelete(false);
	QList<UMLConcept> generalizations;
	generalizations.setAutoDelete(false);

	view->getWidgetAssocs(c,associations);


	//find out associations for this class
	//only base class needs to know about generalization, and
	//only the "whole" needs to know about the "parts" in aggregation / composition
	for(AssociationWidget *a = associations.first(); a ; a = associations.next())
		switch(a->getAssocType()) {
			case Uml::at_Generalization:
				if(a->getData()->getWidgetAID()==c->getID())
					generalizations.append((UMLConcept*)m_doc->findUMLObject(a->getData()->getWidgetBID()));
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
	fileName = findFileName(c,".h");
	if (!fileName) {
		emit codeGenerated(c, false);
		return;
	}

	QFile fileh, filecpp;
	if( !openFile(fileh,fileName+".h") || !openFile(filecpp,fileName+".cpp")) {
		emit codeGenerated(c, false);
		return;
	}
	QTextStream h(&fileh),cpp(&filecpp);

	//////////////////////////////
	//Start generating the code!!
	/////////////////////////////


	//try to find a heading file (license, coments, etc)
	QString str;

	str = getHeadingFile(".h");
	if(!str.isEmpty()) {
		str.replace(QRegExp("%filename%"),fileName+".h");
		str.replace(QRegExp("%filepath%"),fileh.name());
		h<<str<<endl;
	}

	str = getHeadingFile(".cpp");
	if(!str.isEmpty()) {
		str.replace(QRegExp("%filename%"),fileName+".cpp");
		str.replace(QRegExp("%filepath%"),filecpp.name());
		cpp<<str<<endl;
	}


	QString hashDefine = classname.upper().simplifyWhiteSpace().replace(QRegExp(" "),  "_");

	h << "\n#ifndef "<< hashDefine + "_H" << endl;
	h << "#define "<< hashDefine + "_H\n\n";
	cpp << "\n#include \"" << fileName << ".h\"\n\n";

	//write includes and take namespaces into account
	QList<UMLConcept> includes;
	findObjectsRelated(c,includes);
	UMLConcept* conc;
	for(conc = includes.first(); conc ;conc = includes.next()) {
		QString headerName = findFileName(conc, ".h");
		if (headerName.isEmpty()) {
			h<<"#include \""<<findFileName(conc,".h")<<".h\"\n";
		}
	}
	h<<"\n";
	for(conc = includes.first(); conc ; conc = includes.next()) {
		if(conc->getPackage()!=c->getPackage() && !conc->getPackage().isEmpty()) {
			h<<"using "<<cleanName(conc->getPackage())<<"::"<<cleanName(conc->getName())<<";\n";
		}
	}

	if(!c->getPackage().isEmpty()) {
		h<<"\nnamespace "<<cleanName(c->getPackage())<<"{\n\n";
		cpp<<"\nusing namespace "<<cleanName(c->getPackage())<<";\n\n";
	}

	//Write class Documentation if there is somthing or if force option
	if(forceDoc() || !c->getDoc().isEmpty()) {
		h<<"\n/**\n";
		h<<"  * class "<<classname<<endl;
		h<<formatDoc(c->getDoc(),"  * ");
		h<<"  */\n\n";
	}


	//check if class is abstract and / or has abstract methods
	if(c->getAbstract() && !hasAbstractOps(c))
		h<<"/******************************* Abstract Class ****************************\n  "
		<<classname<<" does not have any pure virtual methods, but its author\n"
		<<"  defined it as an abstract class, so you should not use it directly.\n"
		<<"  Inherit from it instead and create only objects from the derived classes\n"
		<<"*****************************************************************************/\n\n";

	h<<"class "<<classname<<(generalizations.count() > 0 ? " : ":"");
	int i;
	UMLObject *obj;
	for(obj = generalizations.first(), i = generalizations.count();
	        obj && i;
	        obj = generalizations.next(), i--)
		h<<"public "<<cleanName(obj->getName())<<(i>1?", ":"");
	h<<"\n{\n";


	//operations
	writeOperations(c,h,cpp);

	//associations
	if( forceSections() || !aggregations.isEmpty()) {
		h<<"\n/**Aggregations: */\n";
		for(AssociationWidget *a = aggregations.first(); a; a = aggregations.next()) {
			h<<"private:\n";
			//maybe we should parse the string here and take multiplicty into account to decide
			//which container to use.
			if (a->getMultiA().isEmpty())
				h << cleanName(a->getWidgetA()->getName()) << " *m_" << cleanName(a->getWidgetA()->getName()) << ";\n";
			else
				h << "vector<" << cleanName(a->getWidgetA()->getName()) << "*> "
				<< cleanName(a->getWidgetA()->getName()).lower() << "Vector;" << endl;
		}//end for
	}

	if( forceSections() || !compositions.isEmpty()) {
		h<<"\n/**Compositions: */\n";
		for(AssociationWidget *a = compositions.first(); a ; a = compositions.next()) {
			// see comment on Aggregation about multiplicity...
			if(a->getMultiA().isEmpty())
				h<<cleanName(a->getWidgetA()->getName())<<" m_"<<cleanName(a->getWidgetA()->getName())<<";\n";
			else
				h<<"vector<"<<cleanName(a->getWidgetA()->getName())<<"> "
				<<cleanName(a->getWidgetA()->getName()).lower()<<"Vector;\n";
		}
	}


	//attributes
	writeAttributes(c,h);

	//finish files
	h<<"\n};\n\n";
	if(!c->getPackage().isEmpty())
		h<<"};  //end of class namespace\n";
	h<<"#endif // " << hashDefine + "_H\n";

	if(hasDefaultValueAttr(c)) {
		QList<UMLAttribute> *atl = c->getAttList();
		cpp<<"void "<<classname<<"::initAttributes( )\n{\n";
		for(UMLAttribute *at = atl->first(); at ; at = atl->next())
			if(!at->getInitialValue().isEmpty())
				cpp<<"  "<<cleanName(at->getName())<<" = "<<at->getInitialValue()<<";\n";
		cpp<<"}\n";
	}

	cpp<<endl;
	//close files and notfiy we are done
	fileh.close();
	filecpp.close();
	emit codeGenerated(c, true);
}


////////////////////////////////////////////////////////////////////////////////////
//  Helper Methods

void CppWriter::writeOperations(UMLConcept *c,QTextStream &h, QTextStream &cpp) {

	//Lists to store operations  sorted by scope
	QList<UMLOperation> *opl;
	QList<UMLOperation> oppub,opprot,oppriv;

	oppub.setAutoDelete(false);
	opprot.setAutoDelete(false);
	oppriv.setAutoDelete(false);

	//sort operations by scope first and see if there are abstract methods
	opl = c->getOpList();
	for(UMLOperation *op = opl->first(); op ; op = opl->next()) {
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

	QString classname(cleanName(c->getName()));

	//write operations to file
	if(forceSections() || !oppub.isEmpty()) {
		h<<"\n/** Public methods: */\npublic:\n";
		writeOperations(classname,oppub,h,cpp);
	}

	if(forceSections() || !opprot.isEmpty()) {
		h<<"\n\n/** Protected methods: */\nprotected:\n";
		writeOperations(classname,opprot,h,cpp);
	}

	if(forceSections() || !oppriv.isEmpty()) {
		h<<"\n\n/** Private methods: */\nprivate:\n";
		writeOperations(classname,oppriv,h,cpp);
	}

	if(hasDefaultValueAttr(c)) {
		QString temp = "initAttributes sets all " + classname + " attributes to its default \
		               value make sure to call this method within your class constructor";
		h<<"  /**\n";
		h<<formatDoc(temp,"    * ");
		h<<"    */\n";
		h<<"private:  void initAttributes();\n";
	}
}

void CppWriter::writeOperations(QString classname, QList<UMLOperation> &opList, QTextStream &h, QTextStream &cpp) {
	UMLOperation *op;
	QList<UMLAttribute> *atl;
	UMLAttribute *at;

	for(op=opList.first(); op ; op=opList.next()) {
		atl = op -> getParmList();
		//write method doc if we have doc || if at least one of the params has doc
		bool writeDoc = forceDoc() || !op->getDoc().isEmpty();
		for(at = atl->first(); at ; at = atl -> next())
			writeDoc |= !at->getDoc().isEmpty();

		if( writeDoc )  //write method documentation
		{
			h<<"    /**\n"
			<<formatDoc(op->getDoc(),"      * ");

			for(at = atl->first(); at ; at = atl -> next())  //write parameter documentation
			{
				if(forceDoc() || !at->getDoc().isEmpty()) {
					h<<"      * @param " + cleanName(at->getName())<<endl;
					h<<formatDoc(at->getDoc(),"      *        ");
				}
			}//end for : write parameter documentation
			h<<"      */" << endl;
		}//end if : write method documentation

		h<< (op->getAbstract() ? "    virtual ":"    ")
		<< (op->getStatic() ? "static " : "")
		<<  op->getReturnType() << " " << cleanName(op->getName()) << "( ";

		cpp<< op->getReturnType() << " " << classname << "::"
		<< cleanName(op->getName()) + "( ";

		int i= atl->count();
		int j=0;
		for( at = atl->first(); at ;at = atl->next(),j++) {
			h << at->getTypeName() << " " << cleanName(at->getName())
			<< (!(at->getInitialValue().isEmpty()) ?
			    (QString(" = ")+at->getInitialValue()) :
			    QString(""))
			<< ((j < i-1)?", ":"");

			cpp << at->getTypeName() << " " << cleanName(at->getName()) << QString(((j < i-1)?", ":""));
		}
		h<< QString(op->getAbstract() ? " ) = 0;" : " );" );
		cpp <<" )\n{\n\n}\n";

		h<<"\n\n";
		cpp<<"\n\n";


	}//end for
}

void CppWriter::writeAttributes(UMLConcept *c, QTextStream &h) {
	QList<UMLAttribute> *atl;

	QList <UMLAttribute>  atpub, atprot, atpriv, atdefval;
	atpub.setAutoDelete(false);
	atprot.setAutoDelete(false);
	atpriv.setAutoDelete(false);
	atdefval.setAutoDelete(false);

	//sort attributes by scope and see if they have a default value
	atl = c->getAttList();
	UMLAttribute *at;
	for(at = atl->first(); at ; at = atl->next()) {
		if(!at->getInitialValue().isEmpty())
			atdefval.append(at);
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
		h<<"\n\n/**Attributes: */\n"<<endl;

	if(forceSections() || atpub.count()) {
		h<<"public:\n";
		writeAttributes(atpub,h);
	}

	if(forceSections() || atprot.count()) {
		h<<"protected:\n";
		writeAttributes(atprot,h);
	}

	if(forceSections() || atpriv.count()) {
		h<<"private:\n";
		writeAttributes(atpriv,h);
	}
}


void CppWriter::writeAttributes(QList<UMLAttribute> &atList, QTextStream &h) {
	for (UMLAttribute *at = atList.first(); at ; at = atList.next()) {
		if (forceDoc() || !at->getDoc().isEmpty()) {
			h << "    /**" << endl
			<< formatDoc(at->getDoc(), "      * ")
			<< "      */" << endl;
		}
		h << "    " << at->getTypeName() << " " << cleanName(at->getName()) << ";" << endl;

	} // end for
}



