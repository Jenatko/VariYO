/*
 * CicleFitByTaubin.h
 *
 * Created: 15.3.2019 22:29:52
 *  Author: Jena
 */ 


#ifndef CICLEFITBYTAUBIN_H_
#define CICLEFITBYTAUBIN_H_

#include "circle.h"
#include "data.h"
#include "mystuff.h"


reals Sigma (Data& data, Circle& circle);
Circle CircleFitByTaubin (Data& data);

#endif /* CICLEFITBYTAUBIN_H_ */