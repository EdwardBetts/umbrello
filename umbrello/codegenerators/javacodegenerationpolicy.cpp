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
 *      Date   : Mon Jun 23 2003
 */

// own header
#include "javacodegenerationpolicy.h"
// qt/kde includes
#include <kconfig.h>
// app includes
#include "javacodegenerationpolicypage.h"
#include "javacodegenerator.h"
#include "../uml.h"

const JavaCodeGenerationPolicy::ScopePolicy JavaCodeGenerationPolicy::DEFAULT_ATTRIB_ACCESSOR_SCOPE = FromParent;
const JavaCodeGenerationPolicy::ScopePolicy JavaCodeGenerationPolicy::DEFAULT_ASSOC_FIELD_SCOPE = FromParent;
const JavaCodeGenerationPolicy::JavaCommentStyle JavaCodeGenerationPolicy::DEFAULT_COMMENT = SlashStar;
const bool JavaCodeGenerationPolicy::DEFAULT_AUTO_GEN_EMPTY_CONSTRUCTORS = false;
const bool JavaCodeGenerationPolicy::DEFAULT_AUTO_GEN_ATTRIB_ACCESSORS = true;
const bool JavaCodeGenerationPolicy::DEFAULT_AUTO_GEN_ASSOC_ACCESSORS = true;

// Constructors/Destructors
//

JavaCodeGenerationPolicy::JavaCodeGenerationPolicy(CodeGenerationPolicy *defaults)
        : CodeGenerationPolicy(defaults)
{
    init();
    setDefaults(defaults,false);
}

JavaCodeGenerationPolicy::JavaCodeGenerationPolicy(KConfig *config)
        : CodeGenerationPolicy(config)
{
    init();
    setDefaults(config,false);
}

JavaCodeGenerationPolicy::~JavaCodeGenerationPolicy ( ) { }

//
// Methods
//

// Accessor methods
//

// Public attribute accessor methods
//

/** Get the default scope for new accessor methods.
 */
JavaCodeGenerationPolicy::ScopePolicy JavaCodeGenerationPolicy::getAttributeAccessorScope () {
    return m_defaultAttributeAccessorScope;
}

/** Set the default scope for new accessor methods.
 */
void JavaCodeGenerationPolicy::setAttributeAccessorScope (ScopePolicy scope) {
    m_defaultAttributeAccessorScope = scope;
    emit modifiedCodeContent();
}

/** Get the default scope for new accessor methods.
 */
JavaCodeGenerationPolicy::ScopePolicy JavaCodeGenerationPolicy::getAssociationFieldScope() {
    return m_defaultAssociationFieldScope;
}

/** Set the default scope for new accessor methods.
 */
void JavaCodeGenerationPolicy::setAssociationFieldScope (ScopePolicy scope) {
    m_defaultAssociationFieldScope = scope;
    emit modifiedCodeContent();
}

/**
 * Set the value of m_commentStyle
 * @param new_var the new value of m_commentStyle
 */
void JavaCodeGenerationPolicy::setCommentStyle ( JavaCommentStyle new_var ) {
    m_commentStyle = new_var;
    emit modifiedCodeContent();
}

/**
 * Get the value of m_commentStyle
 * @return the value of m_commentStyle
 */
JavaCodeGenerationPolicy::JavaCommentStyle JavaCodeGenerationPolicy::getCommentStyle ( ) {
    return m_commentStyle;
}

/**
 * Set the value of m_autoGenerateConstructors
 * @param new_var the new value
 */
void JavaCodeGenerationPolicy::setAutoGenerateConstructors( bool var ) {
    m_autoGenerateConstructors = var;
    emit modifiedCodeContent();
}

/**
 * Get the value of m_autoGenerateConstructors
 * @return the value of m_autoGenerateConstructors
 */
bool JavaCodeGenerationPolicy::getAutoGenerateConstructors( ){
    return m_autoGenerateConstructors;
}

/**
 * Set the value of m_autoGenerateAttribAccessors
 * @param new_var the new value
 */
void JavaCodeGenerationPolicy::setAutoGenerateAttribAccessors( bool var ) {
    m_autoGenerateAttribAccessors = var;
    emit modifiedCodeContent();
}

/**
 * Set the value of m_autoGenerateAssocAccessors
 * @param new_var the new value
 */
void JavaCodeGenerationPolicy::setAutoGenerateAssocAccessors( bool var ) {
    m_autoGenerateAssocAccessors = var;
    emit modifiedCodeContent();
}

/**
 * Get the value of m_autoGenerateAttribAccessors
 * @return the value of m_autoGenerateAttribAccessors
 */
bool JavaCodeGenerationPolicy::getAutoGenerateAttribAccessors( ){
    return m_autoGenerateAttribAccessors;
}

/**
 * Get the value of m_autoGenerateAssocAccessors
 * @return the value of m_autoGenerateAssocAccessors
 */
bool JavaCodeGenerationPolicy::getAutoGenerateAssocAccessors( ){
    return m_autoGenerateAssocAccessors;
}

// Other methods
//

void JavaCodeGenerationPolicy::writeConfig ( KConfig * config )
{

    // write ONLY the Java specific stuff
    config->setGroup("Java Code Generation");

    config->writeEntry("defaultAttributeAccessorScope",int(getAttributeAccessorScope()));
    config->writeEntry("defaultAssocFieldScope",int(getAssociationFieldScope()));
    config->writeEntry("commentStyle",int(getCommentStyle()));
    config->writeEntry("autoGenEmptyConstructors",getAutoGenerateConstructors());
    config->writeEntry("autoGenAccessors",getAutoGenerateAttribAccessors());
    config->writeEntry("autoGenAssocAccessors",getAutoGenerateAssocAccessors());

    CodeGenerator *codegen = UMLApp::app()->getGenerator();
    JavaCodeGenerator *javacodegen = dynamic_cast<JavaCodeGenerator*>(codegen);
    if (javacodegen)
        config->writeEntry("buildANTDocument", javacodegen->getCreateANTBuildFile());

}

void JavaCodeGenerationPolicy::setDefaults ( CodeGenerationPolicy * clone, bool emitUpdateSignal )
{

    JavaCodeGenerationPolicy * jclone;
    if (!clone)
        return;

    // do the super class
    CodeGenerationPolicy::setDefaults(clone, false);

    // NOW block signals for java param setting
    blockSignals(true); // we need to do this because otherwise most of these
    // settors below will each send the modifiedCodeContent() signal
    // needlessly (we can just make one call at the end).


    // now do java-specific stuff IF our clone is also a JavaCodeGenerationPolicy object
    if((jclone = dynamic_cast<JavaCodeGenerationPolicy*>(clone)))
    {
        setAttributeAccessorScope(jclone->getAttributeAccessorScope());
        setAssociationFieldScope(jclone->getAssociationFieldScope());
        setCommentStyle(jclone->getCommentStyle());
        setAutoGenerateConstructors(jclone->getAutoGenerateConstructors());
        setAutoGenerateAttribAccessors(jclone->getAutoGenerateAttribAccessors());
        setAutoGenerateAssocAccessors(jclone->getAutoGenerateAssocAccessors());
    }

    blockSignals(false); // "as you were citizen"

    if(emitUpdateSignal)
        emit modifiedCodeContent();

}

void JavaCodeGenerationPolicy::setDefaults( KConfig * config, bool emitUpdateSignal )
{

    if(!config)
        return;

    // call the superclass to init default stuff
    CodeGenerationPolicy::setDefaults(config, false);

    // NOW block signals (because call to super-class method will leave value at "true")
    blockSignals(true); // we need to do this because otherwise most of these
    // settors below will each send the modifiedCodeContent() signal
    // needlessly (we can just make one call at the end).

    // now do java specific stuff
    config -> setGroup("Java Code Generation");

    setAttributeAccessorScope((ScopePolicy)config->readNumEntry("defaultAttributeAccessorScope",DEFAULT_ATTRIB_ACCESSOR_SCOPE));
    setAssociationFieldScope((ScopePolicy)config->readNumEntry("defaultAssocFieldScope",DEFAULT_ASSOC_FIELD_SCOPE));
    setCommentStyle((JavaCommentStyle)config->readNumEntry("commentStyle",DEFAULT_COMMENT));
    setAutoGenerateConstructors(config->readBoolEntry("autoGenEmptyConstructors",DEFAULT_AUTO_GEN_EMPTY_CONSTRUCTORS));
    setAutoGenerateAttribAccessors(config->readBoolEntry("autoGenAccessors",DEFAULT_AUTO_GEN_ATTRIB_ACCESSORS));
    setAutoGenerateAssocAccessors(config->readBoolEntry("autoGenAssocAccessors",DEFAULT_AUTO_GEN_ASSOC_ACCESSORS));

    CodeGenerator *codegen = UMLApp::app()->getGenerator();
    JavaCodeGenerator *javacodegen = dynamic_cast<JavaCodeGenerator*>(codegen);
    if (javacodegen) {
        bool mkant = config->readBoolEntry("buildANTDocument", JavaCodeGenerator::DEFAULT_BUILD_ANT_DOC);
        javacodegen->setCreateANTBuildFile(mkant);
    }

    blockSignals(false); // "as you were citizen"

    if(emitUpdateSignal)
        emit modifiedCodeContent();
}


/**
 * Create a new dialog interface for this object.
 * @return dialog object
 */
CodeGenerationPolicyPage * JavaCodeGenerationPolicy::createPage ( QWidget *parent, const char *name ) {
    return new JavaCodeGenerationPolicyPage ( parent, name, this );
}

void JavaCodeGenerationPolicy::init() {

    m_defaultAttributeAccessorScope = DEFAULT_ATTRIB_ACCESSOR_SCOPE;
    m_defaultAssociationFieldScope = DEFAULT_ASSOC_FIELD_SCOPE;
    m_commentStyle = DEFAULT_COMMENT;
    m_autoGenerateConstructors = DEFAULT_AUTO_GEN_EMPTY_CONSTRUCTORS;
    m_autoGenerateAttribAccessors = DEFAULT_AUTO_GEN_ATTRIB_ACCESSORS;
    m_autoGenerateAssocAccessors = DEFAULT_AUTO_GEN_ASSOC_ACCESSORS;

}


#include "javacodegenerationpolicy.moc"
