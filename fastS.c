#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

typedef struct {
	long long t;
	long long n;
} ratnum_t;


int getLcd(long long a, long long b){
	long long tmp = 1;
	while (b!=0) {tmp = b; b = a % b; a = tmp;}
	return tmp;
}

int reduce(ratnum_t *r){
	long long t = r->t; //collect value from struct
	long long n = r->n;
	long long lcd = getLcd(t ,n);
	r->t = t/lcd; // change value in our struct
	r->n = n/lcd;
	return 0;
}

int subq(ratnum_t *r1, ratnum_t *r2, ratnum_t *result){
	result->t = -1* ((r1->t*r2->n)-(r2->t*r1->n));
	result->n = r1->n*r2->n;
	reduce(result);
	return 0;
}

int ratcomp(ratnum_t *r1, ratnum_t *r2) {
		double R1 = (double)r1->t/(double)r1->n;
	double R2 = (double)r2->t/(double)r2->n;

if (R1<R2) {

		return 0; //the subtraction became negative --> r1<r2
	}
	return 1;

}

int divq(ratnum_t *r1, ratnum_t *r2){

	r1->t = r1->t*r2->n;
	if(r2->t==0){
		r1->t = 0;
	}else{
		r1->n = r1->n*r2->t;
	}
	reduce(r1);

	return 0;
}

int swap_rows(size_t r, ratnum_t **ineq, int row1, int row2){
	for(size_t j = 0; j<r+1 ; j++){// swap all of the elements in the row
		ratnum_t tmp = ineq[row1][j]; // pekar på elementet jag vill lägga i tmp
  	ineq[row1][j] = ineq[row2][j];
		ineq[row2][j] = tmp;
		}
  return 0;
}

int sort_list( size_t r, size_t s,  ratnum_t **ineq, int *n1, int *n2){
	int count = 0;
	for(size_t i = 0; i<s; i++){
    ratnum_t rat = ineq[i][r-1];

                                 // get ratnum to compare w 0 do not want float
		 if((rat.t > 0 && rat.n > 0 ) || (rat.t < 0 && rat.n < 0)){ 		//check if positive
			if(count!=(int)i){
				swap_rows(r, ineq, i, count ); // rad i flyttas upp
		 	}
      count += 1;
		 }
   }
	*n1 = count; // change value of n1 - all positive values

	for(size_t i = count; i<s; i++){
		ratnum_t rat = ineq[i][r-1]; // get ratnum to compare w 0 do not want float
		if((rat.t < 0 && rat.n > 0 ) || (rat.t > 0 && rat.n < 0)){
			if(count!=(int)i){ //do not swap w myself
        swap_rows(r, ineq, count, i );
			}
      count += 1;
		}
	}

	*n2 = count; // change value of n2

  return 0;
}

int divide_matrix(size_t r, ratnum_t **ineq, int *n2){
	for (int i = 0; i < *n2; i++) {
		ratnum_t *divrat = malloc(sizeof(ratnum_t));
		*divrat = ineq[i][r-1];
		for (int j = 0; j < (int)r+1; j++) {
			divq(&ineq[i][j], divrat);
		}
		free(divrat);

	}

	return 0;
}



int combine_inequalities(size_t r, size_t s,  ratnum_t **ineq, ratnum_t **new_ineq, int *n1, int *n2){
	int count = 0;
	for (int i = 0; i < *n1; i++) {
		for (int k = *n1; k < *n2; k++) {
			for (int j = 0; j < (int)r-1; j++) {
				subq(&ineq[i][j], &ineq[k][j], &new_ineq[count][j]);
			}
			subq(&ineq[k][r], &ineq[i][r], &new_ineq[count][r-1]); // add c
			count = count + 1;

		}
	}

	for (size_t i = *n2; i < s; i++) { // add the rest (=0)
		for (size_t j = 0; j < r-1; j++) {
			new_ineq[count][j] = ineq[i][j];
		}
		new_ineq[count][r-1] = ineq[i][r];
		count = count+1; // ska vara lika med s_new
	}
	return 0;
}

int set_br_Br(ratnum_t *c, int n1, int n2, int *Br_isset, int *br_isset, ratnum_t *br, ratnum_t *Br){
	ratnum_t minrat;
	minrat.t = INT_MAX;
	minrat.n = 1;
	ratnum_t comp;					//Ratnum used to comapre the substraction
	*Br_isset = 0;
	double min;
	double test;
	for (int i = 0; i < n1; i++) {
		subq(&c[i], &minrat, &comp);       //Subtract the max value from the selected value and saves in the first
		min = (double)minrat.t/(double)minrat.n;
		test = (double)c[i].t/(double)c[i].n;

		if (min>test) {
			minrat = c[i];

			*Br_isset = 1;
		}
	}
	ratnum_t maxrat;
	maxrat.t = -INT_MAX;
	maxrat.n = 1;
	*br_isset = 0;
	for (int i = n1; i < n2; i++) {
		double max = (double)maxrat.t/(double)maxrat.n;
		double test2 = (double)c[i].t/(double)c[i].n;
		subq(&c[i], &maxrat, &comp);
	if(max<test2){
			maxrat = c[i];
			*br_isset = 1;
		}
	}
	*Br = minrat;
	*br = maxrat;
	return 0;
}

void break_function(){

}
size_t u;
bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
 	u = cols;
 	size_t s = rows;
 	size_t r = cols;
 	int *n1 = malloc(sizeof(int));
 	int *n2 = malloc(sizeof(int));
 	ratnum_t **ineq = malloc(s*sizeof(ratnum_t *)); //kanske alloca eller skriva eget  istället? Se xmallox i bok.
	for (size_t i = 0; i < s; i++) {
		ineq[i] = malloc((r+1)*sizeof(ratnum_t));
	}

 /* FIRST set matrix */
 for (size_t i = 0; i < s; i++) {
	 for (size_t j = 0; j < r; j++) {
		 ratnum_t rat; //borde gå att definiera bättre
		 rat.t = a[i][j];
		 rat.n = 1;
		 ineq[i][j] = rat;		//I j-loopen
	 }
	 ratnum_t ratc; //borde gå att definiera bättre
	 ratc.t = c[i];
	 ratc.n = 1;
	 ineq[i][r] = ratc;
 }


	while(1){

		if(r==1){
			break;
		}

		/*sort_list (step2)*/
		sort_list(r, s, ineq, n1, n2);
		for (size_t i = 0; i < s; i++) {
			for (size_t j = 0; j < r+1; j++) {
						}
		}

		divide_matrix(r, ineq, n2);


		for (size_t i = 0; i < s; i++) {
			for (size_t j = 0; j < r+1; j++) {
						}
		}

		size_t s_new =(s - (*n2)) + (*n1)*((*n2)-(*n1));

		if(s_new==0){
			for (size_t i = 0; i < s; i++) {
				free(ineq[i]);
			}
			free(ineq);
			free(n1);
			free(n2);
			return 1;	//HAS SOLUTION
		}
		ratnum_t **new_ineq = malloc(s_new*sizeof(ratnum_t *)); //kanske alloca eller skriva eget  istället? Se xmallox i bok.
		for (size_t i = 0; i < s_new; i++) {
			new_ineq[i] = malloc((r)*sizeof(ratnum_t));
		}
		combine_inequalities(r, s, ineq, new_ineq, n1, n2);	//creating a new matrix with the new inequalities

		for (size_t i = 0; i < s_new; i++) {
			for (size_t j = 0; j < r; j++) {
						}
		}

		for (size_t i = 0; i < s; i++) {
			free(ineq[i]);
		}
		free(ineq);

		ineq = malloc(s_new*sizeof(ratnum_t *));
		for (size_t i = 0; i < s_new; i++) {
			ineq[i] =  malloc((r)*sizeof(ratnum_t));
		}
		for (size_t i = 0; i < s_new; i++) {
			for (size_t j = 0; j < r; j++) {
				ineq[i][j] = new_ineq[i][j];
			}
		}

		for (size_t i = 0; i < s_new; i++) {
			free(new_ineq[i]);
		}
		free(new_ineq);

		s = s_new;
		r = r-1;


	}
	sort_list(r, s, ineq, n1, n2);
	ratnum_t *br = malloc(sizeof(ratnum_t));
	ratnum_t *Br = malloc(sizeof(ratnum_t));
	int *Br_isset = malloc(sizeof(int *));
	int *br_isset = malloc(sizeof(int *));
	int *solved = malloc(sizeof(int));
	divide_matrix(r, ineq, n2);

	ratnum_t (*ratc) = malloc(s*sizeof(ratnum_t));
	for (size_t i = 0; i < s; i++) {
		ratnum_t rat; //borde gå att definiera bättre
		rat = ineq[i][r];
		ratc[i] = rat;
	}
	set_br_Br(ratc, *n1, *n2, Br_isset, br_isset, br, Br);

	if(*Br_isset == 0) {			//no upper limit
		*solved = 1;

	}else if (*br_isset == 0) {				//no lower limit
		*solved = 1;

	}
	if (*n2-s > 0) {		//the coefficient of xr is zero
		for (size_t i = *n2; i < s; i++){
			ratnum_t temp = ratc[i];
			if ((temp.t < 0 && temp.n > 0) || (temp.t > 0 && temp.n < 0 )) {
				*solved = 0;

			}
		}
	}
	if(ratcomp(Br,br) == 0 && *Br_isset == 1 && *br_isset == 1) { //if both limits are set and the upper is lower then the lowest
		*solved = 0;

	}else if (ratcomp(Br,br) == 1 && *Br_isset == 1 && *br_isset == 1) {
		*solved = 1;
	}
	free(ratc);
	for (size_t i = 0; i < s; i++) {
		free(ineq[i]);
	}
	free(ineq);
	free(br);
	free(Br);
	free(n1);
	free(n2);
	free(Br_isset);
	free(br_isset);
	int intsolved = *solved;
	free(solved);
return intsolved;
}
