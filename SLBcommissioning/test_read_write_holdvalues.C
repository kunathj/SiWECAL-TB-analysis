//# Copyright 2020  Adrián Irles (IJCLab, CNRS/IN2P3)

#include "conf_struct.h"

void test_read_write_holdvalues(TString filename="15102021/Run_Settings_it8", bool debug=false) {
  int hold[15][16]={0}; //layer/chip
  hold[6][0]=58;
  hold[6][1]=60;
  hold[6][2]=59;
  hold[6][3]=56;
  hold[6][4]=56;
  hold[6][5]=59;
  hold[6][6]=58;
  hold[6][7]=59;
  hold[6][8]=56;;
  hold[6][9]=57;
  hold[6][10]=58;
  hold[6][11]=54;
  hold[6][12]=63;
  hold[6][13]=55;
  hold[6][14]=57;
  hold[6][15]=58;
  hold[7][0]=59;;
  hold[7][1]=57;
  hold[7][2]=55;
  hold[7][3]=57;
  hold[7][4]=58;
  hold[7][5]=57;
  hold[7][6]=58;
  hold[7][7]=55;
  hold[7][8]=57;
  hold[7][9]=55;
  hold[7][10]=57;
  hold[7][11]=62;
  hold[7][12]=60;
  hold[7][13]=54;
  hold[7][14]=55;
  hold[7][15]=62;;
  hold[13][0]=70;
  hold[13][1]=64;
  hold[13][2]=61;
  hold[13][3]=65;
  hold[13][4]=66;
  hold[13][5]=71;
  hold[13][6]=68;
  hold[13][7]=66;
  hold[13][8]=58;
  hold[13][9]=60;
  hold[13][10]=64;
  hold[13][11]=60;
  hold[13][12]=54;
  hold[13][13]=63;
  hold[13][14]=61;
  hold[13][15]=65;
  hold[12][0]=59;
  hold[12][1]=65;
  hold[12][2]=62;
  hold[12][3]=67;
  hold[12][4]=64;
  hold[12][5]=61;
  hold[12][6]=68;
  hold[12][7]=70;
  hold[12][8]=57;
  hold[12][9]=59;
  hold[12][10]=57;
  hold[12][11]=66;
  hold[12][12]=69;
  hold[12][13]=61;
  hold[12][14]=65;
  hold[12][15]=57;
  hold[11][0]=64;
  hold[11][1]=62;
  hold[11][2]=56;
  hold[11][3]=54;
  hold[11][4]=68;
  hold[11][5]=60;
  hold[11][6]=65;
  hold[11][7]=61;
  hold[11][8]=64;
  hold[11][9]=57;
  hold[11][10]=57;
  hold[11][11]=57;
  hold[11][12]=64;
  hold[11][13]=61;
  hold[11][14]=56;
  hold[11][15]=68;
  hold[10][0]=58;
  hold[10][1]=59;
  hold[10][2]=61;
  hold[10][3]=62;
  hold[10][4]=60;
  hold[10][5]=61;
  hold[10][6]=57;
  hold[10][7]=54;
  hold[10][8]=61;
  hold[10][9]=61;
  hold[10][10]=60;
  hold[10][11]=66;
  hold[10][12]=62;
  hold[10][13]=63;
  hold[10][14]=69;
  hold[10][15]=62;
  hold[9][0]=62;
  hold[9][1]=65;
  hold[9][2]=59;
  hold[9][3]=61;
  hold[9][4]=58;
  hold[9][5]=63;
  hold[9][6]=53;
  hold[9][7]=62;
  hold[9][8]=56;
  hold[9][9]=64;
  hold[9][10]=56;
  hold[9][11]=60;
  hold[9][12]=65;
  hold[9][13]=67;
  hold[9][14]=61;
  hold[9][15]=65;
  hold[5][0]=55;
  hold[5][1]=53;
  hold[5][2]=56;
  hold[5][3]=58;
  hold[5][4]=56;
  hold[5][5]=60;;
  hold[5][6]=56;
  hold[5][7]=58;
  hold[5][8]=56;
   hold[5][9]=50;
 hold[5][10]=55;
 hold[5][11]=60;
 hold[5][12]=51;
 hold[5][13]=54;
 hold[5][14]=54;
 hold[5][15]=53;
   hold[8][0]=67;
   hold[8][1]=57;
   hold[8][2]=60;
   hold[8][3]=59;
   hold[8][4]=55;
   hold[8][5]=59;
   hold[8][6]=54;
   hold[8][7]=57;
   hold[8][8]=55;
   hold[8][9]=57;
   hold[8][10]=59;
   hold[8][11]=59;
   hold[8][12]=60;
   hold[8][13]=56;
   hold[8][14]=59;
   hold[8][15]=65;
   hold[4][0]=50;
   hold[4][1]=51;
   hold[4][2]=54;
   hold[4][3]=56;
   hold[4][4]=52;
   hold[4][5]=55;
   hold[4][6]=59;
   hold[4][7]=54;
   hold[4][8]=52;
   hold[4][9]=57;
   hold[4][10]=54;
   hold[4][11]=49;
   hold[4][12]=51;
   hold[4][13]=60;
   hold[4][14]=49;
   hold[4][15]=54;
   hold[3][0]=55;
   hold[3][1]=57;
   hold[3][2]=56;
   hold[3][3]=56;
   hold[3][4]=54;
   hold[3][5]=50;
   hold[3][6]=61;
   hold[3][7]=58;
   hold[3][8]=56;
   hold[3][9]=58;
   hold[3][10]=63;
   hold[3][11]=58;
   hold[3][12]=61;
   hold[3][13]=62;
   hold[3][14]=57;
   hold[3][15]=57;
   hold[1][0]=52;
   hold[1][1]=59;
   hold[1][2]=63;
   hold[1][3]=56;
   hold[1][4]=57;
   hold[1][5]=61;
   hold[1][6]=53;
   hold[1][7]=56;
   hold[1][8]=58;
   hold[1][9]=60;
   hold[1][10]=54;
   hold[1][11]=53;
   hold[1][12]=66;
   hold[1][13]=57;
   hold[1][14]=55;
   hold[1][15]=59;
   hold[14][0]=52;
   hold[14][1]=57;
   hold[14][2]=61;
   hold[14][3]=58;
   hold[14][4]=62;
   hold[14][5]=64;
   hold[14][6]=64;
   hold[14][7]=57;
   hold[14][8]=55;
   hold[14][9]=59;
   hold[14][10]=57;
   hold[14][11]=61;
   hold[14][12]=57;
   hold[14][13]=62;
   hold[14][14]=58;
   hold[14][15]=59;
   hold[0][0]=58;
   hold[0][1]=53;
   hold[0][2]=64;
   hold[0][3]=66;
   hold[0][4]=58;
   hold[0][5]=64;
   hold[0][6]=60;;
   hold[0][7]=64;
   hold[0][8]=68;
   hold[0][9]=58;
   hold[0][10]=60;
   hold[0][11]=63;
   hold[0][12]=61;
   hold[0][13]=63;
   hold[0][14]=59;
   hold[0][15]=69;
   hold[2][0]=54;
   hold[2][1]=60;
   hold[2][2]=61;
   hold[2][3]=50;
   hold[2][4]=57;
   hold[2][5]=54;
   hold[2][6]=54;
   hold[2][7]=59;
   hold[2][8]=55;
   hold[2][9]=56;
   hold[2][10]=63;
   hold[2][11]=58;
   hold[2][12]=58;
   hold[2][13]=60;
   hold[2][14]=57;
   hold[2][15]=57;

   read_configuration_file(filename+".txt",false);
   for(int islab=0; islab<15; islab++) {
     for (int ichip=0; ichip<16; ichip++) {
       detector.slab[0][islab].asu[0].skiroc[ichip].hold_delay=hold[islab][ichip];
     }
   }
   write_configuration_file("run_Settings_it9_holdavlues.txt");

}
