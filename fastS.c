#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

typedef struct RationalNumber{
	long long t;
	long long n;
} RationalNumber;



void reduce(RationalNumber *rn){
	long long a = rn->t;
	long long b = rn->n;
  long long c = a % b;
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
  long long mgn = b;    //kanske onödig
  if(rn->n < 0)
    mgn = - mgn;

  rn->t = rn->t/mgn;
	rn->n = rn->n/mgn;
}

void subq(RationalNumber *rm1, RationalNumber *rm2, RationalNumber *rm_res){
	rm_res->t = (rm2->t*rm1->n)-(rm1->t*rm2->n);
	rm_res->n = rm1->n*rm2->n;
	reduce(rm_res);
}

int compare(RationalNumber *rn1, RationalNumber *rn2) {
	float a = ((float)rn1->t)/rn1->n;
	float b = ((float)rn2->t)/rn2->n;
  if(a>b)
    return 1;
  else
    return 0;

}

void divq(RationalNumber *rn1, RationalNumber *rn2){
 long long a;
 long long b;
	a = rn1->t*rn2->n;
	b = rn1->n*rn2->t;
  rn1->t = a;
  rn1->n = b;
	reduce(rn1);
}

void swapRows(RationalNumber **rm, int row1, int row2){
    RationalNumber* temp = rm[row1];
    rm[row1] = rm[row2];
    rm[row2] = temp;
}

void sort(size_t cols, size_t rows,  RationalNumber **rm, int *npos, int *nnonzer){
	int count = 0;
	for(int i = 0; i < (int)rows; i++){
    RationalNumber rn = rm[i][cols-1];
		 if((rn.t > 0 && rn.n > 0 ) || (rn.t < 0 && rn.n < 0)){
			if(count!=i){
				swapRows(rm, i, count);
		 	}
      count += 1;
		 }
   }
	*npos = count;

	for(int i = count; i < (int)rows; i++){
		RationalNumber rn = rm[i][cols-1];
		if((rn.t < 0 && rn.n > 0 ) || (rn.t > 0 && rn.n < 0)){
			if(count!=i){
        swapRows(rm, count, i );
			}
      count += 1;
		}
	}
	*nnonzer = count;
}

void divideRow(size_t cols, RationalNumber **rm, int *nnonzer){
  int i,j;
  for (i = 0; i < *nnonzer; i++) {
		RationalNumber *divrn = malloc(sizeof(RationalNumber));
		*divrn = rm[i][cols-1];
		for (j = 0; j < (int)cols+1; j++) {
			divq(&rm[i][j], divrn);
		}
		free(divrn);
	}
}



int combine_rmualities(size_t cols, size_t rows,  RationalNumber **rm, RationalNumber **new_rm, int *npos, int *nnonzer){
	int count = 0;
	for (int i = 0; i < *npos; i++) {
		for (int k = *npos; k < *nnonzer; k++) {
			for (int j = 0; j < (int)cols-1; j++) {
				subq(&rm[i][j], &rm[k][j], &new_rm[count][j]);
			}
			subq(&rm[k][cols], &rm[i][cols], &new_rm[count][cols-1]); // add c
			count = count + 1;

		}
	}

	for (size_t i = *nnonzer; i < rows; i++) { // add the rest (=0)
		for (size_t j = 0; j < cols-1; j++) {
			new_rm[count][j] = rm[i][j];
		}
		new_rm[count][cols-1] = rm[i][cols];
		count = count+1; // ska vara lika med s_new
	}
	return 0;
}

int set_br_Br(RationalNumber *c, int npos, int nnonzer, int *Br_isset, int *br_isset, RationalNumber *br, RationalNumber *Br){
	RationalNumber rn_min;
	rn_min.t = 2147483647;
	rn_min.n = 1;
	RationalNumber comp;					//Ratnum used to comapre the substraction
	*Br_isset = 0;
	double min;
	double test;
	for (int i = 0; i < npos; i++) {
		subq(&c[i], &rn_min, &comp);       //Subtract the max value from the selected value and saves in the first
		min = (double)rn_min.t/(double)rn_min.n;
		test = (double)c[i].t/(double)c[i].n;

		if (min>test) {
			rn_min = c[i];

			*Br_isset = 1;
		}
	}
	RationalNumber rn_max;
	rn_max.t = -2147483647;
	rn_max.n = 1;
	*br_isset = 0;
	for (int i = npos; i < nnonzer; i++) {
		double max = (double)rn_max.t/(double)rn_max.n;
		double test2 = (double)c[i].t/(double)c[i].n;
		subq(&c[i], &rn_max, &comp);
	if(max<test2){
			rn_max = c[i];
			*br_isset = 1;
		}
	}
	*Br = rn_min;
	*br = rn_max;
	return 0;
}

void bp(){

}
// size_t u;
bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
  int i,j;
 	int *npos = malloc(sizeof(int));
 	int *nnonzer = malloc(sizeof(int));
 	RationalNumber **rm = malloc(rows*sizeof(RationalNumber *)); //kanske alloca eller skriva eget  istället? Se xmallox i bok.
	for (i = 0; i < (int)rows; i++) {
		rm[i] = malloc((cols+1)*sizeof(RationalNumber));
	}

 /* FIRST set matrix */
 for (i = 0; i < (int)rows; i++) {
	 for (j = 0; j < (int)cols; j++) {
		 RationalNumber rat; //borde gå att definiera bättre
		 rat.t = a[i][j];
		 rat.n = 1;
		 rm[i][j] = rat;		//I j-loopen
	 }
	 RationalNumber ratc; //borde gå att definiera bättre
	 ratc.t = c[i];
	 ratc.n = 1;
	 rm[i][cols] = ratc;
 }


	while(1){

		if(cols==1){
			break;
		}

		/*sort (step2)*/
		sort(cols, rows, rm, npos, nnonzer);
		divideRow(cols, rm, nnonzer);

		int s_new =(rows - (*nnonzer)) + (*npos)*((*nnonzer)-(*npos));

		if(s_new==0){
			for (i = 0; i < (int)rows; i++) {
				free(rm[i]);
			}
			free(rm);
			free(npos);
			free(nnonzer);
			return 1;	//HAS SOLUTION
		}
		RationalNumber **new_rm = malloc(s_new*sizeof(RationalNumber *)); //kanske alloca eller skriva eget  istället? Se xmallox i bok.
		for (i = 0; i < s_new; i++) {
			new_rm[i] = malloc((cols)*sizeof(RationalNumber));
		}
		combine_rmualities(cols, rows, rm, new_rm, npos, nnonzer);	//creating a new matrix with the new rmualities

		for (i = 0; i < (int)s_new; i++) {
			for (j = 0; j < (int)cols; j++) {
						}
		}

		for (i = 0; i < (int)rows; i++) {
			free(rm[i]);
		}
		free(rm);

		rm = malloc(s_new*sizeof(RationalNumber *));
		for (i = 0; i < (int)s_new; i++) {
			rm[i] =  malloc((cols)*sizeof(RationalNumber));
		}
		for (i = 0; i < (int)s_new; i++) {
			for (j = 0; j < (int)cols; j++) {
				rm[i][j] = new_rm[i][j];
			}
		}

		for (i = 0; i < (int)s_new; i++) {
			free(new_rm[i]);
		}
		free(new_rm);

		rows = s_new;
		cols = cols-1;


	}
	sort(cols, rows, rm, npos, nnonzer);
	RationalNumber *br = malloc(sizeof(RationalNumber));
	RationalNumber *Br = malloc(sizeof(RationalNumber));
	int *Br_isset = malloc(sizeof(int *));
	int *br_isset = malloc(sizeof(int *));
	int *solved = malloc(sizeof(int));
	divideRow(cols, rm, nnonzer);

	RationalNumber (*ratc) = malloc(rows*sizeof(RationalNumber));
	for (i = 0; i < (int)rows; i++) {
		RationalNumber rat; //borde gå att definiera bättre
		rat = rm[i][cols];
		ratc[i] = rat;
	}
	set_br_Br(ratc, *npos, *nnonzer, Br_isset, br_isset, br, Br);

	if(*Br_isset == 0) {			//no upper limit
		*solved = 1;

	}else if (*br_isset == 0) {				//no lower limit
		*solved = 1;

	}
	if (*nnonzer-rows > 0) {		//the coefficient of xr is zero
		for (i = *nnonzer; i < (int)rows; i++){
			RationalNumber temp = ratc[i];
			if ((temp.t < 0 && temp.n > 0) || (temp.t > 0 && temp.n < 0 )) {
				*solved = 0;

			}
		}
	}
	if(compare(Br,br) == 0 && *Br_isset == 1 && *br_isset == 1) { //if both limits are set and the upper is lower then the lowest
		*solved = 0;

	}else if (compare(Br,br) == 1 && *Br_isset == 1 && *br_isset == 1) {
		*solved = 1;
	}
	free(ratc);
	for (i = 0; i < (int)rows; i++) {
		free(rm[i]);
	}
	free(rm);
	free(br);
	free(Br);
	free(npos);
	free(nnonzer);
	free(Br_isset);
	free(br_isset);
	int intsolved = *solved;
	free(solved);
return intsolved;
}
