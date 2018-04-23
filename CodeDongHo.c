

//
//                MSP430g2553
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P2.0|-->Data            Row
//            |             p2.1|-->clock              Row
//            |             p2.2|-->latch          Row
//                          p2.3    Latch           Col
//                          p2.4    Clock           Col
//                          p2.5    Data            Col
//
//
//-=================================Include=============================
#include <../../msp430.h>
//===================================Define===============================
#define CLOCK_ON_ROW    P2OUT = P2OUT|(BIT1)
#define CLOCK_OFF_ROW   P2OUT = P2OUT&(~(BIT1))
#define LATCH_ON_ROW    P2OUT = P2OUT|(BIT2)
#define LATCH_OFF_ROW   P2OUT = P2OUT&(~(BIT2))

#define LATCH_ON_COL    P2OUT = P2OUT|(BIT3)
#define LATCH_OFF_COL   P2OUT = P2OUT&(~(BIT3))
#define CLOCK_ON_COL    P2OUT = P2OUT|(BIT4)
#define CLOCK_OFF_COL   P2OUT = P2OUT&(~(BIT4))
//====================================Macros===============================
int DataTransfer595(unsigned char RowData6,unsigned char RowData5,unsigned char RowData4,unsigned char RowData3,unsigned char RowData2,unsigned char RowData1,unsigned char ColData);
/*----------------Time And Date------------------*/
int HangChuc_1(int k); int HangDonVi_1(int k);
int HangChuc_2(int k); int HangDonVi_2(int k);

int setHour(int Count_Button_F); int setMinute(int Count_Button_F);
int setDay(int Count_Button_F);  int setMonth(int Count_Button_F);

void    Check_Time();int     Day_Display();int    Time_Display();
/*------------------Baothuc----------------------*/
int     Baothuc(); int     Dieu_Kien_Bao_Thuc(); int     SetAlarm();
void Buzzer();
int Mode_Interrupt = 0;

//=========================Globle Variable==========================
   /*---------------Date And Time-------------------*/
   int Mode; const int DayMode = 1; const int TimeMode = 2;
   unsigned char Data_LED1[8],Data_LED2[8],Data_LED3[8];

   unsigned char LEDMatrix_3[8]; unsigned char LEDMatrix_2[8]; unsigned char LEDMatrix_1[8];

   unsigned int Count;int Count_Second = 0; int Count_Minute = 0; int Set_Next_Minute = 0;
   unsigned int Button_Count = 0;  unsigned int Exit = 0;
   unsigned int Hour,Minute,Day,Month;
   /*------------Baothuc------------------*/
   int Speaker_Up = 0; int EndBaothuc = 1; int Baothuc_Mode = 0;
   unsigned int Hour_Alarm, Minute_Alarm, Exit_Alarm;
//============================Table===================================
   /*---------------Date And Time------------------*/
    //Number Table 7x5
           unsigned char Number_0[5]={0x7C,0xA2,0x92,0x8A,0x7C};
           unsigned char Number_1[5]={0x00,0x84,0xFE,0x80,0x00};
           unsigned char Number_2[5]={0x84,0xC2,0xA2,0x92,0x8C};
           unsigned char Number_3[5]={0x42,0x82,0x8A,0x96,0x62};
           unsigned char Number_4[5]={0x30,0x28,0x24,0xFE,0x20};
           unsigned char Number_5[5]={0x4E,0x8A,0x8A,0x8A,0x72};
           unsigned char Number_6[5]={0x78,0x94,0x92,0x92,0x60};
           unsigned char Number_7[5]={0x02,0xE2,0x12,0x0A,0x06};
           unsigned char Number_8[5]={0x6C,0x92,0x92,0x92,0x6C};
           unsigned char Number_9[5]={0x0C,0x92,0x92,0x52,0x3C};

           //Number Table 7x4
           unsigned char x4_Number_0[4]={0x7C,0xA2,0x92,0x7C};
           unsigned char x4_Number_1[4]={0x84,0xFE,0x80,0x00};
           unsigned char x4_Number_2[4]={0xF2,0x92,0x92,0x9E};
           unsigned char x4_Number_3[4]={0x92,0x92,0x92,0xFE};
           unsigned char x4_Number_4[4]={0x1E,0x10,0x10,0xFE};
           unsigned char x4_Number_5[4]={0x9E,0x92,0x92,0xF2};
           unsigned char x4_Number_6[4]={0xFE,0x92,0x92,0xF2};
           unsigned char x4_Number_7[4]={0x06,0x02,0x02,0xFE};
           unsigned char x4_Number_8[4]={0xFE,0x92,0x92,0xFE};
           unsigned char x4_Number_9[4]={0x9E,0x92,0x92,0xFE};

           //Time Table
           unsigned char Time_LEDMatrix_1[8]={0x00,0x08,0xF8,0x08,0x00,0xF8,0x00,0xF8};
           unsigned char Time_LEDMatrix_2[8]={0x10,0x20,0x10,0xF8,0x00,0xF8,0xA8,0xA8};
           unsigned char Time_LEDMatrix_3[8]={0x00,0x00,0xFE,0x82,0xBA,0xA2,0x82,0xFE};

           //Date Table
           unsigned char Date_LEDMatrix_1[8]={0x00,0xFC,0x84,0x78,0x00,0xF8,0x24,0x24};
           unsigned char Date_LEDMatrix_2[8]={0xF8,0x00,0x04,0xFC,0x04,0x00,0xFC,0x94};
           unsigned char Date_LEDMatrix_3[8]={0x94,0x00,0xFE,0xFE,0xC6,0xDE,0xFE,0xFE};

           unsigned char COL[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    /*---------------------Baothuc----------------------*/
              //Nhap Gio_Alarm
              unsigned char Nhap_Gio_Alarm1[8]={0xF8,0x24,0x24,0xF8,0x00,0xF8,0x80,0x80};
              unsigned char Nhap_Gio_Alarm2[8]={0x00,0xE8,0xA8,0xF8,0x00,0xF8,0x10,0x20};
              unsigned char Nhap_Gio_Alarm3[8]={0x10,0xF8,0x00,0x80,0x00,0xF8,0x20,0xF8};
              //Nhap Phut_Alarm
              unsigned char Nhap_Phut_Alarm1[8]={0xF8,0x24,0x24,0xF8,0x00,0xF8,0x80,0x80};
              unsigned char Nhap_Phut_Alarm2[8]={0x00,0xE8,0xA8,0xF8,0x00,0xF8,0x10,0x20};
              unsigned char Nhap_Phut_Alarm3[8]={0x10,0xF8,0x00,0xE0,0x20,0xE0,0x20,0xE0};
void main(void)


{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer

	/* Clock */
    BCSCTL1 &=~ BIT2;                       //| RSELx = 11; DCOx = 3 ====>    f = 4.25 MHz  //|
    BCSCTL1 |=  (BIT0 + BIT1 + BIT3);       //|                                             //|
    DCOCTL  &=~ BIT7;                       //|                                             //|
    DCOCTL  |=  (BIT6 + BIT7);              //|---------------------------------------------//|

    /*GPIO Initialize*/
    P2DIR |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6);
    P2OUT &=~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6);

    P1DIR |= BIT5 + BIT6; P1OUT &=~ BIT5;
    P1DIR&=~(BIT0 + BIT1 + BIT2 + BIT3);          //Input P1.0->P1.3
    P1REN|=(BIT0 + BIT1 + BIT2 + BIT3);           //Enable Resistor
    P1OUT|=(BIT0 + BIT1 + BIT2 + BIT3 + BIT6);           //Resistor Connected with Vcc

    LATCH_OFF_ROW;
    LATCH_OFF_COL;

     /*Interrupt Initialize*/
     P1IE |=(BIT3 + BIT1 + BIT0 + BIT2);     //Interrupt Enable on P1.3,P1.1
     P1IES |=(BIT3 + BIT1 + BIT0 + BIT2);    //Ngat canh xuong (1->0)
     P1IFG &=~(BIT3 + BIT1 + BIT0 + BIT2);   //Delete Interrupt Flag

    /*Set Timer Interrupt*/
     TA0CTL = TASSEL_2 + ID_3 + MC_2 + TAIE;
     TA0CCR1  = 53125;
     TA0CCTL1 = CCIE;
     _BIS_SR(GIE);
     __bis_SR_register(GIE);

/*---------------*Firstly, Set time, day *----------------*/
     Mode_Interrupt = 1;    //Set Time
     Data_LED2[6]=0x00; Data_LED2[7]=0x00;
    //Nhap Gio - Output: Hour
     unsigned char Nhap_Gio_1[8]={0xCC,0x92,0x92,0x64,0x00,0xFE,0x92,0x92};
     unsigned char Nhap_Gio_2[8]={0x92,0x00,0x02,0xFE,0x02,0x00,0x00,0xC0};
     unsigned char Nhap_Gio_3[8]={0xC0,0x00,0xFC,0x20,0xFC,0x00,0x00,0x00};

    unsigned int i;
    while(1){
        if (Button_Count >= 24) { Button_Count = 0; }
        if (Exit) {Hour = Button_Count; Button_Count = 0;   Exit = 0; break;}
        setHour(Button_Count);
        for (i=0;i<8;i++)
             {DataTransfer595(0x00,Data_LED2[i],Data_LED1[i],Nhap_Gio_3[i],Nhap_Gio_2[i],Nhap_Gio_1[i],COL[i]);}
               }

    //Nhap Phut - Output: Minute
    unsigned char Nhap_Phut_1[8]={0xCC,0x92,0x92,0x64,0x00,0xFE,0x92,0x92};
    unsigned char Nhap_Phut_2[8]={0x92,0x00,0x02,0xFE,0x02,0x00,0x00,0xC0};
    unsigned char Nhap_Phut_3[8]={0xC0,0x00,0xFC,0x08,0x10,0x08,0xFC,0x00};

    while(1){
        if (Button_Count >= 60) { Button_Count = 0; }
        if (Exit) {Minute = Button_Count; Button_Count = 0; Exit = 0; break;}
        setMinute(Button_Count);
        for (i=0;i<8;i++)
            {DataTransfer595(0x00,Data_LED2[i],Data_LED1[i],Nhap_Phut_3[i],Nhap_Phut_2[i],Nhap_Phut_1[i],COL[i]);}
               }
    //Nhap Ngay - Output: Day
        unsigned char Nhap_Ngay_1[8]={0xCC,0x92,0x92,0x64,0x00,0xFE,0x92,0x92};
        unsigned char Nhap_Ngay_2[8]={0x92,0x00,0x02,0xFE,0x02,0x00,0x00,0xC0};
        unsigned char Nhap_Ngay_3[8]={0xC0,0x00,0xFC,0x84,0x84,0x78,0x00,0x00};

        while(1){
            if (Button_Count >= 32) { Button_Count = 0; }
            if (Exit) {Day = Button_Count; Button_Count = 0; Exit = 0; break;}
            setDay(Button_Count);
            for (i=0;i<8;i++)
                {DataTransfer595(0x00,Data_LED2[i],Data_LED1[i],Nhap_Ngay_3[i],Nhap_Ngay_2[i],Nhap_Ngay_1[i],COL[i]);}
                   }

    //Nhap Thang - Output: Month
    unsigned char Nhap_Thang_1[8]={0xCC,0x92,0x92,0x64,0x00,0xFE,0x92,0x92};
    unsigned char Nhap_Thang_2[8]={0x92,0x00,0x02,0xFE,0x02,0x00,0x00,0xC0};
    unsigned char Nhap_Thang_3[8]={0xC0,0x00,0xFC,0x08,0x10,0x08,0xFC,0x00};

    while(1){
        if (Button_Count >= 13) { Button_Count = 0; }
        if (Exit) {Month = Button_Count; Button_Count = 0; Exit = 0; break;}
        setMonth(Button_Count);
        for (i=0;i<8;i++)
            {DataTransfer595(0x00,Data_LED2[i],Data_LED1[i],Nhap_Thang_3[i],Nhap_Thang_2[i],Nhap_Thang_1[i],COL[i]);}
               }

/*--------------*Secondly,Display Data*-----------------*/
/******PROCESSING******/
         int DieuKienBaoThuc = 0;

         Mode = TimeMode;
  while(1) {
        Mode_Interrupt = 2;    //Chon Alarm
        while(1)    //Hien thi Thoi gian, Ngay Thang
        {
            if(Exit) {Exit = 0; break;}
            Check_Time();
            DieuKienBaoThuc = Dieu_Kien_Bao_Thuc();
            if (DieuKienBaoThuc) { Baothuc();}
            // After I select the data for LED DISPLAYing:
                if (Mode == TimeMode)   Time_Display();
                else Day_Display();
            //Displaying
                for (i=0;i<8;i++)
                {DataTransfer595(Data_LED3[i],Data_LED2[i],Data_LED1[i],LEDMatrix_3[i],LEDMatrix_2[i],LEDMatrix_1[i],COL[i]);}
        }
        Data_LED2[6]=0x00; Data_LED2[7]=0x00;
        /*Baothuc*/
        Mode_Interrupt = 3;    //Cai Dat Bao Thuc
        while(1)    //Cai dat gio Bao thuc
        {
            if(Exit_Alarm) {Exit_Alarm = 0; break;}
            SetAlarm();
        }
}}



//================
int HangChuc_1(int k)
{

    switch (k) {
    case 0:     Data_LED1[0] = Number_0[0];
                Data_LED1[1] = Number_0[1];
                Data_LED1[2] = Number_0[2];
                Data_LED1[3] = Number_0[3];
                Data_LED1[4] = Number_0[4]; return(0);
    case 1:     Data_LED1[0] = Number_1[0];
                Data_LED1[1] = Number_1[1];
                Data_LED1[2] = Number_1[2];
                Data_LED1[3] = Number_1[3];
                Data_LED1[4] = Number_1[4]; return(1);
    case 2:     Data_LED1[0] = Number_2[0];
                Data_LED1[1] = Number_2[1];
                Data_LED1[2] = Number_2[2];
                Data_LED1[3] = Number_2[3];
                Data_LED1[4] = Number_2[4]; return(2);
    case 3:     Data_LED1[0] = Number_3[0];
                Data_LED1[1] = Number_3[1];
                Data_LED1[2] = Number_3[2];
                Data_LED1[3] = Number_3[3];
                Data_LED1[4] = Number_3[4]; return(3);
    case 4:     Data_LED1[0] = Number_4[0];
                Data_LED1[1] = Number_4[1];
                Data_LED1[2] = Number_4[2];
                Data_LED1[3] = Number_4[3];
                Data_LED1[4] = Number_4[4]; return(4);
    case 5:     Data_LED1[0] = Number_5[0];
                Data_LED1[1] = Number_5[1];
                Data_LED1[2] = Number_5[2];
                Data_LED1[3] = Number_5[3];
                Data_LED1[4] = Number_5[4]; return(5);
    case 6:     Data_LED1[0] = Number_6[0];
                Data_LED1[1] = Number_6[1];
                Data_LED1[2] = Number_6[2];
                Data_LED1[3] = Number_6[3];
                Data_LED1[4] = Number_6[4]; return(6); }
    return(100);
}
//===================
int HangDonVi_1(int k)
{
    switch (k)
    {
    case 0: Data_LED1[6] = Number_0[0];
            Data_LED1[7] = Number_0[1];
            Data_LED2[0] = Number_0[2];
            Data_LED2[1] = Number_0[3];
            Data_LED2[2] = Number_0[4]; return(0);
    case 1: Data_LED1[6] = Number_1[0];
            Data_LED1[7] = Number_1[1];
            Data_LED2[0] = Number_1[2];
            Data_LED2[1] = Number_1[3];
            Data_LED2[2] = Number_1[4]; return(1);
    case 2: Data_LED1[6] = Number_2[0];
            Data_LED1[7] = Number_2[1];
            Data_LED2[0] = Number_2[2];
            Data_LED2[1] = Number_2[3];
            Data_LED2[2] = Number_2[4]; return(2);
    case 3: Data_LED1[6] = Number_3[0];
            Data_LED1[7] = Number_3[1];
            Data_LED2[0] = Number_3[2];
            Data_LED2[1] = Number_3[3];
            Data_LED2[2] = Number_3[4]; return(3);
    case 4: Data_LED1[6] = Number_4[0];
            Data_LED1[7] = Number_4[1];
            Data_LED2[0] = Number_4[2];
            Data_LED2[1] = Number_4[3];
            Data_LED2[2] = Number_4[4]; return(4);
    case 5: Data_LED1[6] = Number_5[0];
            Data_LED1[7] = Number_5[1];
            Data_LED2[0] = Number_5[2];
            Data_LED2[1] = Number_5[3];
            Data_LED2[2] = Number_5[4]; return(5);
    case 6: Data_LED1[6] = Number_6[0];
            Data_LED1[7] = Number_6[1];
            Data_LED2[0] = Number_6[2];
            Data_LED2[1] = Number_6[3];
            Data_LED2[2] = Number_6[4]; return(6);
    case 7: Data_LED1[6] = Number_7[0];
            Data_LED1[7] = Number_7[1];
            Data_LED2[0] = Number_7[2];
            Data_LED2[1] = Number_7[3];
            Data_LED2[2] = Number_7[4]; return(7);
    case 8: Data_LED1[6] = Number_8[0];
            Data_LED1[7] = Number_8[1];
            Data_LED2[0] = Number_8[2];
            Data_LED2[1] = Number_8[3];
            Data_LED2[2] = Number_8[4]; return(8);
    case 9: Data_LED1[6] = Number_9[0];
            Data_LED1[7] = Number_9[1];
            Data_LED2[0] = Number_9[2];
            Data_LED2[1] = Number_9[3];
            Data_LED2[2] = Number_9[4]; return(9);}
    return(100);
}
//================================
int HangChuc_2(int k)
{
    switch (k)
        {
        case 0:   Data_LED2[6] = Number_0[0];
                  Data_LED2[7] = Number_0[1];
                  Data_LED3[0] = Number_0[2];
                  Data_LED3[1] = Number_0[3];
                  Data_LED3[2] = Number_0[4]; return(0);
        case 1:   Data_LED2[6] = Number_1[0];
                  Data_LED2[7] = Number_1[1];
                  Data_LED3[0] = Number_1[2];
                  Data_LED3[1] = Number_1[3];
                  Data_LED3[2] = Number_1[4]; return(1);
        case 2:   Data_LED2[6] = Number_2[0];
                  Data_LED2[7] = Number_2[1];
                  Data_LED3[0] = Number_2[2];
                  Data_LED3[1] = Number_2[3];
                  Data_LED3[2] = Number_2[4]; return(2);
        case 3:   Data_LED2[6] = Number_3[0];
                  Data_LED2[7] = Number_3[1];
                  Data_LED3[0] = Number_3[2];
                  Data_LED3[1] = Number_3[3];
                  Data_LED3[2] = Number_3[4]; return(3);
        case 4:   Data_LED2[6] = Number_4[0];
                  Data_LED2[7] = Number_4[1];
                  Data_LED3[0] = Number_4[2];
                  Data_LED3[1] = Number_4[3];
                  Data_LED3[2] = Number_4[4]; return(4);
        case 5:   Data_LED2[6] = Number_5[0];
                  Data_LED2[7] = Number_5[1];
                  Data_LED3[0] = Number_5[2];
                  Data_LED3[1] = Number_5[3];
                  Data_LED3[2] = Number_5[4]; return(5);}
        return(100);
}
//=====================
int HangDonVi_2(int k)
{
    switch (k)
    {
    case 0:  Data_LED3[4] = x4_Number_0[0];
             Data_LED3[5] = x4_Number_0[1];
             Data_LED3[6] = x4_Number_0[2];
             Data_LED3[7] = x4_Number_0[3]; return(0);
    case 1:  Data_LED3[4] = x4_Number_1[0];
             Data_LED3[5] = x4_Number_1[1];
             Data_LED3[6] = x4_Number_1[2];
             Data_LED3[7] = x4_Number_1[3]; return(0);
    case 2:  Data_LED3[4] = x4_Number_2[0];
             Data_LED3[5] = x4_Number_2[1];
             Data_LED3[6] = x4_Number_2[2];
             Data_LED3[7] = x4_Number_2[3]; return(0);
    case 3:  Data_LED3[4] = x4_Number_3[0];
             Data_LED3[5] = x4_Number_3[1];
             Data_LED3[6] = x4_Number_3[2];
             Data_LED3[7] = x4_Number_3[3]; return(0);
    case 4:  Data_LED3[4] = x4_Number_4[0];
             Data_LED3[5] = x4_Number_4[1];
             Data_LED3[6] = x4_Number_4[2];
             Data_LED3[7] = x4_Number_4[3]; return(0);
    case 5:  Data_LED3[4] = x4_Number_5[0];
             Data_LED3[5] = x4_Number_5[1];
             Data_LED3[6] = x4_Number_5[2];
             Data_LED3[7] = x4_Number_5[3]; return(0);
    case 6:  Data_LED3[4] = x4_Number_6[0];
             Data_LED3[5] = x4_Number_6[1];
             Data_LED3[6] = x4_Number_6[2];
             Data_LED3[7] = x4_Number_6[3]; return(0);
    case 7:  Data_LED3[4] = x4_Number_7[0];
             Data_LED3[5] = x4_Number_7[1];
             Data_LED3[6] = x4_Number_7[2];
             Data_LED3[7] = x4_Number_7[3]; return(0);
    case 8:  Data_LED3[4] = x4_Number_8[0];
             Data_LED3[5] = x4_Number_8[1];
             Data_LED3[6] = x4_Number_8[2];
             Data_LED3[7] = x4_Number_8[3]; return(0);
    case 9:  Data_LED3[4] = x4_Number_9[0];
             Data_LED3[5] = x4_Number_9[1];
             Data_LED3[6] = x4_Number_9[2];
             Data_LED3[7] = x4_Number_9[3]; return(0);}
    return(100);
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
//=========================================
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TAIV_Interrupt(void)
{
    if (TA0IV == 0x02)
    {
        Count_Second++;

        if (Count_Second == 7)//7
            {
            if (Baothuc_Mode == 1) P1OUT ^= BIT5;
            Count_Second = 0;
            Count_Minute++;
            Data_LED2[4] ^= 0x28;
            Speaker_Up++;
            }
        if (Speaker_Up == 10) {Baothuc_Mode = 0;EndBaothuc = 0;P1OUT &=~ BIT5;}
       if (Count_Minute == 120)//120
           {
            Minute++;
            Count_Minute = 0;
            /*Baothuc*/


           }
    TA0CCR1  += 53125;
    }
}
//==============================================
#pragma vector = PORT1_VECTOR
__interrupt void Button_Interrupt(void)
{
    if(Mode_Interrupt == 1)
    {       if(P1IFG & BIT1){Button_Count++; P1IFG &=~BIT1; }
            if(P1IFG & BIT3){Exit = 1;P1IFG &=~BIT3;}
    }

    if(Mode_Interrupt == 2)
    {   //DayMode,TimeMode: P1.1    ; Alarm: P1.3; Snake: P1.2
            if(P1IFG & BIT1) {     if (Mode == DayMode) Mode = TimeMode; else Mode = DayMode; P1IFG &=~BIT1;}
            if(P1IFG & BIT3) {Exit = 1;P1IFG &=~BIT3;}
    }
    if(Mode_Interrupt == 3)
    {
            if(P1IFG & BIT1){Button_Count++; P1IFG &=~BIT1; }
            if(P1IFG & BIT3){Exit = 1;P1IFG &=~BIT3;}
    }
    if(Baothuc_Mode == 1)
    {
        if (P1IFG & BIT1) {EndBaothuc = 0; P1OUT &=~ BIT5;Baothuc_Mode = 0; P1IFG &=~BIT1;}
        if (P1IFG & BIT2) {EndBaothuc = 0; P1OUT &=~ BIT5;Baothuc_Mode = 0; P1IFG &=~BIT2;}
        if (P1IFG & BIT3) {EndBaothuc = 0; P1OUT &=~ BIT5;Baothuc_Mode = 0; P1IFG &=~BIT3;}
        if (P1IFG & BIT0) {EndBaothuc = 0; P1OUT &=~ BIT5;Baothuc_Mode = 0; P1IFG &=~BIT0;}
    }
}
//==============================================
int setHour(int Count_Button_F)
{
    switch (Count_Button_F)
           {
           case 0: HangChuc_1(0);HangDonVi_1(0);return(0);
           case 1: HangChuc_1(0);HangDonVi_1(1);return(1);
           case 2: HangChuc_1(0);HangDonVi_1(2);return(2);
           case 3: HangChuc_1(0);HangDonVi_1(3);return(3);
           case 4: HangChuc_1(0);HangDonVi_1(4);return(4);
           case 5: HangChuc_1(0);HangDonVi_1(5);return(5);
           case 6: HangChuc_1(0);HangDonVi_1(6);return(6);
           case 7: HangChuc_1(0);HangDonVi_1(7);return(7);
           case 8: HangChuc_1(0);HangDonVi_1(8);return(8);
           case 9: HangChuc_1(0);HangDonVi_1(9);return(9);
           case 10:HangChuc_1(1);HangDonVi_1(0);return(10);
           case 11:HangChuc_1(1);HangDonVi_1(1);return(11);
           case 12:HangChuc_1(1);HangDonVi_1(2);return(12);
           case 13:HangChuc_1(1);HangDonVi_1(3);return(13);
           case 14:HangChuc_1(1);HangDonVi_1(4);return(14);
           case 15:HangChuc_1(1);HangDonVi_1(5);return(15);
           case 16:HangChuc_1(1);HangDonVi_1(6);return(16);
           case 17:HangChuc_1(1);HangDonVi_1(7);return(17);
           case 18:HangChuc_1(1);HangDonVi_1(8);return(18);
           case 19:HangChuc_1(1);HangDonVi_1(9);return(19);
           case 20:HangChuc_1(2);HangDonVi_1(0);return(20);
           case 21:HangChuc_1(2);HangDonVi_1(1);return(21);
           case 22:HangChuc_1(2);HangDonVi_1(2);return(22);
           case 23:HangChuc_1(2);HangDonVi_1(3);return(23);}
return(100);
           }
//=========================================================
int setMinute(int Count_Button_F)
{
    switch (Count_Button_F)  {
    case 0:HangChuc_1(0);HangDonVi_1(0);return(0);
    case 1:HangChuc_1(0);HangDonVi_1(1);return(1);
    case 2:HangChuc_1(0);HangDonVi_1(2);return(2);
    case 3:HangChuc_1(0);HangDonVi_1(3);return(3);
    case 4:HangChuc_1(0);HangDonVi_1(4);return(4);
    case 5:HangChuc_1(0);HangDonVi_1(5);return(5);
    case 6:HangChuc_1(0);HangDonVi_1(6);return(6);
    case 7:HangChuc_1(0);HangDonVi_1(7);return(7);
    case 8:HangChuc_1(0);HangDonVi_1(8);return(8);
    case 9:HangChuc_1(0);HangDonVi_1(9);return(9);
    case 10:HangChuc_1(1);HangDonVi_1(0);return(10);
    case 11:HangChuc_1(1);HangDonVi_1(1);return(11);
    case 12:HangChuc_1(1);HangDonVi_1(2);return(12);
    case 13:HangChuc_1(1);HangDonVi_1(3);return(13);
    case 24:HangChuc_1(2);HangDonVi_1(4);return(24);
    case 25:HangChuc_1(2);HangDonVi_1(5);return(25);
    case 26:HangChuc_1(2);HangDonVi_1(6);return(26);
    case 27:HangChuc_1(2);HangDonVi_1(7);return(27);
    case 28:HangChuc_1(2);HangDonVi_1(8);return(28);
    case 29:HangChuc_1(2);HangDonVi_1(9);return(29);
    case 30:HangChuc_1(3);HangDonVi_1(0);return(30);
    case 31:HangChuc_1(3);HangDonVi_1(1);return(31);
    case 32:HangChuc_1(3);HangDonVi_1(2);return(32);
    case 33:HangChuc_1(3);HangDonVi_1(3);return(33);
    case 34:HangChuc_1(3);HangDonVi_1(4);return(34);
    case 35:HangChuc_1(3);HangDonVi_1(5);return(35);
    case 36:HangChuc_1(3);HangDonVi_1(6);return(36);
    case 37:HangChuc_1(3);HangDonVi_1(7);return(37);
    case 38:HangChuc_1(3);HangDonVi_1(8);return(38);
    case 39:HangChuc_1(3);HangDonVi_1(9);return(39);
    case 40:HangChuc_1(4);HangDonVi_1(0);return(40);
    case 41:HangChuc_1(4);HangDonVi_1(1);return(41);
    case 42:HangChuc_1(4);HangDonVi_1(2);return(42);
    case 43:HangChuc_1(4);HangDonVi_1(3);return(43);
    case 44:HangChuc_1(4);HangDonVi_1(4);return(44);
    case 45:HangChuc_1(4);HangDonVi_1(5);return(45);
    case 46:HangChuc_1(4);HangDonVi_1(6);return(46);
    case 47:HangChuc_1(4);HangDonVi_1(7);return(47);
    case 48:HangChuc_1(4);HangDonVi_1(8);return(48);
    case 49:HangChuc_1(4);HangDonVi_1(9);return(49);
    case 50:HangChuc_1(5);HangDonVi_1(0);return(50);
    case 51:HangChuc_1(5);HangDonVi_1(1);return(51);
    case 52:HangChuc_1(5);HangDonVi_1(2);return(52);
    case 53:HangChuc_1(5);HangDonVi_1(3);return(53);
    case 54:HangChuc_1(5);HangDonVi_1(4);return(54);
    case 55:HangChuc_1(5);HangDonVi_1(5);return(55);
    case 56:HangChuc_1(5);HangDonVi_1(6);return(56);
    case 57:HangChuc_1(5);HangDonVi_1(7);return(57);
    case 58:HangChuc_1(5);HangDonVi_1(8);return(58);
    case 59:HangChuc_1(5);HangDonVi_1(9);return(59); }
    return(100);
}
//====================================
int setDay(int Count_Button_F)
{
    switch (Count_Button_F)  {
    case 1:HangChuc_1(0);HangDonVi_1(1);return(1);
    case 2:HangChuc_1(0);HangDonVi_1(2);return(2);
    case 3:HangChuc_1(0);HangDonVi_1(3);return(3);
    case 4:HangChuc_1(0);HangDonVi_1(4);return(4);
    case 5:HangChuc_1(0);HangDonVi_1(5);return(5);
    case 6:HangChuc_1(0);HangDonVi_1(6);return(6);
    case 7:HangChuc_1(0);HangDonVi_1(7);return(7);
    case 8:HangChuc_1(0);HangDonVi_1(8);return(8);
    case 9:HangChuc_1(0);HangDonVi_1(9);return(9);
    case 10:HangChuc_1(1);HangDonVi_1(0);return(10);
    case 11:HangChuc_1(1);HangDonVi_1(1);return(11);
    case 12:HangChuc_1(1);HangDonVi_1(2);return(12);
    case 13:HangChuc_1(1);HangDonVi_1(3);return(13);
    case 14:HangChuc_1(1);HangDonVi_1(4);return(14);
    case 15:HangChuc_1(1);HangDonVi_1(5);return(15);
    case 16:HangChuc_1(1);HangDonVi_1(6);return(16);
    case 17:HangChuc_1(1);HangDonVi_1(7);return(17);
    case 18:HangChuc_1(1);HangDonVi_1(8);return(18);
    case 19:HangChuc_1(1);HangDonVi_1(9);return(19);
    case 20:HangChuc_1(2);HangDonVi_1(0);return(20);
    case 21:HangChuc_1(2);HangDonVi_1(1);return(21);
    case 22:HangChuc_1(2);HangDonVi_1(2);return(22);
    case 23:HangChuc_1(2);HangDonVi_1(3);return(23);
    case 24:HangChuc_1(2);HangDonVi_1(4);return(24);
    case 25:HangChuc_1(2);HangDonVi_1(5);return(25);
    case 26:HangChuc_1(2);HangDonVi_1(6);return(26);
    case 27:HangChuc_1(2);HangDonVi_1(7);return(27);
    case 28:HangChuc_1(2);HangDonVi_1(8);return(28);
    case 29:HangChuc_1(2);HangDonVi_1(9);return(29);
    case 30:HangChuc_1(3);HangDonVi_1(0);return(30);
    case 31:HangChuc_1(3);HangDonVi_1(1);return(31);
    }
    return(100);
}
//====================================================
int setMonth(int Count_Button_F)
{
    switch (Count_Button_F)  {
    case 1:HangChuc_1(0);HangDonVi_1(1);return(1);
    case 2:HangChuc_1(0);HangDonVi_1(2);return(2);
    case 3:HangChuc_1(0);HangDonVi_1(3);return(3);
    case 4:HangChuc_1(0);HangDonVi_1(4);return(4);
    case 5:HangChuc_1(0);HangDonVi_1(5);return(5);
    case 6:HangChuc_1(0);HangDonVi_1(6);return(6);
    case 7:HangChuc_1(0);HangDonVi_1(7);return(7);
    case 8:HangChuc_1(0);HangDonVi_1(8);return(8);
    case 9:HangChuc_1(0);HangDonVi_1(9);return(9);
    case 10:HangChuc_1(1);HangDonVi_1(0);return(10);
    case 11:HangChuc_1(1);HangDonVi_1(1);return(11);
    case 12:HangChuc_1(1);HangDonVi_1(2);return(12);
    }
    return(100);
}
//===========================
void    Check_Time()
{
    switch (Month){
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12: if (Day == 32) {Day = 1; Month++;} break;
    case 2:  if (Day == 29)  {Day = 1; Month++;} if (Day >=30) while(1); break;
    case 4:
    case 6:
    case 9:
    case 11: if (Day == 31) {Day = 1; Month++;} break;
    default: while(1);}
    if (Minute == 60) {Minute = 0; Hour++;}
    if (Hour == 24)   {Hour = 0; Day++;}
    if (Month == 13)  {Month = 1;}
}
//============================
int    Time_Display()
{
    unsigned int m;
    for (m = 0;m<8;m++) { LEDMatrix_3[m] = Time_LEDMatrix_3[m];
    LEDMatrix_2[m] = Time_LEDMatrix_2[m];
    LEDMatrix_1[m] = Time_LEDMatrix_1[m];}
    setHour(Hour);
    switch (Minute)  {
        case 0:HangChuc_2(0);HangDonVi_2(0);return(0);
        case 1:HangChuc_2(0);HangDonVi_2(1);return(1);
        case 2:HangChuc_2(0);HangDonVi_2(2);return(2);
        case 3:HangChuc_2(0);HangDonVi_2(3);return(3);
        case 4:HangChuc_2(0);HangDonVi_2(4);return(4);
        case 5:HangChuc_2(0);HangDonVi_2(5);return(5);
        case 6:HangChuc_2(0);HangDonVi_2(6);return(6);
        case 7:HangChuc_2(0);HangDonVi_2(7);return(7);
        case 8:HangChuc_2(0);HangDonVi_2(8);return(8);
        case 9:HangChuc_2(0);HangDonVi_2(9);return(9);
        case 10:HangChuc_2(1);HangDonVi_2(0);return(10);
        case 11:HangChuc_2(1);HangDonVi_2(1);return(11);
        case 12:HangChuc_2(1);HangDonVi_2(2);return(12);
        case 13:HangChuc_2(1);HangDonVi_2(3);return(13);
        case 14:HangChuc_2(1);HangDonVi_2(4);return(14);
        case 15:HangChuc_2(1);HangDonVi_2(5);return(15);
        case 16:HangChuc_2(1);HangDonVi_2(6);return(16);
        case 17:HangChuc_2(1);HangDonVi_2(7);return(17);
        case 18:HangChuc_2(1);HangDonVi_2(8);return(18);
        case 19:HangChuc_2(1);HangDonVi_2(9);return(19);
        case 20:HangChuc_2(2);HangDonVi_2(0);return(20);
        case 21:HangChuc_2(2);HangDonVi_2(1);return(21);
        case 22:HangChuc_2(2);HangDonVi_2(2);return(22);
        case 23:HangChuc_2(2);HangDonVi_2(3);return(23);
        case 24:HangChuc_2(2);HangDonVi_2(4);return(24);
        case 25:HangChuc_2(2);HangDonVi_2(5);return(25);
        case 26:HangChuc_2(2);HangDonVi_2(6);return(26);
        case 27:HangChuc_2(2);HangDonVi_2(7);return(27);
        case 28:HangChuc_2(2);HangDonVi_2(8);return(28);
        case 29:HangChuc_2(2);HangDonVi_2(9);return(29);
        case 30:HangChuc_2(3);HangDonVi_2(0);return(30);
        case 31:HangChuc_2(3);HangDonVi_2(1);return(31);
        case 32:HangChuc_2(3);HangDonVi_2(2);return(32);
        case 33:HangChuc_2(3);HangDonVi_2(3);return(33);
        case 34:HangChuc_2(3);HangDonVi_2(4);return(34);
        case 35:HangChuc_2(3);HangDonVi_2(5);return(35);
        case 36:HangChuc_2(3);HangDonVi_2(6);return(36);
        case 37:HangChuc_2(3);HangDonVi_2(7);return(37);
        case 38:HangChuc_2(3);HangDonVi_2(8);return(38);
        case 39:HangChuc_2(3);HangDonVi_2(9);return(39);
        case 40:HangChuc_2(4);HangDonVi_2(0);return(40);
        case 41:HangChuc_2(4);HangDonVi_2(1);return(41);
        case 42:HangChuc_2(4);HangDonVi_2(2);return(42);
        case 43:HangChuc_2(4);HangDonVi_2(3);return(43);
        case 44:HangChuc_2(4);HangDonVi_2(4);return(44);
        case 45:HangChuc_2(4);HangDonVi_2(5);return(45);
        case 46:HangChuc_2(4);HangDonVi_2(6);return(46);
        case 47:HangChuc_2(4);HangDonVi_2(7);return(47);
        case 48:HangChuc_2(4);HangDonVi_2(8);return(48);
        case 49:HangChuc_2(4);HangDonVi_2(9);return(49);
        case 50:HangChuc_2(5);HangDonVi_2(0);return(50);
        case 51:HangChuc_2(5);HangDonVi_2(1);return(51);
        case 52:HangChuc_2(5);HangDonVi_2(2);return(52);
        case 53:HangChuc_2(5);HangDonVi_2(3);return(53);
        case 54:HangChuc_2(5);HangDonVi_2(4);return(54);
        case 55:HangChuc_2(5);HangDonVi_2(5);return(55);
        case 56:HangChuc_2(5);HangDonVi_2(6);return(56);
        case 57:HangChuc_2(5);HangDonVi_2(7);return(57);
        case 58:HangChuc_2(5);HangDonVi_2(8);return(58);
        case 59:HangChuc_2(5);HangDonVi_2(9);return(59); }
        return(100);
}
//======================
int     Day_Display()
{
    unsigned int m;
    for (m = 0;m<8;m++) { LEDMatrix_3[m] = Date_LEDMatrix_3[m];
     LEDMatrix_2[m] = Date_LEDMatrix_2[m];
     LEDMatrix_1[m] = Date_LEDMatrix_1[m];}
     setDay(Day);
     switch (Month)  {
        case 1:HangChuc_2(0);HangDonVi_2(1);return(1);
        case 2:HangChuc_2(0);HangDonVi_2(2);return(2);
        case 3:HangChuc_2(0);HangDonVi_2(3);return(3);
        case 4:HangChuc_2(0);HangDonVi_2(4);return(4);
        case 5:HangChuc_2(0);HangDonVi_2(5);return(5);
        case 6:HangChuc_2(0);HangDonVi_2(6);return(6);
        case 7:HangChuc_2(0);HangDonVi_2(7);return(7);
        case 8:HangChuc_2(0);HangDonVi_2(8);return(8);
        case 9:HangChuc_2(0);HangDonVi_2(9);return(9);
        case 10:HangChuc_2(1);HangDonVi_2(0);return(10);
        case 11:HangChuc_2(1);HangDonVi_2(1);return(11);
        case 12:HangChuc_2(1);HangDonVi_2(2);return(12);
        }
        return(100);
}
//====================================
int SetAlarm()
{
    unsigned int i;
    while(1){
           if (Button_Count >= 24) { Button_Count = 0; }
           if (Exit) {Hour_Alarm = Button_Count; Button_Count = 0; Exit = 0; break;}
           setHour(Button_Count);
           for (i=0;i<8;i++)
               {DataTransfer595(0x00,Data_LED2[i],Data_LED1[i],Nhap_Gio_Alarm3[i],Nhap_Gio_Alarm2[i],Nhap_Gio_Alarm1[i],COL[i]);}   }
    while(1){
            Exit_Alarm = 1;
           if (Button_Count >= 60) { Button_Count = 0; }
           if (Exit) {Minute_Alarm = Button_Count; Button_Count = 0; Exit = 0; return(1);}
           setMinute(Button_Count);
           for (i=0;i<8;i++)
                   {DataTransfer595(0x00,Data_LED2[i],Data_LED1[i],Nhap_Phut_Alarm3[i],Nhap_Phut_Alarm2[i],Nhap_Phut_Alarm1[i],COL[i]);}   }

}
//=======================================
int     Dieu_Kien_Bao_Thuc()
{
    if (Hour == Hour_Alarm)
    {   if (Minute == Minute_Alarm){
        Baothuc();
        return(1);
    }}
    return(0);

}
//========================================
int     Baothuc()
{
    Speaker_Up = 0; EndBaothuc = 1; Baothuc_Mode = 1;
    unsigned int i;
    int Bien = 30;
    while(EndBaothuc == 1)
    {
        while (1)
        {
            Bien--;
            if (Bien == 20) break;
            for (i=0;i<8;i++)
            {DataTransfer595(0xff,0xff,0xff,0xff,0xff,0xff,COL[i]);}
            }
        while (1)
        {
            Bien--;     if (Bien == 10) {Bien = 30; break;}
            for (i=0;i<8;i++)
            {DataTransfer595(0x00,0x00,0x00,0x00,0x00,0x00,COL[i]);}
            }
    }
return(100);
}

//=======================================

