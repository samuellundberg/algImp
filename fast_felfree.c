#include <stdbool.h>
#include <stdio.h>  /* printf */
#include <stdlib.h> /* qsort */

typedef struct RationalNumber{
  long long numerator;
  long long denumerator;
}RationalNumber;

int isRationalNumberPositive(RationalNumber rn){
  if(rn.numerator > 0 && rn.denumerator > 0)
    return 1;
  if(rn.numerator < 0 && rn.denumerator < 0)
    return 1;

  return 0;
}

RationalNumber reduce(RationalNumber r){
  long long a = r.numerator;
  long long b = r.denumerator;
  long long mgn;
  RationalNumber newr;
  if(a == 0){
    newr.numerator = 0;
    newr.denumerator = 1;
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
  newr.numerator = r.numerator/mgn;
  newr.denumerator = r.denumerator/mgn;
  if(newr.denumerator < 0){
    newr.numerator = -newr.numerator;
    newr.denumerator = -newr.denumerator;
  }
  return newr;

};

RationalNumber subq(RationalNumber r1, RationalNumber r2){
  RationalNumber newrat;
  newrat.numerator = r1.numerator*r2.denumerator - r2.numerator*r1.denumerator;
  newrat.denumerator = r1.denumerator*r2.denumerator;
  newrat = reduce(newrat);
  return newrat;
}

RationalNumber divq(RationalNumber r1, RationalNumber r2){
  RationalNumber newrat;
  newrat.numerator = r1.numerator*r2.denumerator;
  newrat.denumerator = r1.denumerator*r2.numerator;
  newrat = reduce(newrat);
  return newrat;
}

typedef struct RationalMatrix{
  int rows;                         // Antal rader
  int cols;                         // Antal kolonner
  struct RationalNumber** data;     // En pekare till en vektor med rows perkare till vektorer; där varje vektor innehåller cols rationalNumber
}RationalMatrix;

RationalMatrix* make_rationalMatrix(int n_rows, int n_cols){
  RationalMatrix* rm = malloc(sizeof(RationalMatrix));
  rm->rows = n_rows;
  rm->cols = n_cols;
  RationalNumber** data = malloc(sizeof(RationalNumber*) * n_rows);
  for (int i = 0; i < n_rows; i++){
    data[i] = calloc(n_cols, sizeof(RationalNumber));
  }
  rm->data = data;
  return rm;
}

void free_rationalMatrix(RationalMatrix* rm){
	int rows = rm->rows;
	for (int i = 0; i<rows;i++){
		free(rm->data[i]);
	}
	//free(rm->data);
	free(rm);
}

void mergeAandC2(RationalMatrix* AC, int rows, int cols, signed char A[rows][cols], signed char C[rows]){
  int i,j;
  for(i = 0; i < rows; i++){
    AC->data[i][cols].numerator = C[i];
    AC->data[i][cols].denumerator = 1;
    for(j = 0; j < cols; j++){
      AC->data[i][j].numerator = A[i][j];
      AC->data[i][j].denumerator = 1;
    }
  }
}

void sort(RationalMatrix* rm, int rows, int cols, int *counters){
  int i;
  int zercount = 0;
  int negcount = 0;
  int poscount = 0;
  //Beräknar antalet pos, neg och zer element i första kolonnen för att kunna skapa matriserna
  for( i = 0; i < rows; i++){
    if (rm->data[i][0].numerator == 0)
      zercount++;
    else if (isRationalNumberPositive(rm->data[i][0]))
      poscount++;
    else
      negcount++;
  }
	counters[0] = zercount;
	counters[1] = negcount;
	counters[2] = poscount;

  //Skapar/allocerar minne för tre stycken matriser att lägga pos, neg och zer elementen i
  //RationalMatrix* rm_zer = make_rationalMatrix(zercount, cols);
  //RationalMatrix* rm_neg = make_rationalMatrix(negcount, cols);
  //RationalMatrix* rm_pos = make_rationalMatrix(poscount, cols);
  RationalMatrix* rm_temp = make_rationalMatrix(rows, cols);
//Lägger in pos, neg och zer rader i sina respektive matriser
	int j = 0;    //zer
  int k = 0;    //pos
  int l = 0;    //neg
  for(int i = 0; i < rows; i++){
    RationalNumber temp = rm->data[i][0];

    if(temp.numerator == 0){
      rm_temp->data[poscount + negcount + j] = rm->data[i];
      j++;
    }
    else if(isRationalNumberPositive(temp)){
			rm_temp->data[k] = rm->data[i];
      k++;
    }
    else {
			rm_temp->data[poscount + l] = rm->data[i];
      l++;
    }
  }

  //free_rationalMatrix(rm);
  //RationalMatrix* rm = make_rationalMatrix(rows, cols);
  for(int i = 0; i < rows; i++)
    rm->data[i] = rm_temp->data[i];

  free(rm_temp);




  //Lägger in elementen igen i matrisen rm, fast i rätt ordning
  //for(i = 0; i<poscount; i++)
    //rm->data[i] = rm_pos->data[i];
  //for(i = 0; i<negcount; i++)
    //rm->data[poscount + i] = rm_neg->data[i];
    //Kan vara så att vi kan skita i den här loopen kommer inte ihåg ifall vi ändå ska ta bort 0elementen dirrekt efter detta
  //for(i = 0; i<zercount; i++)
    //rm->data[poscount + negcount + i] = rm_zer->data[i];


    //if(zercount > 0)
      //free_rationalMatrix(rm_zer);

    //if(negcount > 0)
      //  free_rationalMatrix(rm_neg);
	//free_rationalMatrix(rm_zer);
	//free_rationalMatrix(rm_pos);
	//free_rationalMatrix(rm_neg);
}

RationalMatrix* assemRationallMatrices(RationalMatrix* new_rm, RationalMatrix* rm, int *counters){
  int i;
	int nneg = counters[1];
	int npos = counters[2];

  for(i = 0; i < npos; i++)
    new_rm->data[i] = rm->data[i];

  for(i = 0; i < nneg; i++)
    new_rm->data[npos + i] = rm->data[npos + i];

  return new_rm;
}

void divFirstRow(RationalMatrix* rm){
  int i, j;
  int cols = rm->cols;
  int rows = rm->rows;
    for (i = 0; i < rows; i++){
      RationalNumber rn = rm->data[i][0];
      for(j=0 ; j< cols; j++){
        rm->data[i][j] = divq(rm->data[i][j], rn);
      }
  }
}

void freeFirstCol(RationalMatrix* rm){
  int i, j;
  int cols = rm->cols;
  int rows = rm->rows;
  for (i = 0; i < rows; i++){
    for(j=1 ; j< cols-1; j++){
      rm->data[i][j].numerator = - rm->data[i][j].numerator;
    }
  }
}

RationalMatrix* eliminateFirstCol(RationalMatrix* new_rm, RationalMatrix* non_zer_rm, RationalMatrix* rm, int npos, int nneg, int nzer){
  int i,j,k;
  int cols = non_zer_rm->cols;
  for(i = 0; i < npos; i++){
    for(k = 0; k < nneg; k++){
        for(j = 0; j < cols-1; j++)
          new_rm->data[i*nneg+k][j] = subq(non_zer_rm->data[i][j+1], non_zer_rm->data[npos+k][j+1]);
    }
  }

  if(nzer > 0){
    for(i = 0; i < nzer; i++)
		new_rm->data[npos*nneg+i] = rm->data[npos+nneg+i];
  }

  int r = new_rm->rows;
  int c = new_rm->cols;
  for (i = 0; i < r; i++){
    for(j = 0; j < c-1; j++){
      new_rm->data[i][j].numerator = - new_rm->data[i][j].numerator;
    }
  }

  return new_rm;
}

int compare(RationalNumber rn1, RationalNumber rn2){
	float a = ((float)rn1.numerator)/rn1.denumerator;
	float b = ((float)rn2.numerator)/rn2.denumerator;
	if(a>b)
  //printf("a:%f\nb:%f",a,b);
		return 1;
	else
		return 0;
}

RationalNumber findMin(RationalMatrix* rm, RationalNumber rn, int start, int end){
  int i;
  for(i = start; i < end; i++){
    if(compare(rn, rm->data[i][1]) == 1)
      rn = rm->data[i][1];
  }
  return rn;
}

RationalNumber findMax(RationalMatrix* rm, RationalNumber rn, int start, int end){
  int i;
  for(i = start; i < end; i++){
    if(compare(rn, rm->data[i][1]) == 0)
      rn = rm->data[i][1];
  }
  return rn;
}

void extractRationalMatrix(RationalMatrix* new_rm, RationalMatrix* rm, int start, int end){
	int i;
	int diff = end - start;
	for (i = 0; i<diff ;i++)
		new_rm->data[i]=rm->data[start + i];

}

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
	int counters[3] = {0};

	RationalMatrix* rm =  make_rationalMatrix(rows,cols + 1);
	mergeAandC2(rm, rows, cols, a, c);
	cols = cols + 1;
	sort(rm, rows, cols, counters);

	while(cols > 2){
		int nzer = counters[0];
		int nneg = counters[1];
		int npos = counters[2];

		if(npos*nneg + nzer <= 0)
			return 1;

		RationalMatrix* non_zer_rm = make_rationalMatrix(npos+nneg, cols);

		if(npos == 0)
			extractRationalMatrix(non_zer_rm, rm, 0, nneg);
		else if(nneg == 0)
			extractRationalMatrix(non_zer_rm, rm, 0, npos);
		else
		non_zer_rm = assemRationallMatrices(non_zer_rm, rm, counters);

		if(npos+nneg > 0){
      divFirstRow(non_zer_rm);
  	  freeFirstCol(non_zer_rm);
    }

		cols = cols - 1;
		rows = npos * nneg + nzer;
		RationalMatrix* new_rm = make_rationalMatrix(rows, cols);
		new_rm = eliminateFirstCol(new_rm, non_zer_rm, rm, npos, nneg, nzer);
		free_rationalMatrix(non_zer_rm);
    free_rationalMatrix(rm);
    RationalMatrix* rm = make_rationalMatrix(rows, cols);

    int r = rows;
    //int c = cols;
    int i;
    for(i = 0; i < r; i++){
      //for(j = 0; j < c; j++){
        rm->data[i] = new_rm->data[i];
      //  rm->data[i][j].denumerator = new_rm->data[i][j].denumerator;
      //}
    }
		sort(rm,rows,cols,counters);
    free_rationalMatrix(new_rm);

	}

  //RationalMatrix* temp = rm;
  //cols = cols - 1;
  //rows = npos * nneg + nzer;
  //RationalMatrix* new_rm = make_rationalMatrix(rows, cols);

  //new_rm = eliminateFirstCol(new_rm, non_zer_rm, temp, npos, nneg, nzer);
  //free_rationalMatrix(non_zer_rm);
  //sort(new_rm,rows,cols,counters);
  //rm = new_rm;
  //free(temp);

		int nzer = counters[0];
		int nneg = counters[1];
		int npos = counters[2];

		RationalMatrix* non_zer_rm = make_rationalMatrix(npos+nneg , cols);
		non_zer_rm = assemRationallMatrices(non_zer_rm, rm, counters);

		if(npos+nneg > 0)
      divFirstRow(non_zer_rm);

    RationalNumber B1;// = malloc(sizeof(int)*2);
    RationalNumber b1;// = malloc(sizeof(int)*2);
    RationalNumber q_min;// = malloc(sizeof(int)*2);

    B1.numerator = 2147483647;    //INT_MAX
    B1.denumerator = 1;
    b1.numerator = -2147483647;   //INT_MIN
    b1.denumerator = 1;
    q_min.numerator = 2147483647;          //INT_MIN
    q_min.denumerator = 1;

		if(npos > 0)
      B1 = findMin(non_zer_rm, B1, 0, npos);
    if(nneg > 0)
      b1 = findMax(non_zer_rm, b1, npos, npos + nneg);
    if (nzer > 0)
      q_min = findMin(rm, q_min, nneg+npos, nneg+npos+nzer);

    free_rationalMatrix(non_zer_rm);
    free_rationalMatrix(rm);

		if (q_min.numerator <= 0)
			return 0;

		if(compare(B1, b1) == 1)
			return 1;
		else
			return 0;

}
