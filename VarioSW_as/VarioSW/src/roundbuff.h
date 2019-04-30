#define RBUFF_LENGTH  5


class RBUFF
{
  public:

    unsigned int buff[RBUFF_LENGTH];
    unsigned int buff_old[RBUFF_LENGTH];
    
    void putIn(unsigned int number);
    unsigned int getOut(int number);
    unsigned int getAverage();
    unsigned int getAverageOld();
  private:
    int i = 0;
};

extern RBUFF rbuff;
