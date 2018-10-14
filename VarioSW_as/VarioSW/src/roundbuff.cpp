#include "roundbuff.h"

RBUFF rbuff;

void RBUFF::putIn(unsigned int number) {
  //void RBUFF::putIn(float number) {
  buff_old[i] = buff[i];
  buff[i] = number;;
  i++;
  if (i >= RBUFF_LENGTH)
    i = 0;
}


unsigned int RBUFF::getOut(int number) {
  return buff[number];
}
unsigned int RBUFF::getAverage() {
  unsigned int avg = 0;
  for (int j = 0; j < RBUFF_LENGTH; j++)
    avg += buff[j];

  return avg / RBUFF_LENGTH;

}

unsigned int RBUFF::getAverageOld() {
  unsigned int avg = 0;
  for (int j = 0; j < RBUFF_LENGTH; j++)
    avg += buff_old[j];

  return avg / RBUFF_LENGTH;

}
