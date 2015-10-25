#include <CapacitiveSensor.h>
/*
 * Copyright 2015 Bram Rausch -- GPLv2, See license for reuse details
 * This code uses the capacitive touch library for Arduino that could be downloaded here:
 * http://playground.arduino.cc/Main/CapacitiveSensor
 */
CapacitiveSensor   sens1 = CapacitiveSensor(2,3);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   sens2 = CapacitiveSensor(2,4);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapacitiveSensor   sens3 = CapacitiveSensor(2,5);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil
CapacitiveSensor   sens4 = CapacitiveSensor(2,6);       // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil

int data0[4] = {0,0,0,0};
int data1[4] = {0,0,0,0};
int data2[4] = {0,0,0,0};
int data3[4] = {0,0,0,0};
long total[4];
int d0first = 0;
int d1first = 0;
int count;
int tapped = 0;

void setup(){
   Serial.begin(9600);
}

void loop(){
  for(int i = 0; i < 5; i++){ //take four samples
    count = 0;
    long start = millis();
    total[0] =  sens1.capacitiveSensor(30); //Measure capacitance for all pads
    total[1] =  sens2.capacitiveSensor(30);
    total[2] =  sens3.capacitiveSensor(30);
    total[3] =  sens4.capacitiveSensor(30);
    for(int z = 0; z < 4; z++){ //shift the data previous captured
      for(int q = 0; q < 5; q++){ 
        switch(z){
          case 0:
            data3[q] = data2[q];
          break;
          case 1:
            data2[q] = data1[q];
          break;
          case 2:
            data1[q] = data0[q];
          break;
        }
      }
    }
    for(int x = 0; x < 5; x++){ //This loop takes a look at all values
        if(total[x] > 200){ //if the value is bigger then 200 the pad is touched change this number to change sensitivity
          data0[x] = 1;
        } else{
          data0[x] = 0;
        }
        count = count+data0[x];
    }
    if(count == 0){
      if(tapped == 1){ //when released
        tapped = 0;  
      }  
    }
    if(tapped == 1){  //if there is a change dtected
      d0first = 0;
      d1first = 0;
      for(int x = 0; x < 5; x++){ 
        if(data1[x] == 1 && d0first == 0){
          d0first = x;
        }
        if(data0[x] == 1 && d1first == 0){
          d1first = x;
        }
      }
      if(d1first-d0first == 1 || d0first-d1first == 7){
        Serial.println("r");  //Returns movement to the right
      }else if(d0first-d1first == 1 || d1first-d0first == 7){
        Serial.println("l");  //Returns movement to the left
      }
    }
    
    if(count != 0){
      if(tapped != 1){ //when pressed
        tapped = 1;  
      }
    } 
    delay(10);
  }
}
