#include <stdio.h>

#include <stdint.h>
#include <math.h>


#define XSIZE 100
#define YSIZE 100
int main(void) {
uint32_t tag_mask1       =   0xffff8000;
  uint32_t index_mask1   =   0x00007fe0;
  uint32_t index_mask2   =   0x0000ffe0;
  uint32_t tag_mask2     =   0xfffe0000;
  uint32_t addr;
  uint32_t index;
  uint32_t tag;

  int x[XSIZE][YSIZE];
  int y[XSIZE][YSIZE];
  int z[XSIZE][YSIZE];
  int i, j, k;
  int hits1   = 0;
  int hits2   = 0;
  int misses1 = 0;
  int misses2 = 0;

  int r;
  int * temp; 

  /* Initialize x matrix */
  for (i = 0; i < XSIZE; i++) { 
    for (j = 0; j < YSIZE; j++) { 
      x[i][j] = 0;
    }
  }
  /* Initialize y matrix */
  for (i = 0; i < XSIZE; i++) {
    for (j = 0; j < YSIZE; j++) {
      y[i][j] = i + j;
    }
  }
  /* Initialize z matrix */
  for (i = 0; i < XSIZE; i++) {
    for (j = 0; j < YSIZE; j++) {
      z[i][j] = i + j;
    }
  }



  double miss_rate1,miss_rate2;

  int size1 = pow(2,10); //2^10 elements in teh array
  /******************************/
  int valid1[size1];
  uint32_t tags1[size1];
/********************************/

  int size2 = pow(2,11);//2^11 elements in the array
  int valid2[size2];
  uint32_t tags2[size2];
  /* Do matrix multiply */
  for (i = 0; i < XSIZE; i = i + 1) {
    for (j = 0; j < YSIZE; j = j + 1) {
      r = 0;
      for (k = 0; k < XSIZE; k = k + 1) {
        r = r + y[i][k] * z[k][j];
      }
      x[i][j] = r;
      //1. Everytime you reference x, y, or z you get the addr of that array element 
      temp = &x[i][j];  //gives me the addr of a given array element
      addr = (uint32_t) temp;

      //2. Break down the addr into tag, index, and offset fields
 
      index = ((addr & index_mask1) >> 5); //isolate index and shift
  

 
      tag = ((addr & tag_mask1) >> 15);//isolate tag and shift

      


      //Account for all misses and hits in Case 1
      if (valid1[index] == 1) {// if the valid bit is a 1, compare tags 
        if (tags1[index] == tag) { // if data is in the cache block that has the same tag
            hits1++; //due to spatial locality 
        }
        else { //tags are not the same
            misses1++; 
            tags1[index] = tag; //have to replace tag with updated tag
        }

      }
      else {  //data not already in cache
            valid1[index] = 1; //flip valid bit to 1
            tags1[index] = tag; //copy tag of addr into tag entry
            misses1++; //compulsory type miss since this is the first time you set the valid bit
      }
      
      
     

      index = ((addr & index_mask2) >> 6);
      tag = ((addr & tag_mask2) >> 17);
      //Account for all misses and hits in Case 2
      if (valid2[index] == 1) {// if the valid bit is a 1, compare tags 
        if (tags2[index] == tag) { // if data is in the cache block that has the same tag
            hits2++; //due to spatial locality 
        }
        else { //tags are not the same
            misses2++; 
            tags2[index] = tag; //have to replace tag with updated tag
        }

      }
      else {  //data not already in cache
            valid2[index] = 1; //flip valid bit to 1
            tags2[index] = tag; //copy tag of addr into tag entry
            misses2++; //compulsory type miss since this is the first time you set the valid bit
      }
      
    }
  }
  int total1 = misses1 + hits1;
  int total2 = misses2 + hits2;
  miss_rate1 = (misses1 / (double)(total1)) * 100;
  miss_rate2 = (misses2 / (double)(total2)) * 100;
  printf("first case: %.2f%\n", miss_rate1);
  printf("second case: %.2f%\n", miss_rate2);

}

