#include <stdbool.h>
#include <stdio.h>  /* printf */
#include <stdlib.h> /* qsort */

typedef struct RationalNumber{
  long long t;
  long long n;
}RationalNumber;

int isRationalNumberPositive(RationalNumber rn){
  if(rn.t > 0 && rn.n > 0)
    return 1;
  if(rn.t < 0 && rn.n < 0)
    return 1;

  return 0;
}

RationalNumber** make_matrix(int rows, int cols){
  RationalNumber** rm = malloc(rows*sizeof(RationalNumber *));
  for (int i = 0; i < rows; i++)
    rm[i] = malloc(cols*sizeof(RationalNumber));

  return rm;
}

void free_matrix(RationalNumber** rm, int rows){
  for (int i = 0; i < rows; i++) {
      free(rm[i]);
  }
  free(rm);
}

RationalNumber reduce(RationalNumber r){
  long long a = r.t;
  long long b = r.n;
  long long mgn;
  RationalNumber newr;
  if(a == 0){
    newr.t = 0;
    newr.n = 1;
    return newr;
  }
  int c = a % b;
  if (c < 0){
    c = -c;
    a = -a;
  }
  while(c != 0)
  {
      a = b;
      b = c;
      c = a % b;
  }
  mgn = b;
  newr.t = r.t/mgn;
  newr.n = r.n/mgn;
  if(newr.n < 0){
    newr.t = -newr.t;
    newr.n = -newr.n;
  }
  return newr;

};

RationalNumber subq(RationalNumber r1, RationalNumber r2){
  RationalNumber newrat;
  newrat.t = r1.t*r2.n - r2.t*r1.n;
  newrat.n = r1.n*r2.n;
  newrat = reduce(newrat);
  return newrat;
}

RationalNumber divq(RationalNumber r1, RationalNumber r2){
  RationalNumber newrat;
  newrat.t = r1.t*r2.n;
  newrat.n = r1.n*r2.t;
  newrat = reduce(newrat);
  return newrat;
}

void sort(RationalNumber** rm, int rows, int cols, int *counters){
  int i;
  int zercount = 0;
  int negcount = 0;
  int poscount = 0;
  //Beräknar antalet pos, neg och zer element i första kolonnen för att kunna skapa matriserna
  for( i = 0; i < rows; i++){
    if (rm[i][0].t == 0)
      zercount++;
    else if (isRationalNumberPositive(rm[i][0]))
      poscount++;
    else
      negcount++;
  }
	counters[0] = zercount;
	counters[1] = negcount;
	counters[2] = poscount;

  //Skapar/allocerar minne för tre stycken matriser att lägga pos, neg och zer elementen i
  //Lägger in pos, neg och zer rader i sina respektive matriser

  RationalNumber **rm_temp = make_matrix(rows, cols);

  int j = 0;    //zer
  int k = 0;    //pos
  int l = 0;    //neg
  for(int i = 0; i < rows; i++){
    RationalNumber temp = rm[i][0];

    if(temp.t == 0){
      rm_temp[poscount + negcount + j] = rm[i];
      j++;
    }
    else if(isRationalNumberPositive(temp)){
			rm_temp[k] = rm[i];
      k++;
    }
    else {
			rm_temp[poscount + l] = rm[i];
      l++;
    }
  }

  for(int i = 0; i < rows; i++)
    rm[i] = rm_temp[i];

    //free_matrix(rm_temp, rows);
		free(rm_temp);

}

void divFirstRow(RationalNumber** rm, int rows, int cols){
  int i, j;
  for (i = 0; i < rows; i++){
      RationalNumber rn = rm[i][0];
      for(j=0 ; j< cols; j++){
        rm[i][j] = divq(rm[i][j], rn);
      }
  }
}

void freeFirstCol(RationalNumber** rm, int rows, int  cols){
  int i, j;
  for (i = 0; i < rows; i++){
    for(j=1 ; j< cols-1; j++){
      rm[i][j].t = - rm[i][j].t;
    }
  }
}

RationalNumber** eliminateFirstCol(RationalNumber** new_rm, RationalNumber** non_zer_rm, RationalNumber** rm, int* counters, int rows, int cols){
  int nzer = counters[0];
  int nneg = counters[1];
  int npos = counters[2];

  int i,j,k;
  for(i = 0; i < npos; i++){
    for(k = 0; k < nneg; k++){
        for(j = 0; j < cols; j++)
          new_rm[i*nneg+k][j] = subq(rm[i][j+1], rm[npos+k][j+1]);
    }
  }
  if(nzer > 0){
    for(i = 0; i < nzer; i++){
      for(j = 0; j < cols; j++)
		    new_rm[npos*nneg+i] = rm[npos+nneg+i];
      }
  }
  for (i = 0; i < rows; i++){
    for(j = 1; j < cols-1; j++){
      new_rm[i][j].t = - new_rm[i][j].t;
    }
  }
  return new_rm;
}

int compare(RationalNumber rn1, RationalNumber rn2){
	float a = ((float)rn1.t)/rn1.n;
	float b = ((float)rn2.t)/rn2.n;
	if(a>b)
  //printf("a:%f\nb:%f",a,b);
		return 1;
	else
		return 0;
}

RationalNumber findMin(RationalNumber** rm, RationalNumber rn, int start, int end){
  int i;
  for(i = start; i < end; i++){
    if(compare(rn, rm[i][1]) == 1)
      rn = rm[i][1];
  }
  return rn;
}

RationalNumber findMax(RationalNumber** rm, RationalNumber rn, int start, int end){
  int i;
  for(i = start; i < end; i++){
    if(compare(rn, rm[i][1]) == 0)
      rn = rm[i][1];
  }
  return rn;
}

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
	int counters[3] = {0};
  RationalNumber** rm = make_matrix(rows, cols+1);
  for (size_t i = 0; i < rows; i++) {
	   for (size_t j = 0; j < cols; j++) {
		     RationalNumber ratA;
    		 ratA.t = a[i][j];
    		 ratA.n = 1;
    		 rm[i][j] = ratA;
	   }
     RationalNumber ratC;
	    ratC.t = c[i];
	    ratC.n = 1;
	    rm[i][cols] = ratC;
  }
  cols = cols + 1;
  sort(rm, rows, cols, counters);
	while(cols > 2){
      int nzer = counters[0];
		  int nneg = counters[1];
		  int npos = counters[2];
      if(npos*nneg + nzer <= 0)
        return 1;

      RationalNumber **non_zer_rm = make_matrix(npos+nneg, cols);
      for (int i = 0; i < (npos+nneg); i++)
        non_zer_rm[i] = rm[i];


		  if(npos+nneg > 0){
         divFirstRow(non_zer_rm, rows, cols);
  	     freeFirstCol(non_zer_rm, rows, cols);
      }

      RationalNumber** temp = rm;
      cols = cols - 1;
      rows = npos * nneg + nzer;
      RationalNumber **new_rm = make_matrix(rows, cols);
      new_rm = eliminateFirstCol(new_rm, non_zer_rm, rm, counters, rows, cols);
      free_matrix(non_zer_rm, npos+nneg);
      sort(new_rm,rows,cols,counters);
  		rm = new_rm;
      free(temp);

      //free_matrix(rm,rows);
      //RationalNumber **rm = make_matrix(rows, cols);
      //for(size_t i = 0; i < rows; i++){
        //  rm[i] = new_rm[i];
      //}
  		//sort(rm,rows,cols,counters);
      //free_matrix(new_rm,rows);
	}
  int nzer = counters[0];
  int nneg = counters[1];
  int npos = counters[2];

  RationalNumber **non_zer_rm = make_matrix(npos+nneg, cols);

  for (int i = 0; i < (npos+nneg); i++)
	  non_zer_rm[i] = rm[i];

  if(npos+nneg > 0)
    divFirstRow(non_zer_rm, rows, cols);

  RationalNumber B1;
  RationalNumber b1;
  RationalNumber q_min;

  B1.t = 2147483647;
  B1.n = 1;
  b1.t = -2147483647;
  b1.n = 1;
  q_min.t = 2147483647;
  q_min.n = 1;

	if(npos > 0)
    B1 = findMin(non_zer_rm, B1, 0, npos);
  if(nneg > 0)
    b1 = findMax(non_zer_rm, b1, npos, npos + nneg);
  if (nzer > 0)
    q_min = findMin(rm, q_min, nneg+npos, nneg+npos+nzer);

  free_matrix(non_zer_rm, npos+nneg);
  //free_matrix(rm, rows);
	free(rm);
	if (q_min.t <= 0)
		return 0;
	if(compare(B1, b1) == 1)
		return 1;
	else
		return 0;
}
