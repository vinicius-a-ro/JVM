/*
 * formatoClass.c
 *
 *  Created on: 21/12/2010
 *      Author: joaofreitas
 */


#ifndef CLASSFILEREADER_H
#define	CLASSFILEREADER_H

#include <stdio.h>
#include <stdlib.h>
#include "../structures/classFileStructures.h"

u1 u1Read(FILE *fp);

u2 u2Read(FILE *fp);

u4 u4Read(FILE *fp);

#endif
