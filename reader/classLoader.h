/*
 * ClassLoader.h
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */

#ifndef CLASSLOADER_H_
#define CLASSLOADER_H_

#include <stdio.h>
#include <stdlib.h>

#include "../structures/classFileStructures.h"
#include "classFileReader.h"
#include "attribute/attributeUtil.h"

classFileFormat* loadClassFile(const char *arquivo);

void readMagicNumber(classFileFormat *classFile, FILE *fp);

void readVersion(classFileFormat *classFile, FILE *fp);

void readConstantPoolCount(classFileFormat *classFile, FILE *fp);

void readConstantPool(classFileFormat *classFile, FILE *fp);

void readAccessFlags(classFileFormat *classFile, FILE *fp);

void readThisClass(classFileFormat *classFile, FILE *fp);

void readSuperClass(classFileFormat *classFile, FILE *fp);

void readInterfaceCount(classFileFormat *classFile, FILE *fp);

void readInterface(classFileFormat *classFile, FILE *fp);

void readFieldsCount(classFileFormat *classFile, FILE *fp);

void readFields(classFileFormat *classFile, FILE *fp);

void readMethodsCount(classFileFormat *classFile, FILE *fp);

void readMethods(classFileFormat *classFile, FILE *fp);

void readAttributesCount(classFileFormat *classFile, FILE *fp);

void readAttributes(classFileFormat *classFile, FILE *fp);

cp_info getConstantPoolElementByIndex(classFileFormat *classFile, int index);

#endif /* CLASSLOADER_H_ */
