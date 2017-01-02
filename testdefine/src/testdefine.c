/*
 ============================================================================
 Name        : testdefine.c
 Author      : jccho@fine-tec.com
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "b.h"


int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}

void setaddr(int a)
{
	addr = a;
}

