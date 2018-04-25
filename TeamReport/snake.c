/*-------------------include--------------------*/
#include <msp430.h>
#include "msp430g2553.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/*------------------Definitions-------------------*/
#define CLOCK_ON_ROW    P2OUT = P2OUT|(BIT1)
#define CLOCK_OFF_ROW   P2OUT = P2OUT&(~(BIT1))
#define LATCH_ON_ROW    P2OUT = P2OUT|(BIT2)
#define LATCH_OFF_ROW   P2OUT = P2OUT&(~(BIT2))

#define LATCH_ON_COL    P2OUT = P2OUT|(BIT3)
#define LATCH_OFF_COL   P2OUT = P2OUT&(~(BIT3))
#define CLOCK_ON_COL    P2OUT = P2OUT|(BIT4)
#define CLOCK_OFF_COL   P2OUT = P2OUT&(~(BIT4))
/*-------------------Macros------------------------*/
int Mode_Interrupt = 3;
extern void thong_so_ban_dau(void);
extern void trai(void);
extern void phai(void);
extern void xuong(void);

extern void len(void);
int DataTransfer595(unsigned char RowData6,unsigned char RowData5,unsigned char RowData4,unsigned char RowData3,unsigned char RowData2,unsigned char RowData1,unsigned char ColData);
extern void sangdau_tatduoi(void);

unsigned char Row,Colum, leng;
unsigned char bottom, die;
unsigned char dinh_vi0[6],dinh_vi1[6],dinh_vi2[6],dinh_vi3[6],dinh_vi4[6],dinh_vi5[6],dinh_vi6[6],dinh_vi7[6];
unsigned char dinh_vi0[6]={0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dinh_vi1[6]={0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dinh_vi2[6]={0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dinh_vi3[6]={0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dinh_vi4[6]={0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dinh_vi5[6]={0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dinh_vi6[6]={0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dinh_vi7[6]={0x00,0x00,0x00,0x00,0x00,0x00};
int dau[2], duoi[2];
unsigned char con_ran[2][30];

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer

    BCSCTL1 &=~ BIT2;                       //| RSELx = 11; DCOx = 3 ====>    f = 4.25 MHz  //|
    BCSCTL1 |=  (BIT0 + BIT1 + BIT3);       //|                                             //|
    DCOCTL  &=~ BIT7;                       //|                                             //|
    DCOCTL  |=  (BIT6 + BIT7);              //|---------------------------------------------//|

   // P1DIR |= BIT6;  P1OUT |= BIT6;
    P2DIR |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);
    P2OUT &=~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);
    LATCH_OFF_ROW;
    LATCH_OFF_COL;

     P1DIR&=~(BIT0 + BIT1 + BIT2 + BIT3);
     P1REN|=(BIT0 + BIT1 + BIT2 + BIT3);
     P1OUT|=(BIT0 + BIT1 + BIT2 + BIT3);

     P1IE |=(BIT0 + BIT1 + BIT2 + BIT3);
     P1IES |=(BIT0 + BIT1 + BIT2 + BIT3);

     P1IFG &=~(BIT0 + BIT1 + BIT2 + BIT3);
     __bis_SR_register(GIE);
        int i,j;
        unsigned char COL[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
      thong_so_ban_dau();
        while(1)
        {
        switch(bottom)
            {
            case 1:
                trai();
                break;
            case 2:
                xuong();
                break;
            case 3:
                phai();
                break;
            case 4:
                len();
                break;
            }
            sangdau_tatduoi();
            for(i=0; i<100; i++){
            for(j=0; j<8; j++){
                switch (j){
                case 0:
                    DataTransfer595(dinh_vi0[5],dinh_vi0[4],dinh_vi0[3],dinh_vi0[2],dinh_vi0[1],dinh_vi0[0],COL[0]); break;
                case 1:
                    DataTransfer595(dinh_vi1[5],dinh_vi1[4],dinh_vi1[3],dinh_vi1[2],dinh_vi1[1],dinh_vi1[0],COL[1]); break;
                case 2:
                    DataTransfer595(dinh_vi2[5],dinh_vi2[4],dinh_vi2[3],dinh_vi2[2],dinh_vi2[1],dinh_vi2[0],COL[2]); break;
                case 3:
                    DataTransfer595(dinh_vi3[5],dinh_vi3[4],dinh_vi3[3],dinh_vi3[2],dinh_vi3[1],dinh_vi3[0],COL[3]); break;
                case 4:
                    DataTransfer595(dinh_vi4[5],dinh_vi4[4],dinh_vi4[3],dinh_vi4[2],dinh_vi4[1],dinh_vi4[0],COL[4]); break;
                case 5:
                    DataTransfer595(dinh_vi5[5],dinh_vi5[4],dinh_vi5[3],dinh_vi5[2],dinh_vi5[1],dinh_vi5[0],COL[5]); break;
                case 6:
                    DataTransfer595(dinh_vi6[5],dinh_vi6[4],dinh_vi6[3],dinh_vi6[2],dinh_vi6[1],dinh_vi6[0],COL[6]); break;
                case 7:
                    DataTransfer595(dinh_vi7[5],dinh_vi7[4],dinh_vi7[3],dinh_vi7[2],dinh_vi7[1],dinh_vi7[0],COL[7]); break;
                }
            }
       }
}
}

//===========================FUNCTIONS======================
extern void thong_so_ban_dau(void)
{
    die=0;
    dinh_vi1[3]=0x80;
    dau[0]=8;
    dau[1]=3;
    con_ran[0][0]=8;
    con_ran[1][0]=3;
    con_ran[0][1]=8;
    con_ran[1][1]=4;
    Row=0;
    Colum=2;
    bottom=1;
    leng=2;
    dinh_vi1[3]&=~(0x01<<(7));
    if(((Row+1)%8)!=0){
        switch(Colum)
        {
        case 0: dinh_vi0[(Row+1)/8]|=(0x01<<(Row%8)); break;
        case 1: dinh_vi1[(Row+1)/8]|=(0x01<<(Row%8)); break;
        case 2: dinh_vi2[(Row+1)/8]|=(0x01<<(Row%8)); break;
        case 3: dinh_vi3[(Row+1)/8]|=(0x01<<(Row%8)); break;
        case 4: dinh_vi4[(Row+1)/8]|=(0x01<<(Row%8)); break;
        case 5: dinh_vi5[(Row+1)/8]|=(0x01<<(Row%8)); break;
        case 6: dinh_vi6[(Row+1)/8]|=(0x01<<(Row%8)); break;
        case 7: dinh_vi7[(Row+1)/8]|=(0x01<<(Row%8)); break;
        }}
    else {
        switch(Colum)
        {
        case 0: dinh_vi0[(Row+1)/8-1]|=0x01; break;
        case 1: dinh_vi1[(Row+1)/8-1]|=0x01; break;
        case 2: dinh_vi2[(Row+1)/8-1]|=0x01; break;
        case 3: dinh_vi3[(Row+1)/8-1]|=0x01; break;
        case 4: dinh_vi4[(Row+1)/8-1]|=0x01; break;
        case 5: dinh_vi5[(Row+1)/8-1]|=0x01; break;
        case 6: dinh_vi6[(Row+1)/8-1]|=0x01; break;
        case 7: dinh_vi7[(Row+1)/8-1]|=0x01; break;
        }
        }
}

extern void trai(void)   //bottom=1
{
    dau[1]=dau[1]-1;
    if((dau[0]>=0)&(dau[0]<=7)&(dau[1]==-1)){
        dau[0]=dau[0]+16;
        dau[1]=7;}
    if((dau[0]>=8)&(dau[0]<=23)&(dau[1]==-1)){
        dau[0]=dau[0]-8;
        dau[1]=7;}
    if((dau[0]>=24)&(dau[0]<=31)&(dau[1]==-1)){
        dau[0]=dau[0]+16;
        dau[1]=7;}
    if((dau[0]>=32)&(dau[0]<=47)&(dau[1]==-1)){
        dau[0]=dau[0]-8;
        dau[1]=7;}
}
extern void phai(void)   //bottom=1
{
    dau[1]=dau[1]+1;
    if((dau[0]>=0)&(dau[0]<=15)&(dau[1]==8)){
        dau[0]=dau[0]+8;
        dau[1]=0;}
    if((dau[0]>=16)&(dau[0]<=23)&(dau[1]==8)){
        dau[0]=dau[0]-16;
        dau[1]=0;}
    if((dau[0]>=24)&(dau[0]<=39)&(dau[1]==8)){
        dau[0]=dau[0]+8;
        dau[1]=0;}
    if((dau[0]>=40)&(dau[0]<=47)&(dau[1]==8)){
        dau[0]=dau[0]-16;
        dau[1]=0;}
}
extern void xuong(void)   //bottom=1
{
    dau[0]=dau[0]+1;
    if((dau[0]==8)|(dau[0]==16)|(dau[0]==24))
        dau[0]=dau[0]+16;
    else if((dau[0]==32)|(dau[0]==40)|(dau[0]==48))
        dau[0]=dau[0]-32;
}
extern void len(void)   //bottom=1
{
    dau[0]=dau[0]-1;
    if((dau[0]==-1)|(dau[0]==7)|(dau[0]==15))
        dau[0]=dau[0]+32;
    else if((dau[0]==23)|(dau[0]==31)|(dau[0]==39))
        dau[0]=dau[0]-16;
}
extern void sangdau_tatduoi(void){
    unsigned int i;
    int a=con_ran[0][0], b=con_ran[1][0], c, d;
    con_ran[0][0]=dau[0];
    con_ran[1][0]=dau[1];

    for(i=0; i<leng-1; i++){
        c=con_ran[0][i+1];
        d=con_ran[1][i+1];
        con_ran[0][i+1]=a;
        con_ran[1][i+1]=b;
        a=c;
        b=d;
    }
    if(((dau[0]+1)%8)!=0){
                switch(dau[1])
                {
                case 0: dinh_vi0[(dau[0]+1)/8]|=(0x01<<(dau[0]%8)); break;
                case 1: dinh_vi1[(dau[0]+1)/8]|=(0x01<<(dau[0]%8)); break;
                case 2: dinh_vi2[(dau[0]+1)/8]|=(0x01<<(dau[0]%8)); break;
                case 3: dinh_vi3[(dau[0]+1)/8]|=(0x01<<(dau[0]%8)); break;
                case 4: dinh_vi4[(dau[0]+1)/8]|=(0x01<<(dau[0]%8)); break;
                case 5: dinh_vi5[(dau[0]+1)/8]|=(0x01<<(dau[0]%8)); break;
                case 6: dinh_vi6[(dau[0]+1)/8]|=(0x01<<(dau[0]%8)); break;
                case 7: dinh_vi7[(dau[0]+1)/8]|=(0x01<<(dau[0]%8)); break;
                }}
            else {
                switch(dau[1])
                {
                case 0: dinh_vi0[(dau[0]+1)/8-1]|=0x80; break;
                case 1: dinh_vi1[(dau[0]+1)/8-1]|=0x80; break;
                case 2: dinh_vi2[(dau[0]+1)/8-1]|=0x80; break;
                case 3: dinh_vi3[(dau[0]+1)/8-1]|=0x80; break;
                case 4: dinh_vi4[(dau[0]+1)/8-1]|=0x80; break;
                case 5: dinh_vi5[(dau[0]+1)/8-1]|=0x80; break;
                case 6: dinh_vi6[(dau[0]+1)/8-1]|=0x80; break;
                case 7: dinh_vi7[(dau[0]+1)/8-1]|=0x80; break;
                }
                }
    if ((dau[0]==Row)&(dau[1]==Colum)){
        leng=leng+1;
        con_ran[0][leng-1]=c;
        con_ran[1][leng-1]=d;
        srand(time(NULL));
        Row=rand() % 48;
        Colum=rand() % 8;
        for(i=0; i<leng; i++){
        while(1){
        if((Row==con_ran[0][i])&(Colum==con_ran[1][i])){
            srand(time(NULL));
            Row=rand() % 48;
            Colum=rand() % 8;
        }
        else break;
        }
        }
        if(((Row+1)%8)!=0){
            switch(Colum)
            {
            case 0: dinh_vi0[(Row+1)/8]|=(0x01<<(Row%8)); break;
            case 1: dinh_vi1[(Row+1)/8]|=(0x01<<(Row%8)); break;
            case 2: dinh_vi2[(Row+1)/8]|=(0x01<<(Row%8)); break;
            case 3: dinh_vi3[(Row+1)/8]|=(0x01<<(Row%8)); break;
            case 4: dinh_vi4[(Row+1)/8]|=(0x01<<(Row%8)); break;
            case 5: dinh_vi5[(Row+1)/8]|=(0x01<<(Row%8)); break;
            case 6: dinh_vi6[(Row+1)/8]|=(0x01<<(Row%8)); break;
            case 7: dinh_vi7[(Row+1)/8]|=(0x01<<(Row%8)); break;
            }}
        else {
            switch(Colum)
            {
            case 0: dinh_vi0[(Row+1)/8-1]|=0x80; break;
            case 1: dinh_vi1[(Row+1)/8-1]|=0x80; break;
            case 2: dinh_vi2[(Row+1)/8-1]|=0x80; break;
            case 3: dinh_vi3[(Row+1)/8-1]|=0x80; break;
            case 4: dinh_vi4[(Row+1)/8-1]|=0x80; break;
            case 5: dinh_vi5[(Row+1)/8-1]|=0x80; break;
            case 6: dinh_vi6[(Row+1)/8-1]|=0x80; break;
            case 7: dinh_vi7[(Row+1)/8-1]|=0x80; break;
            }
            }
    }
    else {
    duoi[0]=c;
    duoi[1]=d;
    if(((duoi[0]+1)%8)!=0){
        switch(duoi[1])
        {
        case 0: dinh_vi0[((duoi[0]+1)/8)]&=~(0x01<<((duoi[0])%8)); break;
        case 1: dinh_vi1[((duoi[0]+1)/8)]&=~(0x01<<((duoi[0])%8)); break;
        case 2: dinh_vi2[((duoi[0]+1)/8)]&=~(0x01<<((duoi[0])%8)); break;
        case 3: dinh_vi3[((duoi[0]+1)/8)]&=~(0x01<<((duoi[0])%8)); break;
        case 4: dinh_vi4[((duoi[0]+1)/8)]&=~(0x01<<((duoi[0])%8)); break;
        case 5: dinh_vi5[((duoi[0]+1)/8)]&=~(0x01<<((duoi[0])%8)); break;
        case 6: dinh_vi6[((duoi[0]+1)/8)]&=~(0x01<<((duoi[0])%8)); break;
        case 7: dinh_vi7[((duoi[0]+1)/8)]&=~(0x01<<((duoi[0])%8)); break;
        }}
    else {
        switch(duoi[1])
        {
        case 0: dinh_vi0[(duoi[0]+1)/8-1]&=~0x80; break;
        case 1: dinh_vi1[(duoi[0]+1)/8-1]&=~0x80; break;
        case 2: dinh_vi2[(duoi[0]+1)/8-1]&=~0x80; break;
        case 3: dinh_vi3[(duoi[0]+1)/8-1]&=~0x80; break;
        case 4: dinh_vi4[(duoi[0]+1)/8-1]&=~0x80; break;
        case 5: dinh_vi5[(duoi[0]+1)/8-1]&=~0x80; break;
        case 6: dinh_vi6[(duoi[0]+1)/8-1]&=~0x80; break;
        case 7: dinh_vi7[(duoi[0]+1)/8-1]&=~0x80; break;
        }
        }
    }
//  gameover:
}


//=================================================================
int DataTransfer595(unsigned char RowData6,unsigned char RowData5,unsigned char RowData4,unsigned char RowData3,unsigned char RowData2,unsigned char RowData1,unsigned char ColData)
{
    /* MCU -> 74hc595 : Truyen du lieu */
        int i;
        unsigned char mask_row = 0x01;
        unsigned char data_dich, data_mask;
        /* Truyen data cho Row*/
            LATCH_OFF_ROW;
            for (i=0;i<8;i++)
                {
                    CLOCK_OFF_ROW;
                    data_dich = RowData1 >> i;
                    data_mask = data_dich & mask_row;
                    P2OUT &=~ BIT0;                 // Dua du lieu ra P2.0
                    P2OUT |= data_mask;             // Dua du lieu ra P2.0
                    CLOCK_ON_ROW;
                }
            for (i=0;i<8;i++)
                {
                    CLOCK_OFF_ROW;
                    data_dich = RowData2 >> i;
                    data_mask = data_dich & mask_row;
                    P2OUT &=~ BIT0;                 // Dua du lieu ra P2.0
                    P2OUT |= data_mask;             // Dua du lieu ra P2.0
                    CLOCK_ON_ROW;
                }

            for (i=0;i<8;i++)
                {
                    CLOCK_OFF_ROW;
                    data_dich = RowData3 >> i;
                    data_mask = data_dich & mask_row;
                    P2OUT &=~ BIT0;                 // Dua du lieu ra P2.0
                    P2OUT |= data_mask;             // Dua du lieu ra P2.0
                    CLOCK_ON_ROW;
                }
            for (i=0;i<8;i++)
                            {
                                CLOCK_OFF_ROW;
                                data_dich = RowData4 >> i;
                                data_mask = data_dich & mask_row;
                                P2OUT &=~ BIT0;                 // Dua du lieu ra P2.0
                                P2OUT |= data_mask;             // Dua du lieu ra P2.0
                                CLOCK_ON_ROW;
                            }
            for (i=0;i<8;i++)
                            {
                                CLOCK_OFF_ROW;
                                data_dich = RowData5 >> i;
                                data_mask = data_dich & mask_row;
                                P2OUT &=~ BIT0;                 // Dua du lieu ra P2.0
                                P2OUT |= data_mask;             // Dua du lieu ra P2.0
                                CLOCK_ON_ROW;
                            }
            for (i=0;i<8;i++)
                            {
                                CLOCK_OFF_ROW;
                                data_dich = RowData6 >> i;
                                data_mask = data_dich & mask_row;
                                P2OUT &=~ BIT0;                 // Dua du lieu ra P2.0
                                P2OUT |= data_mask;             // Dua du lieu ra P2.0
                                CLOCK_ON_ROW;
                            }

            CLOCK_OFF_ROW;
            //-----------
            for (i=0;i<8;i++)
                        {
                            CLOCK_OFF_COL;
                            data_dich = ColData >> i;
                            data_mask = data_dich & mask_row;
                            unsigned char DATAA = data_mask << 5;
                            P2OUT &=~ BIT5;                 // Dua du lieu ra P2.0
                            P2OUT |= DATAA;             // Dua du lieu ra P2.0
                            CLOCK_ON_COL;
                                                    }
            CLOCK_OFF_COL;
            LATCH_ON_COL;
            LATCH_OFF_COL;


            LATCH_ON_ROW;
            LATCH_OFF_ROW;
        return(0);
}

#pragma vector = PORT1_VECTOR
__interrupt void Button_Interrupt(void)
{
    if(P1IFG&BIT0)
    {
      bottom=1;
      P1IFG &=~BIT0;
    }
    if(P1IFG&BIT1)
    {
      bottom=4;
      P1IFG &=~BIT1;
    }
    if(P1IFG&BIT2)
    {
      bottom=3;
      P1IFG &=~BIT2;
    }
    if(P1IFG&BIT3)
    {
      bottom=2;
      P1IFG &=~BIT3;
    }
}

