/*
 * Gauge.h
 *
 * Created: 20/09/2019 13:00:05
 *  Author: tcwrhb
 */ 



#ifndef GAUGE_H_
#define GAUGE_H_

#include "Variables.h"

void updateGauge(Gauge* gaugepointer, float value);

	void printGauges();
	void printGauges_frames();
	void printGauges_values();
	
	void Gauge_print(Gauge *gau, int inverse_value = 0);
	void Gauge_print_value(Gauge *gau, int inverse_value = 0);
		void Gauge_print_frame(Gauge *gau, int inverse_value = 0);


#endif /* GAUGE_H_ */