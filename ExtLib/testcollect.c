#include<stdio.h>
#include<stdlib.h>

typedef struct _B SL;
struct _B{
	struct _B* prior;
	struct _B* next;
	int * data;
};
typedef struct _A L;
struct _A{
	struct _A* next;
	int* p;
};

// storage the path for different length, use list 
static L* *zlist;
static int v=0;
static int zone = 5;
static int pnum=0;
static int *tmpath=NULL;

// quick double list --> point to zlist
static SL* *lst;
static SL* lsth = 0;
static SL* addh = 0;

static char addp_docs[] = 
	"AddPath([...]): add path and storge, remove similar path";
static char setv_docs[] = 
	"SetV(V): add path and storge, remove similar path; at best run once";

static void trans(char *ps, int* zn){
	int i = 0,item=0,pi=0;
	for (i = 0; ps[i]!=0; ++i) {
		if(ps[i]==','){
			ps[i] = 0;
			zn[item++] = atof(ps+pi);
			pi = i+1;
			if(item >= v-1)
				break;
		}
	}
	zn[item] = atof(ps+pi);

}
/**Return the New zone for paths;
 * */
int* addzn(int p){
	L* tp=zlist[p];
	if(tp == NULL){
		zlist[p] = (L*)malloc(sizeof(L));
		zlist[p]->p = (int *)malloc(sizeof(int)*p*zone);
		return zlist[p]->p;
	}
	while(tp->next!=NULL)tp = tp->next;
	tp->next = (L *)malloc(sizeof(L));
	tp->next->p = (int *)malloc(sizeof(int)*p*zone);
	return tp->next->p;
}

SL* addem(int p){
	SL *ret=0;
	int* tadd = 0;
	SL * tlst = 0;
	int i = 0;
	if( addh[p].next == addh[p].prior){
		/** malloc for more space
		 **/
		tadd = addzn(p);
		tlst = (SL*)malloc(sizeof(SL)*zone);
		for (i = 0; i < zone; ++i) {
			tlst[i].prior = tlst+i-1;
			tlst[i].next = tlst+i+1;
			tlst[i].data = tadd+i*p;	
		}
		tlst[0].prior = addh+p;
		tlst[i-1].next = addh+p;

		addh[p].prior = tlst+i-1;
		addh[p].next = tlst;

		printf("APPEND list END!\n");
		return addem(p);
	}else{  
		ret = addh[p].next;
		//append head
		addh[p].next = addh[p].next->next;
		//begin and end become circle
		addh[p].next->next->prior = addh+p;
		
		// lst head
		// append element at the end
		ret->prior = lsth[p].prior;
		ret->next = lsth+p;
		lsth[p].prior->next = ret;
		lsth[p].prior = ret;

		return ret;
	}	
	
}
void delem(int p, SL* dele){
	dele->next->prior=dele->prior;
	dele->prior->next=dele->next;

	addh[p].prior->next = dele;
	dele->prior = addh[p].prior;
	dele->next = addh+p;
	addh[p].prior = dele;
}

static int compare(int pl,int *tl,int ps,int *ts){
	return 0;
}

/**
 * This Func init vertex number and apply for the double list for all different length path;
 * */
static int* SetV(int *self, int* args){
	int i=6,j;
	int *pt=0;
	if(self!=NULL)
		return NULL;
	if(self == NULL){
		i = 6;
	}
	v = i;
	zlist=(L**)malloc(sizeof(L*)*(v+1));
	lst = (SL**)malloc(sizeof(SL*)*(v+1));
	lsth = (SL *)malloc(sizeof(SL)*(v+1));
	addh= (SL *)malloc(sizeof(SL)*(v+1));
	// for all length >= 2 (path)
	for(i=2; i<v+1;i++){
		zlist[i] =  NULL;
		lst[i] = (SL*)malloc(sizeof(SL)*zone);
		pt = addzn(i);
		for(j=0; j<zone; j++)
		{
			lst[i][j].prior = lst[i]+j-1;
			lst[i][j].next = lst[i]+j+1;
			lst[i][j].data = pt+j*i;
		}
		lst[i][0].prior = addh+i;
		lst[i][j-1].next = addh+i;

		addh[i].prior = lst[i]+j-1;
		addh[i].next = lst[i];

		lsth[i].prior = lsth+i;
		lsth[i].next = lsth+i;
	}
		
	return NULL;
}



void test_print(int p){
	int i = 0;
	SL * tp =lsth[p].next;
	printf("Test Zlist :\n");
	for (i = 0; i < v+1; ++i) {
		printf("%d,",(int)zlist[i]);
	}
	printf("\n");
	printf("Test lst:\n");
	i=0;
	while(tp!=lsth+p){
		i++;
		printf("(%d,%d,%d,%d),",(int)tp,(int)tp->next,(int)tp->prior,(int)tp->data);
		tp = tp->next;
		if(i == 10)break;
	}
	printf("\n");
	printf("Test addlist:\n");
	tp = addh[p].next;
	i=0;
	while(tp!=addh+p){
		i++;
		printf("(%d,%d,%d,%d),",(int)tp,(int)tp->next,(int)tp->prior,(int)tp->data);
		tp = tp->next;
		if(i == 10)break;
	}
	printf("\n");
}
void test_addem(){
	int i =0;
	SL* dt = NULL;
	SetV(NULL,NULL);
	test_print(6);
	for(i=0; i<zone+2; ++i){
		addem(6);
		test_print(6);
	}
	for(i=0; i<zone; ++i){
		dt = lsth[6].prior;
		delem(6,dt);
		printf("Delet test (%d)~~~\n",(int)dt);
		test_print(6);
	}	

}
int main(){
	test_addem();
	return 0;
}
