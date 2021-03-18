/*
 * SD_statvar.h
 *
 * Created: 18.3.2021 21:52:27
 *  Author: Jena
 */ 

#include "Variables.h"



#ifndef SD_STATVAR_H_
#define SD_STATVAR_H_

void save_statvar();
void load_statvar();

int load_statvar_SD(StaticVariables &param);
int save_statvar_SD(StaticVariables &param);


void setVariablesDefault();

#endif /* SD_STATVAR_H_ */