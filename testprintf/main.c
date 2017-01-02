/*
 * main.c
 *
 *  Created on: 2016. 11. 10.
 *      Author: josco
 */


#include <stdio.h>

typedef unsigned int uint;

#define FW_VER		"PA_V1.5"

char buf[80];
char strVersion[5];

int main(void)
{
	int n_i;
	char *strFW_VER = FW_VER;
	uint uiSensorVtg = 5045;
/*	uint uiVersion = 0;
*/
	if (uiSensorVtg > 9999) {
		uiSensorVtg = 0;
	}

	for (n_i=0; n_i < 4; n_i++) {
		strVersion[n_i] = '0';
	}
	strVersion[n_i] = '\0';

	strVersion[1] = strFW_VER[4];
	strVersion[2] = strFW_VER[6];

	sprintf(buf, "C%s0%04uE00\n", strVersion, uiSensorVtg);
	printf(buf);

	return(0);
}
