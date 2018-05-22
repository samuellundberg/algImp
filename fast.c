#include <stdbool.h>
#include <stdio.h>  /* printf */
#include <stdlib.h> /* qsort */
#define ARRAYSIZE(A) (sizeof(A) / sizeof(A[0]))

//En strukt för hur rationella tal beskrivs
typedef struct RationalNumber{
  long long numerator;
  long long denumerator;
}RationalNumber;

int isRationalNumberZero(RationalNumber rn){
  if(rn.numerator == 0)
    return 1;

  return 0;
}

/*Kollar om ett rationellt tal är positivt.
  Returnerar 1 om pos och 0 annars*/
int isRationalNumberPositive(RationalNumber rn){
  if(rn.numerator > 0 && rn.denumerator > 0)
    return 1;
  if(rn.numerator < 0 && rn.denumerator < 0)
    return 1;

  return 0;
}
//kan va kul att ha
int bp(int a, int b){
  printf("%d HAAAAAY %d\n",a, b);
  return 0;
}

/* Räkneregler med rationella tal, från lab1*/
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

RationalNumber addq(RationalNumber r1, RationalNumber r2){
  RationalNumber newrat;
  newrat.numerator = r1.numerator*r2.denumerator + r2.numerator*r1.denumerator;
  newrat.denumerator = r1.denumerator*r2.denumerator;
  newrat = reduce(newrat);
  return newrat;
}

RationalNumber subq(RationalNumber r1, RationalNumber r2){
  RationalNumber newrat;
  newrat.numerator = r1.numerator*r2.denumerator - r2.numerator*r1.denumerator;
  newrat.denumerator = r1.denumerator*r2.denumerator;
  newrat = reduce(newrat);
  return newrat;
}

RationalNumber mulq(RationalNumber r1, RationalNumber r2){
  RationalNumber newrat;
  newrat.numerator = r1.numerator*r2.numerator;
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

/* En strukt som beskriver en matris av rationella tal */
typedef struct RationalMatrix{
  int rows;                         // Antal rader
  int cols;                         // Antal kolonner
  struct RationalNumber** data;     // En pekare till en vektor med rows perkare till vektorer; där varje vektor innehåller cols rationalNumber
}RationalMatrix;

/* Allokerar minne och skapar en matris med ratinella tal */
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
  int i;
  int rows = rm->rows;
  for (i=0; i<rows; i++)
      free(rm->data[i]);

  free(rm);
}

RationalMatrix* convertToRationlNumbers(RationalMatrix* rm, int rows, int cols, signed char matrix[][cols]){
  int i, j;
  for(i = 0; i < rows; i++){
      for(j = 0; j < cols; j++){
          rm->data[i][j].numerator = matrix[i][j];
          rm->data[i][j].denumerator = 1;
      }
  }
  return rm;
}

RationalMatrix* convertC(RationalMatrix* rm, int rows, signed char vector[]){
  int i;
  for(i = 0; i < rows; i++){
          rm->data[i][0].numerator = vector[i];
          rm->data[i][0].denumerator = 1;
  }
  return rm;
}

/* Steg 2 är att sortera första kolonnen i matrisen med rationella tal.
   Använder oss av mergesort för att sortera listan.
   Komplexitet: O(n log n)*/


RationalMatrix* assemRationallMatrices(RationalMatrix* new_rm, RationalMatrix* rm1, RationalMatrix* rm2){
  int rm1_rows = rm1->rows;
  int rm2_rows = rm2->rows;
  int i;
  for(i = 0; i < rm1_rows; i++)
    new_rm->data[i] = rm1->data[i];

  for(i = 0; i < rm2_rows; i++)
    new_rm->data[rm1_rows + i] = rm2->data[i];

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

RationalMatrix* eliminateFirstCol(RationalMatrix* new_rm, RationalMatrix* old_rm, RationalMatrix* rm_zer, int npos, int nneg, int nzer){
  int i,j,k;
  int cols = old_rm->cols;
  for(i = 0; i < npos; i++){
    for(k = 0; k < nneg; k++){
        for(j = 0; j < cols-1; j++)
          new_rm->data[i*nneg+k][j] = subq(old_rm->data[i][j+1], old_rm->data[npos+k][j+1]);
    }
  }

  if(nzer > 0){
    for(i = 0; i < nzer; i++){
      for(j = 1; j < cols; j++){
        RationalNumber rn = rm_zer->data[i][j];
        //rn.numerator = - rn.numerator;
        new_rm->data[npos * nneg + i][j-1] = rn;
      }
    }
  }

  //testar här
  int r = new_rm->rows;
  int c = new_rm->cols;
  for (i = 0; i < r; i++){
    for(j = 0; j < c-1; j++){
      new_rm->data[i][j].numerator = - new_rm->data[i][j].numerator;
    }
  }

  return new_rm;
}

RationalMatrix* mergeAandC(RationalMatrix* AC, RationalMatrix* A, RationalMatrix* C){
  int i;
  int cols = A->cols;
  int rows = A->rows;
  for(i = 0; i < rows; i++){
    AC->data[i] = A->data[i];
    AC->data[i][cols] = C->data[i][0];
  }
  return AC;
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

typedef struct RationalMatrices{
  RationalMatrix* rm_zer;
  RationalMatrix* rm_neg;
  RationalMatrix* rm_pos;
}RationalMatrices;

/* Här är sortering funktionen, den lägger alla pos, neg, zer i
   var sin egen matris och sen lägger ihop den i orginalmatrisen.
   Funktionen sorterar första kolonnen och returnerar zercount,
   negcount och poscount i en vektor som heter counters.
   Tror den har tidskomplexitet O(n)
   */
void sort(RationalMatrix* rm, int rows, int *counters, RationalMatrices *rms){
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
  //Skapar/allocerar minne för tre stycken matriser att lägga pos, neg och zer elementen i
  int rm_cols = rm->cols;

  RationalMatrix* rm_zer = make_rationalMatrix(zercount, rm_cols);
  RationalMatrix* rm_neg = make_rationalMatrix(negcount, rm_cols);
  RationalMatrix* rm_pos = make_rationalMatrix(poscount, rm_cols);
	int j = 0;
	int k = 0;
	int l = 0;
  for(int i = 0; i < rows; i++){
    RationalNumber temp = rm->data[i][0];

    if(temp.numerator == 0){
      rm_zer->data[j] = rm->data[i];
      j++;
    }
    else if(isRationalNumberPositive(temp)){
			rm_pos->data[l] = rm->data[i];
      l++;
    }
    else {
			rm_neg->data[k] = rm->data[i];
      k++;
    }
  }
  rms->rm_zer = rm_zer;
  rms->rm_neg = rm_neg;
  rms->rm_pos = rm_pos;

  //Lägger in elementen igen i matrisen rm, fast i rätt ordning
  for(i = 0; i<poscount; i++)
    rm->data[i] = rm_pos->data[i];
  for(i = 0; i<negcount; i++)
    rm->data[poscount + i] = rm_neg->data[i];
    //Kan vara så att vi kan skita i den här loopen kommer inte ihåg ifall vi ändå ska ta bort 0elementen dirrekt efter detta
  for(i = 0; i<zercount; i++)
    rm->data[poscount + negcount + i] = rm_zer->data[i];

  counters[0] = zercount;
  counters[1] = negcount;
  counters[2] = poscount;
}

// return 1 om rn1>rn2, annars 0
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

//HÄR BÖRJAR MAIN
bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{

	  int counters[3] = {0};        //Skapar en 1x3 vektor för att spara antal pos, neg, zer

	/*-------HÄR BÖRJAR ALGORITMEN-------*/

    RationalMatrix* rm =  make_rationalMatrix(rows,cols + 1);
    mergeAandC2(rm, rows, cols, a, c);
    cols = cols + 1;
    RationalMatrices *rms = (RationalMatrices *) malloc (sizeof(RationalMatrices));
	  sort(rm, rows, counters, rms);

	 while(cols > 2){
	    int nzer = counters[0];
	    int nneg = counters[1];
	    int npos = counters[2];

	    if(npos*nneg + nzer <= 0)
        return 1;

	    RationalMatrix* non_zer_rm = make_rationalMatrix(npos+nneg, cols);
	    if(npos == 0)
	      non_zer_rm = rms->rm_neg;
	    else if(nneg == 0)
	      non_zer_rm = rms->rm_pos;
	    else
	      non_zer_rm = assemRationallMatrices(non_zer_rm, rms->rm_pos, rms->rm_neg);

      if(npos+nneg > 0){
        divFirstRow(non_zer_rm); //kommer det in nollor här?
  	    freeFirstCol(non_zer_rm);
      }
	    cols = cols - 1;
      rows = npos * nneg + nzer;
	    RationalMatrix* new_rm = make_rationalMatrix(rows, cols);
	    new_rm = eliminateFirstCol(new_rm, non_zer_rm, rms->rm_zer, npos, nneg, nzer);

      free_rationalMatrix(non_zer_rm);
      free(rms->rm_neg);
      free(rms->rm_pos);
      free(rms->rm_zer);
      sort(new_rm, rows, counters, rms);
	    rm = new_rm;
	    //free(new_rm);
    }
    //Nu har vi bara två kolloner kvar så dags att bedömma dem
    int nzer = counters[0];
    int nneg = counters[1];
    int npos = counters[2];

    RationalMatrix* non_zer_rm = make_rationalMatrix(npos+nneg , cols);
    non_zer_rm = assemRationallMatrices(non_zer_rm, rms->rm_pos, rms->rm_neg);
    RationalMatrix* zer_rm = make_rationalMatrix(nzer, cols);
    zer_rm = rms->rm_zer;

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
      q_min = findMin(zer_rm, q_min, 0, nzer);

    free_rationalMatrix(zer_rm);
    free_rationalMatrix(rm);
    free(rms);

    if (q_min.numerator <= 0)
      return 0;

    if(compare(B1, b1) == 1)
      return 1;
    else
      return 0;
}
