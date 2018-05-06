#include <stdio.h>  /* printf */
#include <stdlib.h> /* qsort */
#define ARRAYSIZE(A) (sizeof(A) / sizeof(A[0]))


void printMatrix (int matrix[][2], int rows, int cols){
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

/*Kollar om ett rationellt tal är negativt.
  Returnerar 0 om neg och 1 annars*/
int isRationalNumberNegative(RationalNumber rn){
  if(rn.numerator > 0 && rn.denumerator > 0){
    return 0;
  }
  if(rn.numerator < 0 && rn.denumerator < 0){
    return 0;
  }
  return 1;
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
}

/* Funktionen som konverterar matris av ints till rationella tal
   obs! Jag har inte lyckats lösa så att matrisen som man skickar
   kan vara av vilken storlek som helst utan det måste göras manuellt
   för tillfället, därav 2an i int matrix[][2] som inparameter*/
RationalMatrix* convertToRationlNumbers(int matrix[][2], int rows, int cols){
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

/* Steg 2 är att sortera första kolonnen i matrisen med rationella tal.
   Använder oss av mergesort för att sortera listan.
   Komplexitet: O(n log n)*/

/* Funktion för att skriva ut första kolonnen i matrisen
   med rationella tal */
void* printFirstCol(RationalMatrix* rm){
  for (int i = 0; i < rm->rows; i++){
      printf("%d / %d \n", rm->data[i][0].numerator, rm->data[i][0].denumerator);
  }
}
// void merge(RationalMatrix* rm, int l, int m, int r){
//   int i, j, k;
//   int n1 = m - l +1;
//   int n2 = r - m;
//   // printf("Vi kommer  in i merge %d\n", m);
//   //Skapar temporära matriser med raionella tal
//   RationalMatrix* L = make_rationalMatrix(n1, rm->cols);
//   RationalMatrix* R = make_rationalMatrix(n2, rm->cols);
//
//   //Lägger in datan till rat-matriserna L och R
//   for (i = 0; i < n1; i++)
//     L->data[i] = rm->data[l+i]; //Kanske inte fungerar att kalla till er rad på det sättet
//   for (j = 0; j < n2; j++)
//     R->data[j] = rm->data[m+1+j]; //Kanske inte fungerar att kalla till er rad på det sättet
//
//   //Lägger nu tillbaka de temporära rat-matriserna i orginal rat-matrisen
//   i = j = 0;
//   // k = l;
//   // printf("L:\n");
//   // printFirstCol(L);
//   // printf("R:\n");
//   // printFirstCol(R);
//   // kopierar negativa element från L
//   while(i < n1 && isRationalNumberNegative(L->data[i][0]))
//         rm->data[k++] = L->data[i++];
//   // kopierar negativa element från R
//   while(j < n2 && isRationalNumberNegative(R->data[j][0]))
//         rm->data[k++] = R->data[j++];
//   // kopierar positiva element från L
//   while(i < n1)
//         rm->data[k++] = L->data[i++];
//   // kopierar positiva element från R
//   while(j < n2)
//         rm->data[k++] = R->data[j++];
//
// }
//
// void rearrangeRationalMatrix(RationalMatrix* rm, int l, int r){
//       if (l < r){
//           int m = (l + r)/2; //Kan skrivas om som l + (r - l) / 2; stog något om att det annar kunde fucka för stora tal.
//           //printf("Vi kommer  in i rearr\n");
//           rearrangeRationalMatrix(rm, l, m);
//           rearrangeRationalMatrix(rm, m + 1, r);
//           printFirstCol(rm);
//           printf("l = %d, m = %d, r = %d\n", l, m, r);
//           merge(rm, l, m, r);
//       }
// }


/* Här är sortering funktionen, den lägger alla pos, neg, zer i
   var sin egen matris och sen lägger ihop den i orginalmatrisen.
   Tror den har tidskomplexitet O(n)
   */
}
RationalMatrix* sort(RationalMatrix* rm, int rows, int cols){
  int i;
  int zercount = 0;
  int negcount = 0;
  int poscount = 0;


  //Beräknar antalet pos, neg och zer element i första kolonnen för att kunna skapa matriserna
  for( i=0; i < rows; i++){
    if (rm->data[i][0].numerator == 0)
      zercount++;
    else if (isRationalNumberNegative(rm->data[i][0]))
      negcount++;
    else
      poscount++;
  }


  //Skapar/allocerar minna för tre stycken matriser att lägga pos, neg och zer elementen i
  int rm_cols = rm->cols;

  RationalMatrix* rm_zer = make_rationalMatrix(zercount, rm_cols);
  // if (negcount>0)
  RationalMatrix* rm_neg = make_rationalMatrix(negcount, rm_cols);
  // if (poscount>0)
  RationalMatrix* rm_pos = make_rationalMatrix(poscount, rm_cols);

  int j,k,l = 0;
  //Lägger in pos, neg och zer rader i sina respektive matriser
  for( i = 0; i < rows; i++){
    if (rm->data[i][0].numerator == 0){
      rm_zer->data[j] = rm->data[i];
      j++;
    }
    else if (isRationalNumberNegative(rm->data[i][0])){
      rm_neg->data[k] = rm->data[i];
      k++;
    }
    else {
      rm_pos->data[l] = rm->data[i];
      l++;
    }
  }

  //Lägger in elementen igen i matrisen rm, fast i rätt ordning
  for(i = 0; i<poscount; i++)
    rm->data[i] = rm_pos->data[i];
  for(i = 0; i<negcount; i++)
    rm->data[poscount + i] = rm_neg->data[i];
    //Kan vara så att vi kan skita i den här loopen kommer inte ihåg ifall vi ändå ska ta bort 0elementen dirrekt efter detta
  for(i = 0; i<zercount; i++)
    rm->data[poscount + negcount + i] = rm_neg->data[i];

  return rm;
}


 int main(int ac, char** av){
  int A[4][2] = {
    {2, -11} ,
    {-3, 2} ,
    {1, 3} ,
    {-2, 0}
  };

  int rows = ARRAYSIZE(A);
  int cols = ARRAYSIZE(A[0]);

  //printMatrix(A, rows, cols);
  RationalMatrix* rm = convertToRationlNumbers(A,rows,cols);
  //print_rationalMatrix(rm);
  printf("Första kolonnen innan sortering:\n");
  printFirstCol(rm);

  sort(rm, rows,cols);

  printf("Andra kolonnen efter sortering:\n");
  //rearrangeRationalMatrix(rm, 0, rows);
  printFirstCol(rm);

     return 0;
}
