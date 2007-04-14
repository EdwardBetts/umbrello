
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Mon Jun 30 2003
 */



#ifndef JAVACODECLASSFIELD_H
#define JAVACODECLASSFIELD_H

#include <qstring.h>

#include "../codeclassfield.h"

class ClassifierCodeDocument;

class JavaCodeClassField : public CodeClassField
{
    Q_OBJECT
public:

    // Constructors/Destructors
    //

    /**
     * Constructors
     */
    JavaCodeClassField (ClassifierCodeDocument * parentDoc, UMLRole * role);
    JavaCodeClassField (ClassifierCodeDocument * parentDoc, UMLAttribute * attrib);

    /**
     * Empty Destructor
     */
    virtual ~JavaCodeClassField ( );

    QString getFieldType();
    QString getFieldName();
    QString getInitialValue();

    QString getTypeName ( );
protected:

private:

    // void initDeclCodeBlock ();

};

#endif // JAVACODECLASSFIELD_H
