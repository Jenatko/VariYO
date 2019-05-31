/*
 * ToDo.h
 *
 * Created: 18.5.2019 21:59:51
 *  Author: Jena
 */ 


#ifndef TODO_H_
#define TODO_H_

/*
AGL data interpolation, buffer in RAM so not reading from SD if same location, add support to 30m mesh data (probably can be determined by file size)
printing to display is SLOOOOOOOOWWWWW!!!!
nothing with bluetooth is done yet
some XContest compatible tracklog format (or convert on pc?)
display heating  --partially done, maybe some automatic mode?
ellipoid fit, correction of magnetometer and accelerometer (offset and gain)
airspaces? there is some open airspace format, maybe with that
alarm, stopwatch, countdown timer
chess is super lame, maybe some exit button, not returning when king is taken:-)
temperature/humdity logger
gps to galileo instead of beidou?
turning off all the unnecessary shit in microcontroller would be handy to reduce consumption
check why MAX17055 doesnt indicate SOC 100%
altitude above take-off
compass (indicating towards horizon, yaw indicates top of vario, shows bullshit when vertically on riser)
SW reset
adjustable equation for buzzer (that quadratic a^2 * vario + b*vario + c = buzzer frequency)?
gauges for all the important stuff + option to edit them
some font with less horizontal spacing (biggest one takes fucktone of space)
full refresh from time to time
adujstable size of buffer for calculating wind, somehow limit it for reasonable values
option to skip writing of values to tracklog if velocity, speed and vario the same (tracklog gets crazy big, and these points add no additional info when moving on the straight line, only first and last point sufficient)
tracklog debug on/off option for writing all these <note> values
more "desktops"?
dark mode?
graph of altitude in past xy minutes
if display can be made to updated faster, maybe update vario info twice a second?
menu hierarchy is slowly loosing any sense:-)
numpad can not return floats (even though there is decimal point prepared:-)), can overflow, maybe could have parameter to show units (eg when putting sea level pressure, there could be "Pa" shown, so you dont put there hPa)
normal keyboard as input method, if gauges are in a state that they can be editted in vario (to write custom gauge header, units etc)
having menu that can have some items not detailed and some items detailed would be fucking awesome





*/


#endif /* TODO_H_ */