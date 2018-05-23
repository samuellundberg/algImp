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
  long long mgn = b;
  if(rn->n < 0)
    mgn = - mgn;

  rn->t = rn->t/mgn;
	rn->n = rn->n/mgn;
}

void subq(RationalNumber *rm1, RationalNumber *rm2, RationalNumber *rm_res){
	rm_res->t = rm2->t*rm1->n-rm1->t*rm2->n;
	rm_res->n = rm1->n*rm2->n;
	reduce(rm_res);
}

int compare(RationalNumber *rn1, RationalNumber *rn2) {
	float a = ((float)rn1->t)/rn1->n;
	float b = ((float)rn2->t)/rn2->n;
  if(a>b)
    return 1;

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

void swap(RationalNumber **rm, int index_1, int index_2){
    RationalNumber* temp = rm[index_1];
    rm[index_1] = rm[index_2];
    rm[index_2] = temp;
}

void sort(RationalNumber **rm, int rows, int cols, int *npos, int *nnonzer){
	int count = 0;
	int i;
	for(i = 0; i < rows; i++){
    RationalNumber rn = rm[i][cols-1];
		 if((rn.t > 0 && rn.n > 0 ) || (rn.t < 0 && rn.n < 0)){
			if(count!=i)
				swap(rm, i, count);

      count += 1;
		 }
   }
	*npos = count;

	for(i = count; i < rows; i++){
		RationalNumber rn = rm[i][cols-1];
		if((rn.t < 0 && rn.n > 0 ) || (rn.t > 0 && rn.n < 0)){
			if(count!=i)
        swap(rm, count, i);

      count += 1;
		}
	}
	*nnonzer = count;
}

void divideRow(RationalNumber **rm, int cols, int *nnonzer){
  int i,j;
  for (i = 0; i < *nnonzer; i++) {
		RationalNumber *rn_div = malloc(sizeof(RationalNumber));
		*rn_div = rm[i][cols-1];
		for (j = 0; j < cols+1; j++)
			divq(&rm[i][j], rn_div);

		free(rn_div);
	}
}

void assamble_rm(RationalNumber **rm, RationalNumber **new_rm, int rows, int cols, int *npos, int *nnonzer){
	int count = 0;
	int i,j,k;
	for(i = 0; i < *npos; i++) {
		for(k = *npos; k < *nnonzer; k++) {
			for(j = 0; j < cols-1; j++)
				subq(&rm[i][j], &rm[k][j], &new_rm[count][j]);

			subq(&rm[k][cols], &rm[i][cols], &new_rm[count][cols-1]);
			count = count + 1;
		}
	}

	for(i = *nnonzer; i < rows; i++) {
		for(j = 0; j < cols-1; j++)
			new_rm[count][j] = rm[i][j];

		new_rm[count][cols-1] = rm[i][cols];
		count = count+1;
	}
}

void set_limits(RationalNumber *c, int *B_isset, int *b_isset, RationalNumber *Br, RationalNumber *br, int npos, int nnonzer){
	RationalNumber rn_min;
	rn_min.t = 2147483647;
	rn_min.n = 1;
	RationalNumber comp;
	*B_isset = 0;
	double min;
	double test;
	int i;
	for(i = 0; i < npos; i++) {
		subq(&c[i], &rn_min, &comp);
		min = (double)rn_min.t/(double)rn_min.n;
		test = (double)c[i].t/(double)c[i].n;
		if (min>test) {
			rn_min = c[i];
			*B_isset = 1;
		}
	}
	RationalNumber rn_max;
	rn_max.t = -2147483647;
	rn_max.n = 1;
	*b_isset = 0;
	for(i = npos; i < nnonzer; i++) {
		double max = (double)rn_max.t/(double)rn_max.n;
		double test2 = (double)c[i].t/(double)c[i].n;
		subq(&c[i], &rn_max, &comp);
		if(max<test2){
			rn_max = c[i];
			*b_isset = 1;
		}
	}
	*Br = rn_min;
	*br = rn_max;
}

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
  size_t i,j;
 	int *npos = malloc(sizeof(int));
 	int *nnonzer = malloc(sizeof(int));
 	RationalNumber **rm = malloc(rows*sizeof(RationalNumber *));
	for (i = 0; i < rows; i++) {
		rm[i] = malloc((cols+1)*sizeof(RationalNumber));
	}

 for(i = 0; i < rows; i++) {
	 for(j = 0; j < cols; j++) {
		 RationalNumber rnA;
		 rnA.t = a[i][j];
		 rnA.n = 1;
		 rm[i][j] = rnA;
	 }
	 RationalNumber rnC;
	 rnC.t = c[i];
	 rnC.n = 1;
	 rm[i][cols] = rnC;
 }
	//cols = cols + 1;

	while(cols > 1){

		sort(rm, rows, cols, npos, nnonzer);
		divideRow(rm, cols, nnonzer);

		size_t new_rows =(rows - (*nnonzer)) + (*npos)*((*nnonzer)-(*npos));

		if(new_rows==0){
			for(i = 0; i < rows; i++)
				free(rm[i]);

			free(rm);
			free(npos);
			free(nnonzer);
			return 1;
		}
		RationalNumber **new_rm = malloc(new_rows*sizeof(RationalNumber *));
		for (i = 0; i < new_rows; i++) {
			new_rm[i] = malloc((cols)*sizeof(RationalNumber));
		}
		assamble_rm(rm, new_rm, rows, cols, npos, nnonzer);

		for(i = 0; i < new_rows; i++) {
			for(j = 0; j < cols; j++) {
						}
		}

		for(i = 0; i < rows; i++) {
			free(rm[i]);
		}
		free(rm);

		rm = malloc(new_rows*sizeof(RationalNumber *));
		for (i = 0; i < new_rows; i++) {
			rm[i] =  malloc(cols*sizeof(RationalNumber));
		}
		for(i = 0; i < new_rows; i++) {
			for(j = 0; j < cols; j++) {
				rm[i][j] = new_rm[i][j];
			}
		}

		for(i = 0; i < new_rows; i++) {
			free(new_rm[i]);
		}
		free(new_rm);

		rows = new_rows;
		cols = cols-1;


	}
	sort(rm, rows, cols, npos, nnonzer);
	RationalNumber *br = malloc(sizeof(RationalNumber));
	RationalNumber *Br = malloc(sizeof(RationalNumber));
	int *B_isset = malloc(sizeof(int*));
	int *b_isset = malloc(sizeof(int*));
	int *solved = malloc(sizeof(int*));
	divideRow(rm, cols, nnonzer);

	RationalNumber *rmC = malloc(rows*sizeof(RationalNumber));
	for (i = 0; i < rows; i++)
		rmC[i] = rm[i][cols];

	set_limits(rmC, B_isset, b_isset, Br, br, *npos, *nnonzer);

	if(*B_isset == 0)
		*solved = 1;
	else if(*b_isset == 0)
		*solved = 1;


	if(*nnonzer-rows > 0) {
		for(i = *nnonzer; i < rows; i++){
			RationalNumber temp = rmC[i];
			if((temp.t < 0 && temp.n > 0) || (temp.t > 0 && temp.n < 0 ))
				*solved = 0;
		}
	}
	if(compare(Br, br) == 0 && *B_isset == 1 && *b_isset == 1)
		*solved = 0;
	else if(compare(Br, br) == 1 && *B_isset == 1 && *b_isset == 1)
		*solved = 1;

	for(i = 0; i < rows; i++)
		free(rm[i]);
	free(rm);

	free(rmC);
	free(br);
	free(Br);
	free(npos);
	free(nnonzer);
	free(B_isset);
	free(b_isset);
	int intsolved = *solved;
	free(solved);
	return intsolved;
}
