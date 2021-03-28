#include "mbed.h"
#include "uLCD_4DGL.h"

#define Ratio (0.91)

uLCD_4DGL uLCD(D1, D0, D2);
AnalogOut Aout(PA_4);
AnalogIn Ain(A0);

DigitalIn upBtn(D8);
DigitalIn dwBtn(D9);
DigitalIn selBtn(D10);
DigitalOut led1(LED1);

//uLCD setting and data
int a[2] = {72, 36};
int arrow = 0;
char s[2][5] = {"<--\n", "    "};

// argument for different freq
int us[2] = {1300/2, 1300};
int cycles[2] = {72, 36};
int sampleRate[2] = {2880, 1440};

float ADCdata[3800];
void genWav(int us)
{
    double v = 0;
    int i, j;
    int s = 0;
    int id = 0;

    int total = 0;

    printf("%d\n", sampleRate[arrow]);
    
    for (j = 0; j < cycles[arrow]; j++) {
        for (i = 0; i < 20; i++) {
            total++;
            if (id < 3800) {
                ADCdata[id] = Ain; 
                s = 0;
                id++;
            }
            wait_us(us/2);

            if (i <= 17) {
                v = Ratio * (1.0/17) * i;
            } else {
                v = Ratio * (1 - 0.5 * (i - 17));
            }

            total++;
            if (id < 3800) {
                ADCdata[id] = Ain; 
                s = 0;
                id++;
            }

            Aout = v;
            wait_us(us/2);
            //ThisThread::sleep_for(1ms);
        }
    }
    for (int i = 0; i < id; i++) {
        printf("%f\n", ADCdata[i]);
    }
        printf("idMax:%d\n", id);
        printf("total:%d\n", total);

}
void arrowUp()
{
    if(arrow == 1) arrow = 0;
    else arrow++;
}
void arrowDw()
{
    if(arrow == 0) arrow = 1;
    else arrow--;
}
void menuArrowUpdate()
{
    uLCD.locate(10,1);
    uLCD.printf("%s", s[arrow]);
    uLCD.locate(10,2);
    uLCD.printf("%s", s[!arrow]);
}



int main() {
    uLCD.locate(1,1);
    uLCD.printf("%dHz\n", a[0]);
    uLCD.locate(1,2);
    uLCD.printf("%dHz\n", a[1]);
    menuArrowUpdate();

    while (1){
        if (upBtn.read()) {
            arrowUp();
            menuArrowUpdate();
            ThisThread::sleep_for(500ms);
        }
        if (dwBtn.read()) {
            arrowDw();
            menuArrowUpdate();
            ThisThread::sleep_for(500ms);
        }
        if (selBtn.read()) {
            ThisThread::sleep_for(500ms);
            genWav(us[arrow]);
        }
    }
}
