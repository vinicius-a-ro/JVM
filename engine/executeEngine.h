/*
 * exec.h
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#ifndef EXEC_H_
#define EXEC_H_

#include "../structures/methodArea.h"
#include "../structures/heap.h"
#include "../structures/frame.h"
#include "../structures/classFileStructures.h"
#include "../reader/classLoader.h"
#include <stdlib.h>

method_area *m_area;

void exec(classFileFormat *, char *path);

void saveFilePath(char *classPath);

class_struct *getSymbolicReferenceClass(char *class_name);

field_info *getResolvedFieldReference(class_struct *cl, cp_info fieldref);

#endif /* EXEC_H_ */
