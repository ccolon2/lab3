#include <stdio.h>
#include <math.h>

#define XSIZE 100
#define YSIZE 100
#define CACHE_SIZE 2048

unsigned long int bit_extractor(unsigned long int number, int sz, int start) {
    return (((1 << sz) - 1) & (number >> (start - 1)));
}

int main(void) {
  int index_length = 11;   //32 byte cache : 10   128 byte cache: 11
  int index_start = 7;   //32 byte cache: 6    128 byte cache: 7
  int tag_length = 15;     //32 byte cache: 17   128 byte cache: 15
  int tag_start = 18;    //32 byte cache: 16   128 byte cache: 18

  int x[XSIZE][YSIZE];
  int y[XSIZE][YSIZE];
  int z[XSIZE][YSIZE];

  int i, j, k;

  int r;

  int valid_bits[CACHE_SIZE];
  int tag[CACHE_SIZE];



  /* Initialize x matrix */
  for(i=0; i<XSIZE; i++) {
    for(j=0; j<YSIZE; j++) {
      x[i][j] = 0;
    }
  }


  /* Initialize y matrix */
  for(i=0; i<XSIZE; i++) {
    for(j=0; j<YSIZE; j++) {
      y[i][j] = i + j;
    }
  }

  /* Initialize z matrix */
  for(i=0; i<XSIZE; i++) {
    for(j=0; j<YSIZE; j++) {
        z[i][j] = i + j;
    }
  }

  for(i=0; i<CACHE_SIZE; i++) {
    valid_bits[i] = 0;
  }

  for(i=0; i<CACHE_SIZE; i++) {
    tag[i] = 0;
  }
  unsigned long int x_beginning = bit_extractor(&x[0][0], index_length, index_start);
  unsigned long int y_beginning = bit_extractor(&y[0][0], index_length, index_start);
  unsigned long int z_beginning = bit_extractor(&z[0][0], index_length, index_start);

  //15 11 6
//0000 0000 0000 000|0  0000 0000 00|00 0000

//17 10 5
//0000 0000 0000 0000  0|000 0000 000|0 0000
  int miss = 0;
  int hits = 0;




  unsigned long int index_extracted;
  unsigned long int tag_extracted;


 



  /* Do matrix multiply */
  for(i=0; i<XSIZE; i=i+1) {
    for(j=0; j<YSIZE; j=j+1) {
      // r = 0;
      for(k=0; k<XSIZE; k=k+1) {
        x[i][j] = x[i][j] + y[i][k] * z[k][j];
        index_extracted = bit_extractor(&x[i][j], index_length, index_start);
        tag_extracted = bit_extractor(&x[i][j], tag_length, tag_start);
        //for all of X array cache
        if(valid_bits[x_beginning - index_extracted] == 1) { // if the valid bit is a 1, compare tags 
            if(tag[x_beginning - index_extracted] == tag_extracted) { // if data is in the cache block that has the same tag
                hits++; //due to spatial locality 
            }
            else {
                tag[x_beginning - index_extracted] = tag_extracted;//have to replace tag with updated tag
                miss++;
            }
        }
        else { //data not already in cache
            miss++; //compulsory type miss since this is the first time you set the valid bit
            valid_bits[x_beginning - index_extracted] = 1;//flip valid bit to 1
            tag[x_beginning - index_extracted] = tag_extracted;//copy tag of addr into tag entry
        }

        tag_extracted = bit_extractor(&y[i][k], tag_length, tag_start);
        index_extracted = bit_extractor(&y[i][k], index_length, index_start);
      
  
        if(valid_bits[y_beginning - index_extracted] == 1) {
            if(tag[y_beginning - index_extracted] == tag_extracted) {
                hits++;
            }
            else {
                tag[y_beginning - index_extracted] = tag_extracted;
                miss++;
            }
        }
        else {
            miss++;
            valid_bits[y_beginning - index_extracted] = 1;
            tag[y_beginning - index_extracted] = tag_extracted;
        }
        tag_extracted = bit_extractor(&z[k][j], tag_length, tag_start);
        index_extracted = bit_extractor(&z[k][j], index_length, index_start);
     
        if(valid_bits[z_beginning - index_extracted] == 1) {
            if(tag[z_beginning - index_extracted] == tag_extracted) {
                hits++;
            }
            else {
                tag[z_beginning - index_extracted] = tag_extracted;
                miss++;
            }
        }
        else {
            miss++;
            valid_bits[z_beginning - index_extracted] = 1;
            tag[z_beginning - index_extracted] = tag_extracted;
        }
      }
      // x[i][j] = r;
    }
  }

  float miss_rate = (miss/(float)(miss+hits))*100;
  printf("miss rate: %.2f%  \n", miss_rate);




}


