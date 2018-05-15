#include <stdbool.h>
#include <stdio.h>  /* printf */
#include <stdlib.h> /* qsort */
#define ARRAYSIZE(A) (sizeof(A) / sizeof(A[0]))


/* --- innehållsförteckning---*/
/*  1. printMatrix (skriver ut en int matris)
    2. struct RationalNumber (sparar rationella tal som numerator och denumerator)
      2.1 isRationalNumberZero (kollar om RationalNumber är = 0)
      2.2 isRationalNumberNegative (kollar om RationalNumber är < 0)
      2.3 isRationalNumberPositive (kollar om RationalNumber är > 0)
      2.4 reduce (reducerar RationalNumber till minsta nämnare)
      2.5 addq (från lab1)
      2.6 subq (från lab1)
      2.7 multq (från lab1)
      2.8 divq (från lab1)
    3. struct RationalMatrix (sparar antal rader och kolonner samt en pekarvektor till en pekarvektor som pekar på ett Rationellt tal. Alltså en matris med rationella tal)
      3.1 make_rationalMatrix (allokerar minne i heapen till en rationell matris)
      3.2 print_rationalMatrix (Skriver ut en rationell matris)
      3.3 convertToRationlNumbers (Gör om en intMatris till en matris med rationella tal)
      3.4 printFirstCol (Skirver bara ut första kolonnen i matrisen, brukar inte använda denna)
      3.5 assemRationallMatrices(Jämför varje pos rad med varje neg rad i 2 rationella matriser när vi ställer upp olikheterna för pos och neg)
      3.6 divFirstRow (dividerar alla rationella tal i en rad med första rationella talet i raden, gör detta för en hel matris)
      3.7 freeFirstCol (byter tecken på alla rationella tal i en rad utom det första, vilket motsvara att flytta till andra sidan likhetstecknet)
      3.8 eliminateFirstCol (tar bort första kolonnen ut matrisen)
      3.9 mergeAandC (Lägger ihop matriserna A och C)
    4. struct RationalMatrices (Är till för att spara de tre matriserna som har pos, neg eller zer i första kolonnen)
      4.1 sort (Sorterar talen så poitiva på toppen sen negativa och sist nollelement, ger de tre matriserna samt antal npos nneg och nzer)'
    5. compareMax (Jämför 2 rationella tal och returnerar 1 om det första man skickar in är störst)
       compareMin (Jämför 2 rationella tal och returnerar 1 om det första man skickar in är minst)
    6. findMin (Hittar det minsta talet i kolonn 2 i en rationell matris, används för att hitta B1)
       findMax (Hittar det största talet i kolonn 2 i en rationell matris, används för att hitta b1)*/
//Skriver ut en matris
void printMatrix (int rows, int cols, int matrix[][cols]){
  int i, j;

  for(i = 0; i < rows; i++)
  {
        for(j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
  }
}

/* Steg 1 är att ändra indatan från en int matris till en
   matris med rationella tal. */

/* En strukt för hur rationella tal beskrivs */
typedef struct RationalNumber{
  int numerator;
  int denumerator;
}RationalNumber;

int isRationalNumberZero(RationalNumber rn){
  if(rn.numerator == 0)
    return 1;

  return 0;
}

/*Kollar om ett rationellt tal är negativt.
  Returnerar 1 om neg och 0 annars*/
int isRationalNumberNegative(RationalNumber rn){
  if(rn.numerator > 0 && rn.denumerator < 0)
    return 1;
  else if(rn.numerator < 0 && rn.denumerator > 0)
    return 1;
  else
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
int bp(int a, int b){
  printf("%d HAAAAAY %d\n",a, b);
  return 0;
}

/* Räkneregler med rationella tal, från lab1*/
RationalNumber reduce(RationalNumber r){
  int a = r.numerator;
  int b = r.denumerator;
  int mgn;
  RationalNumber newr;

  if(a == 0){
    b = 1;
  }
  else if(b == 0){
    a = 2147483647;
    b = 1;
  }
//  bp(a,b);
  int c = a % b;

  if(c==0){
    a = a/b;
    b = 1;
  }

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
  //newrat = reduce(newrat);

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

/* Skriver ut matrisen med rationella tal */
void print_rationalMatrix(RationalMatrix* rm){
  for (int i = 0; i < rm->rows; i++){
    for (int j = 0; j< rm->cols; j++){
      printf("%d / %d   ", rm->data[i][j].numerator, rm->data[i][j].denumerator);
    }
    printf("\n");
  }
  printf("\n");
}

/* Funktionen som konverterar matris av ints till rationella tal
   obs! Jag har inte lyckats lösa så att matrisen som man skickar
   kan vara av vilken storlek som helst utan det måste göras manuellt
   för tillfället, därav 2an i int matrix[][2] som inparameter*/
RationalMatrix* convertToRationlNumbers(int rows, int cols, signed char matrix[][cols]){
  int i, j;
  RationalMatrix* rm = make_rationalMatrix(rows, cols);

  for(i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            rm->data[i][j].numerator = matrix[i][j];
            rm->data[i][j].denumerator = 1;
        }
  }
  return rm;
}

RationalMatrix* convertC(int rows, signed char vector[]){
  int i;
  RationalMatrix* rm = make_rationalMatrix(rows, 1);

  for(i = 0; i < rows; i++){
          rm->data[i][1].numerator = vector[i];
          rm->data[i][1].denumerator = 1;
  }
  return rm;
}

/* Steg 2 är att sortera första kolonnen i matrisen med rationella tal.
   Använder oss av mergesort för att sortera listan.
   Komplexitet: O(n log n)*/


/* Funktion för att skriva ut första kolonnen i matrisen
   med rationella tal */
//varför void-pekare, terminal ber om en return
void printFirstCol(RationalMatrix* rm){
  for (int i = 0; i < rm->rows; i++){
      printf("%d / %d \n", rm->data[i][0].numerator, rm->data[i][0].denumerator);
  }
}


RationalMatrix* assemRationallMatrices(RationalMatrix* rm1, RationalMatrix* rm2){
  int rm1_cols = rm1->cols;
  int rm2_cols = rm2->cols;

	//Varför har vi denna if-satsen, måste väl vara samma?
  if (rm1_cols == rm2_cols){
    int rm1_rows = rm1->rows;
    int rm2_rows = rm2->rows;
    //int new_cols = rm1_cols + rm2_cols;
    RationalMatrix* new_rm = make_rationalMatrix(rm1_rows + rm2_rows, rm1_cols);

    int i;
    for(i = 0; i < rm1_rows; i++)
      new_rm->data[i] = rm1->data[i];

    for(i = 0; i < rm2_rows; i++)
      new_rm->data[rm1_rows + i] = rm2->data[i];

      return new_rm;
  }
  else
    return NULL;
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

RationalMatrix* eliminateFirstCol(RationalMatrix* old_rm, RationalMatrix* rm_zer, int npos, int nneg, int nzer){
  int i,j,k;
  int cols = old_rm->cols;
  int new_rows = npos * nneg + nzer;
  RationalMatrix* new_rm = make_rationalMatrix(new_rows, cols-1);

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
        rn.numerator = - rn.numerator;
        new_rm->data[npos * nneg + i][j-1] = rn;
        //new_rm->data[npos + nneg + 1 + i][j] = rm_zer->data[i][j];
      }
    }
  }
  return new_rm;
}

RationalMatrix* mergeAandC(RationalMatrix* A, RationalMatrix* C){
  int i;
  int cols = A->cols;
  int rows = A->rows;

  RationalMatrix* AC = make_rationalMatrix(rows, cols + 1);

  for(i = 0; i < rows; i++){
    AC->data[i] = A->data[i];
    AC->data[i][cols] = C->data[i][0];
  }
  return AC;
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
    else if (isRationalNumberNegative(rm->data[i][0]))
      negcount++;
    else
      poscount++;
  }


  //Skapar/allocerar minne för tre stycken matriser att lägga pos, neg och zer elementen i
  int rm_cols = rm->cols;

  RationalMatrix* rm_zer = make_rationalMatrix(zercount, rm_cols);
  // if (negcount>0)
  RationalMatrix* rm_neg = make_rationalMatrix(negcount, rm_cols);
  // if (poscount>0)
  RationalMatrix* rm_pos = make_rationalMatrix(poscount, rm_cols);


//Lägger in pos, neg och zer rader i sina respektive matriser
	int j = 0;
	int k = 0;
	int l = 0;
  for(int i = 0; i < rows; i++){
    RationalNumber temp = rm->data[i][0];

    if(temp.numerator == 0){
      rm_zer->data[j] = rm->data[i];
      j++;
    }
    else if(isRationalNumberNegative(temp)){
      rm_neg->data[k] = rm->data[i];
      k++;
    }
    else {
      rm_pos->data[l] = rm->data[i];
      l++;
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

  // counters = {zercount, negcount, poscount};
  counters[0] = zercount;
  counters[1] = negcount;
  counters[2] = poscount;

}


int compareMax(RationalNumber rn1, RationalNumber rn2){
	if(isRationalNumberPositive(rn1) == 1 && isRationalNumberPositive(rn2) == 1){
	  if (abs(rn1.numerator * rn2.denumerator) >  abs(rn2.numerator * rn1.denumerator))
	    return 1;
	  else
	    return 0;
	}
	if(isRationalNumberNegative(rn1) == 1 && isRationalNumberNegative(rn2) == 1){
			if (abs(rn1.numerator * rn2.denumerator) <  abs(rn2.numerator * rn1.denumerator))
			return 1;
	  else
	    return 0;
	}
	if(isRationalNumberPositive(rn1))
		return 1;

	return 0;

}
//1 om rn1 < rn2
int compareMin(RationalNumber rn1, RationalNumber rn2){
	if (isRationalNumberPositive(rn1) == 1 && isRationalNumberPositive(rn2) == 1){
		if (abs(rn1.numerator * rn2.denumerator) <  abs(rn2.numerator * rn1.denumerator))
			return 1;
		else
			return 0;
	}
	if (isRationalNumberNegative(rn1) == 1 && isRationalNumberNegative(rn2) == 1){
		if (abs(rn1.numerator * rn2.denumerator) >  abs(rn2.numerator * rn1.denumerator))
			return 1;
		else
			return 0;
	}
	if (isRationalNumberPositive(rn1))
		return 0;

	return 1;
}

RationalNumber findMin(RationalMatrix* rm, RationalNumber rn, int start, int end){
  int i;
  rn = rm->data[0][1];
  for(i = start; i < end; i++){
    if(compareMin(rn, rm->data[i][1]) == 0)
      rn = rm->data[i][1];
  }
  return rn;
}

RationalNumber findMax(RationalMatrix* rm, RationalNumber rn, int start, int end){
  int i;
  rn = rm->data[0][1];
  for(i = start; i < end; i++){
    if(compareMax(rn, rm->data[i][1]) == 0)
      rn = rm->data[i][1];
  }
  return rn;
}

//HÄR BÖRJAR MAIN
bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{

	  int counters[3] = {0};        //Skapar en 1x3 vektor för att spara antal pos, neg, zer
	  RationalMatrices *rms = (RationalMatrices *) malloc (sizeof(RationalMatrices));

	/*-------HÄR BÖRJAR ALGORITMEN-------*/
		RationalMatrix* rma =  make_rationalMatrix(rows, cols);
		RationalMatrix* rmc =  make_rationalMatrix(rows, 1);
		rma = convertToRationlNumbers(rows, cols, a);
		rmc = convertC(rows, c);
		//convertToRationlNumbers(int rows, int cols, int matrix[][cols])

	  RationalMatrix* rm =  make_rationalMatrix(rows,cols + 1);
	  rm = mergeAandC(rma, rmc); 	//gör så merge tar icke-rat a och c
		free(rma);
		free(rmc);
	  //print_rationalMatrix(rm); // det är väldit skumt men tar man bort denna raden fungerar inte sort??
	  sort(rm, rows, counters, rms);
	  //print_rationalMatrix(rms->rm_neg);// det är väldit skumt men tar man bort denna raden fungerar inte sort??

	 while(cols > 2){
	    int nzer = counters[0];
	    int nneg = counters[1];
	    int npos = counters[2];

	    /*Följer algoritmen från Matlab, slut på olikheter och onändligt antal lösningar */

	    if(npos*nneg + nzer <= 0)
	      return 1;

	    RationalMatrix* non_zer_rm = make_rationalMatrix(npos+nneg, cols);

			//Om npos och nneg är 0?
	    if(npos == 0)
	      non_zer_rm = rms->rm_neg;
	    else if(nneg == 0)
	      non_zer_rm = rms->rm_pos;
	    else
	      non_zer_rm = assemRationallMatrices(rms->rm_pos, rms->rm_neg);

      if(npos+nneg > 0){
        divFirstRow(non_zer_rm); //kommer det in nollor här?
  	    freeFirstCol(non_zer_rm);
      }
	    /*eftersom vi ska eliminera en kolonn kan vi ta cols = cols - 1; redan nu
	      så slipper vi kalla på cols -1 i funktionerna nedan */
	    cols = cols - 1;

	    RationalMatrix* new_rm = make_rationalMatrix(npos * nneg + nzer, cols);
	    new_rm = eliminateFirstCol(non_zer_rm, rms->rm_zer, npos, nneg, nzer);
	    free(non_zer_rm);
	    sort(new_rm, rows, counters, rms);
	    //Tror vi måste realloca storleken för rm om den ska användas igen.
	    free(rm);
	    //RationalMatrix* rm = make_rationalMatrix(npos * nneg + nzer, cols);
	    //RationalMatrix* rm = (RationalMatrix*)realloc(RationalMatrix* rm, rows * cols * sizeof(RationalNumber) + 2*sizeof(int)); //HÄÄR är det lurigt!
	    rm = new_rm;
	    //Vi använder oss bara av rms i algoritmen.
	    //free(new_rm);
    }
    //sort(rm, rows, counters, rms);//onödig rad?
    //Nu har vi bara två kolloner kvar så dags att bedömma dem

    int nzer = counters[0];
    int nneg = counters[1];
    int npos = counters[2];

    RationalMatrix* non_zer_rm = make_rationalMatrix(npos+nneg , cols);
    non_zer_rm = assemRationallMatrices(rms->rm_pos, rms->rm_neg);
    RationalMatrix* zer_rm = make_rationalMatrix(nzer, cols);
    zer_rm = rms->rm_zer;

		free(rm);
		free(rms);
		//free(new_rm);

    if(npos+nneg > 0)
      divFirstRow(non_zer_rm);
    //cols = cols - 1;
    //RationalMatrix* new_rm = make_rationalMatrix(npos * nneg + nzer, cols);
    //new_rm = eliminateFirstCol(non_zer_rm, rms->rm_zer, npos, nneg, nzer);
    //free(non_zer_rm);
    //divFirstRow(new_rm);
    //sort(new_rm, new_rm->rows, counters, rms);
    //nzer = counters[0];
    //nneg = counters[1];
    //npos = counters[2];
    RationalNumber B1;// = malloc(sizeof(int)*2);
    RationalNumber b1;// = malloc(sizeof(int)*2);
    RationalNumber q_min;// = malloc(sizeof(int)*2);

    B1.numerator = 2147483647; //INT_MAX
    B1.denumerator = 1;
    b1.numerator = -2147483647; //INT_MIN
    b1.denumerator = 1;
    q_min.numerator = 0; //INT_MIN
    q_min.denumerator = 1;

    if(npos > 0)
      B1 = findMin(non_zer_rm, B1, 0, npos);
    if(nneg > 0)
      b1 = findMax(non_zer_rm, b1, npos, npos + nneg);
    if (nzer > 0)
      q_min = findMin(zer_rm, q_min, 0, nzer);

    if (q_min.numerator <= 0)
      return 0;

    if(compareMin(b1,B1) == 1)
      return 1;
    else
      return 0;

      //free(new_rm);
      //free(B1);
      //free(b1);
      //free(q_min);

		//Om vi redan har returnat, går vi fortfarande hit och freear?

}
