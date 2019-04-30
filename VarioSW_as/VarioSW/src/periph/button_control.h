#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define PRESS 4

#include "../definitions.h"



class BUTTONCONTROL
{
  public:
    void buttonUpdate();
    int getButtonPressed();
    int getFlag();
  private:
    int wasPressed = 0;
    int flag = 0;
	int index = 0;
    int buttonPressed[5] = {0, 0, 0, 0, 0};
    int block = 0;
};

extern BUTTONCONTROL buttons;