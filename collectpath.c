#include<Python.h>

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
static int zone = 1000;
static int *tmpath=NULL;

// quick double list --> point to zlist
static SL* *lst;
static SL* lsth = 0;
static SL* addh = 0;

static char addp_docs[] = 
	"AddPath([...]): add path and storge, remove similar path";
static char setv_docs[] = 
	"SetV(V): add path and storge, remove similar path; at best run once";

int trans(char *ps, int* zn){
	int i = 0,item=0,pi=0;
	for (i = 0; ps[i]!=0; ++i) {
		if(ps[i]==','){
			if(item > v-1)
				goto transEnd;
			ps[i] = 0;
			zn[item++] = atoi(ps+pi);
			pi = i+1;
		}
	}
	if(pi <i && item < v){
		zn[item] = atof(ps+pi);
		item++;
	}
transEnd:
	return item;
	

}
/**Return the New zone for paths;
 * */
static int* addzn(int p){
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

static SL* addem(int p){
	SL *ret=0;
	int *tadd = 0 ;
	SL * tlst = 0 ;
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
static SL* delem(int p, SL* dele){
	SL* ret = dele->prior;

	dele->next->prior=dele->prior;
	dele->prior->next=dele->next;

	addh[p].prior->next = dele;
	dele->prior = addh[p].prior;
	dele->next = addh+p;
	addh[p].prior = dele;

	return ret;
}

static int compare(int pl,int *tl,int ps,int *ts){
	int tps=--ps;
	--pl;
	while(pl>=ps ){
		while( *(tl+pl)==*(ts+ps) && ps >= 0 && pl >= 0 ){
			--ps;
			--pl;
		}
		if(ps < 0 )
			return 1;
		else if(tps > ps)
			return 0;
		--pl;

	}
	return 0;
}

static PyObject* AddPath(PyObject *self, PyObject* args){
       char * gts = 0;
       int len  = 0, i=0;
       SL * tp;
	if(!PyArg_ParseTuple(args,"s",&gts) || v == 0)
		return NULL;
	len = trans(gts,tmpath);
	for(i = len+1; i < v+1; i++){
		tp = lsth[i].next;
		if(tp!=NULL)
		for (tp = lsth[i].next; tp != lsth+i ; tp=tp->next) {
			if(compare(i,tp->data,len,tmpath)){
				return Py_BuildValue("s","FAIL");
			}
		}
	}
	tp = addem(len);
	for(i = 0; i < len; i++)
		tp->data[i] = tmpath[i];

	for( i = len - 1; i>1; --i ){
		for (tp = lsth[i].next; tp != lsth+i ; tp=tp->next) {
			if(compare(len,tmpath,i,tp->data)){	
				tp=delem(i,tp);
			}
		}
	}
	return Py_BuildValue("s","OK");
	
}
/**
 * This Func init vertex number and apply for the double list for all different length path;
 * */
static PyObject* SetV(PyObject *self, PyObject* args){
	int i,j;
	int *pt=0;
	if( !PyArg_ParseTuple(args,"i",&i))
		return NULL;
	v = i;
	zlist=(L**)malloc(sizeof(L*)*(v+1));
	lst = (SL**)malloc(sizeof(SL*)*(v+1));
	lsth = (SL *)malloc(sizeof(SL)*(v+1));
	addh= (SL *)malloc(sizeof(SL)*(v+1));

	tmpath = (int *)malloc(sizeof(int)*(v+1));
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
		
	return Py_BuildValue("s","OK");
}


static PyMethodDef funcs[]={
	{"AddPath",(PyCFunction)AddPath, METH_NOARGS,addp_docs},
	{"AddPath",(PyCFunction)AddPath, METH_VARARGS,addp_docs},
	{"SetV",(PyCFunction)SetV, METH_NOARGS,setv_docs},
	{"SetV",(PyCFunction)SetV, METH_VARARGS,setv_docs},
         {NULL,NULL,0,NULL}
};
void initcollectpath(void){

	Py_InitModule3("collectpath",funcs,"collect path and merge them!");
}

void test_print(int p){
	int i = 0;
	SL * tp =lsth[p].next;
	for (i = 0; i < v; ++i) {
	}
	while(tp!=lsth+p){
		tp = tp->next;
	}
}
void test_addem(){
	int i =0;
	SetV(NULL,NULL);
	zone = 5;
	for(i=0; i<zone; ++i){
		addem(2);
		test_print(2);
	}
}
