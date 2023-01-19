// Monitor
// WinC++ app that r/w data from/to a serial port

#include <Windows.h>
#include <iostream>

#include "Com.h"

int main(int argc, char* argv[])
{
    if (argv[1] == NULL) {
        printf_s("\n Specify a virtual serial port. \n\n");
        return 0;
    }

    char SerialBuffer[64] = {0};
    Com com = Com::Com(SerialBuffer);

    com.RegisterCom(argv[1]);

    if (com.CheckPort()) {
        printf_s("\n Port cant be opened \n\n");
        return 0;
    }

    if (com.DoGetCommState() == FALSE) {
        printf_s("\n Error to get the comm state \n\n");
        com.UnregisterCom();
        return 0;
    }

    if (com.DoSetCommState(CBR_9600, 8, ONESTOPBIT, NOPARITY) == FALSE) {
        printf_s("\n Error to setting dcb structure \n\n");
        com.UnregisterCom();
        return 0;
    }

    if (com.SetTimeouts(50, 50, 10, 50, 10) == FALSE) {
        printf_s("\nError to setting time outs\n\n");
        com.UnregisterCom();
        return 0;
    }

    if (com.SetMask(EV_RXCHAR) == FALSE) {
        printf_s("\nError to in Setting CommMask\n\n");
        com.UnregisterCom();
        return 0;
    }

    if (com.WaitComEvent(NULL) == FALSE) {
        printf_s("\nError in setting WaitCommEvent\n\n");
        com.UnregisterCom();
        return 0;
    }

    com.Print(NULL);
    com.UnregisterCom();
    return 0;
}
