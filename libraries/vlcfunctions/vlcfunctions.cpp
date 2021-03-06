#include<arduino.h>


char* interleaver(char* s, char interout[][8]){
    /* The Interleaver block rearranges the binary bits. Each character in the string is represented by 8 bit binary.
     Input to this block is string array(message) and the output will be stored in interout[][8]. We have to pass a string array
     another 2D array say for example interout[][8] while calling this function. This function returns a character pointer. */
    uint8_t a=0,b=0;

    while(*s !='\0'){

        for (b = 0; b < 8; b++) {
        interout [a][b] = !((*s << b) & 0x80);
        }
        //UARTprintf("%c",*s);
        s++;
        a++;
            }
    return s;
}


char* manchester(char* s, char interout[][8], char manout[][16]){
    /*The manchester encoding block represents binary "1" as "10" and binary 0 as "01". Input to this block is the string array(message)
    interout[][8] that is the output of interleaver block and manout[][16] to hold the output. We have to pass a string array, output of
    interleaver and an another 2D array say for example manout[][16] while calling this function. This function returns a character pointer.*/
    int i=0,j=0;

    while(*s != '\0'){

        for (j=0; j<8; j++){
            if (interout[i][j]==1){
                manout[i][2*j]=1;
                manout[i][2*j+1]=0;
            }
            else{
                manout[i][2*j]=0;
                manout[i][2*j+1]=1;
            }
        }

        s++;
        i++;
    }

    return s;
}


char* foo(char* s, char manout[][16], char final[][56]){
    /*This block is for increasing the number of ones. each binary "1" is represented as "111" and binary "0" is represented as "0".
     Input to this block is the string array(message),manout[][16] that is the output of manchester encoding block and final[][32] to hold the output.
     We have to pass a string array, output of manchester and an another 2D array say for example final[][32] while calling this function.
     This function returns a character pointer.*/
    int i=0,j=0,k=0;


    while(*s != '\0'){
        k=0;
        for (j=0; j<16; j++){
            if (manout[i][j]==1){
                final[i][k]=1;
                final[i][k+1]=1;
                final[i][k+2]=1;
                final[i][k+3]=1;
                final[i][k+4]=1;
                final[i][k+5]=1;
                k+=6;
            }
            else{
                final[i][k]=0;
                k++;
            }
        }
        s++;
        i++;
    }

    return s;
}

void send_vlc_data(char* dt, char final[][56], int VLC_MODULATION_PIN)
{

    int i=0,j=0;
//        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6, GPIO_PIN_6);
    digitalWrite(VLC_MODULATION_PIN, HIGH);
    delayMicroseconds(150);
    while(*dt++ != '\0'){
        for (j = 0; j < 56; j++) {
            if (final[i][j]==1){
                        //GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6, 0);
                        digitalWrite(VLC_MODULATION_PIN, LOW);
                        delayMicroseconds(50);
//                        delay(500);
                    }
                    else if(final[i][j]==0){

                        //GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6, GPIO_PIN_6);
                        digitalWrite(VLC_MODULATION_PIN, HIGH);
                        delayMicroseconds(50);
//                        delay(500);

                    }
                }
            i++;
    }
    digitalWrite(VLC_MODULATION_PIN, HIGH);
    delayMicroseconds(150);
    digitalWrite(VLC_MODULATION_PIN, LOW);

}
