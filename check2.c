#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ARENA_SIZE      (10024 * 10024)

typedef struct list_t   list_t;
struct list_t {
        list_t*         succ;
        list_t*         pred;
        void*           data;
};
static list_t* xfree_list;

typedef struct arena_t  arena_t;

struct arena_t {
        char*   current;
        char    buffer[ARENA_SIZE];
};

arena_t* new_arena(void)
{
        arena_t*        arena;
        arena = calloc(1, sizeof(arena_t));
        arena->current = &arena->buffer[0];
        return arena;
}

static arena_t* arena;

void* alloc(arena_t* arena, size_t size)
{
        size_t  remain;         // how much memory is left.
        void*   data;           // return this to user.
        remain = ARENA_SIZE - (arena->current - &arena->buffer[0]);
        if (size > remain)
                return NULL;
                //printf("RAN OUT OF MEMORY\n" );
                //exit(0);
        data = arena->current;
        arena->current += size;
        return data;
}

void free_alloc(arena_t* arena){
        //check something?
        free(arena);
}

void free_list(list_t** head){
        list_t* h = *head;
        if(h==NULL)
                return;
        h->pred->succ = xfree_list;
        xfree_list = h;
        *head = NULL;
}

static double sec(void)
{
        struct timeval  tv;

        gettimeofday(&tv, NULL);

        return tv.tv_sec + 1e-6 * tv.tv_usec;
}

int empty(list_t* list)
{
        return list == list->succ;
}

list_t* new_list(void* data, arena_t* arena)
{
        list_t*         list;
        if(xfree_list != NULL){
                list = xfree_list;
                xfree_list = xfree_list->succ;
        }else{
                list = alloc(arena, sizeof(list_t));
        }

        assert(list != NULL);

        list->succ = list->pred = list;
        list->data = data;

        return list;
}

void add(list_t* list, void* data, arena_t* arena)
{
        list_t*         link;
        list_t*         temp;

        link            = new_list(data, arena);

        list->pred->succ= link;
        link->succ      = list;
        temp            = list->pred;
        list->pred      = link;
        link->pred      = temp;
}

void take_out(list_t* list)
{
        list->pred->succ = list->succ;
        list->succ->pred = list->pred;
        list->succ = list->pred = list;
}

void* take_out_first(list_t* list)
{
        list_t* succ;
        void*   data;

        if (list->succ->data == NULL)
                return NULL;

        data = list->succ->data;

        succ = list->succ;
        take_out(succ);
        free_list(&succ);

        return data;
}

static size_t nextsize()
{
#if 1
	return rand() % 4096;
#else
	size_t		size;
	static int	i;
	static size_t	v[] = { 24, 520, 32, 32, 72, 8000, 16, 24, 212 };

	size = v[i];

	i = (i + 1) % (sizeof v/ sizeof v[0]);

	return size;
#endif
}

static void fail(char* s)
{
        fprintf(stderr, "check: %s\n", s);
        abort();
}

int main(int ac, char** av)
{
        int             n = 500000;             /* mallocs in main. */
        int             n0;
        list_t*         head;
        double          begin;
        double          end;
        double          t = 2.5e-9;
        arena = new_arena();

        if (ac > 1)
                n = atoi(av[1]);

        n0 = n;

        head = new_list(NULL, arena);

        printf("check starts\n");

        begin = sec();


        list_t*         list;
        while (n > 0) {
                if(xfree_list != NULL){
                        list = xfree_list;
                        xfree_list = xfree_list->succ;
                }else{
                        list = alloc(arena, sizeof(list_t));
                }
                add(head, list, arena);
                n -= 1;

                if ((n & 1) && !empty(head))
                        free_list(take_out_first(head));
        }

        while (!empty(head))
                free_list(take_out_first(head));

        free_list(&head);
        free_alloc(arena);

        end = sec();

        printf("check is ready\n");
        printf("total = %1.3lf s\n", end-begin);
        printf("m+f   = %1.3g s\n", (end-begin)/(2*n0));
        printf("cy    = %1.3lf s\n", ((end-begin)/(2*n0))/t);

        return 0;
}
