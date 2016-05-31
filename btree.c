#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "btree.h"

node rootnode;
long start[2], root=NIL, freelist=NIL;
FILE *fptree;

btuple_t gen_tuple(char *index, long value) {
    btuple_t ret;
    strcpy(ret.index,index);
    ret.value = value;
    
    return ret;
}

/*
int main()
{
btuple_t x; int code=0;
char ch, treefilnam[51], inpfilnam[51]; char buffer[100];
char *token;
char *stringp;
FILE *fpinp;


    strcpy(treefilnam,"tchururu.bin");
	 fptree = fopen(treefilnam, "r+b");
	 if (fptree == NULL) {
		fptree = fopen(treefilnam, "w+b");
		wrstart();
	 }else{
		  rdstart();
		  printtree(root);
          return 0;
	}
    strcpy(inpfilnam,"input.bt");
    if ((fpinp = fopen(inpfilnam, "r")) == NULL)
      error("file not available");
    while (fgets(buffer,99, fpinp) != NULL) {
        buffer[strlen(buffer)-1] = 0;
        stringp = buffer;
        printf("READ: [%s]\n",buffer);
        token = strsep(&stringp, " "); 
        strcpy(x.index,token);
        printf("READ: [%s]\n",x.index);
        token = strsep(&stringp, " "); 
        x.value = atol(token);
        printf("READ: [%li]\n",x.value);
        
        insert(x);
    }
    fclose(fpinp);
    printtree(root);
	
	
  wrstart();
  fclose(fptree);
  return(0);
}
*/

void error(char *str)
{
	printf("\nError: %s\n", str);
	exit(1);
}



void readnode(long t, node *pnode)
{
  if (t == root){
	 *pnode = rootnode;
	  return;
  }
  if (fseek(fptree, t, SEEK_SET))
	 error("fseek in readnode");
  if (fread(pnode, sizeof(node), 1, fptree) == 0)
	 error("fread in readnode");
}


void writenode(long t, node *pnode)
{
	if (t == root)
		rootnode = *pnode;
	if (fseek(fptree, t, SEEK_SET))
	  error("fseek in writenode");
	if (fwrite(pnode, sizeof(node), 1, fptree) == 0)
	  error("fwrite in writenode");
}

long getnode(void)
{
long t;
node nod;

  if (freelist == NIL) {
	 if (fseek(fptree, 0L, SEEK_END))
		 error("fseek in getnode");
  t = ftell(fptree);
  writenode(t, &nod);  }         /*  Allocate space on disk  */

  else{
	  t = freelist;
		readnode(t, &nod);             /*  To update freelist      */
		freelist = nod.ptr[0];
 }
 return(t);
}



void found(long t,  int i)
{
node nod;

	printf("Found in position %i of node with contents:  ", i);
	readnode(t, &nod);
	for (i=0; i < nod.cnt; i++)
		printf("  %s", nod.tuple[i].index);
	puts("");

}




void notfound(int x)
{
	printf("Item %i not found\n", x);
}




int binsearch(btuple_t x, btuple_t *a, int n)
{
    int i, left, right;

	if (strcmp(x.index, a[0].index) <= 0)
		return 0;
	if (strcmp(x.index, a[n-1].index) > 0)
		 return n;
	left = 0;
	right = n-1;
	while (right -  left > 1){
		  i = (right + left)/2;
		  if (strcmp(x.index, a[i].index) <= 0)
			  right = i;
		  else
			 left = i;
	}
	return(right);
}

long search(char *index)
{
int i, j, n;
btuple_t *k;

btuple_t x;
strcpy(x.index,index);

node nod;
long t = root;

	while (t != NIL){
		  readnode(t, &nod);
		  k = nod.tuple;
		  n = nod.cnt;
		  i = binsearch(x, k, n);
		  if (i < n && strcmp(x.index,k[i].index) == 0){
			  return k[i].value;
		  }
		  t = nod.ptr[i];
	}
	return -1;
}


/*
	Insert x in B-tree with root t.  If not completely successful, the
	 integer *y and the pointer *u remain to be inserted.
*/
status ins(btuple_t x, long t, btuple_t *y, long *u)
{
 long tnew, p_final, *p;
 int i, j, *n;
 btuple_t *k;
 btuple_t xnew, k_final;
 status code;
 node nod, newnod;

	/*  Examine whether t is a pointer member in a leaf  */
	if (t == NIL){
		*u = NIL;
		 *y = x;
		 return(INSERTNOTCOMPLETE);
	}
	readnode(t, &nod);
	n = & nod.cnt;
	k = nod.tuple;
	p = nod.ptr;
	/*  Select pointer p[i] and try to insert x in  the subtree of whichp[i]
	  is  the root:  */
	i = binsearch(x, k, *n);
	if (i < *n && strcmp(x.index,k[i].index) == 0)
	  return(DUPLICATEKEY);
	code = ins(x, p[i], &xnew, &tnew);
	if (code != INSERTNOTCOMPLETE)
	  return code;
	/* Insertion in subtree did not completely succeed; try to insert xnew and
	tnew in the current node:  */
	if (*n < MM){
		i = binsearch(xnew, k, *n);
		for (j = *n; j > i; j--){
			k[j] = k[j-1];
			p[j+1] = p[j];
		}
	  k[i] = xnew;
	  p[i+1] = tnew;
	  ++*n;
	  writenode(t, &nod);
	  return(SUCCESS);
	}
	/*  The current node was already full, so split it.  Pass item k[M] in the
	 middle of the augmented sequence back through parameter y, so that it
	 can move upward in the tree.  Also, pass a pointer to the newly created
	 node back through u.  Return INSERTNOTCOMPLETE, to report that insertion
	 was not completed:    */
	if (i == MM){
	  k_final = xnew;
	  p_final = tnew;
	 }else{
		  k_final = k[MM-1];
		  p_final = p[MM];
		  for (j=MM-1; j>i; j--){
			  k[j] = k[j-1];
			  p[j+1] = p[j];
		  }
			k[i] = xnew;
			p[i+1] = tnew;
	}
	*y = k[M];
	*n = M;
	*u = getnode(); newnod.cnt = M;
	for (j=0; j< M-1; j++){
		newnod.tuple[j] = k[j+M+1];
		newnod.ptr[j] = p[j+M+1];
	}
	newnod.ptr[M-1] = p[MM];
	newnod.tuple[M-1] = k_final;
	newnod.ptr[M] = p_final;
	writenode(t, &nod);
	writenode(*u, &newnod);
	return(INSERTNOTCOMPLETE);
}


/*  Driver function for node insertion, called only in the main function.

	 Most of the work is delegated to 'ins'.
*/
status insert(char *index, long value)
{
long tnew, u;
btuple_t x;
strcpy(x.index,index);
x.value = value;
btuple_t xnew;
status code = ins(x, root, &xnew, &tnew);

  if (code == DUPLICATEKEY)
	 printf("Duplicate uid %s ignored \n", x.index);
  else
	 if (code == INSERTNOTCOMPLETE){
		u = getnode();
		rootnode.cnt = 1;
		rootnode.tuple[0] = xnew;
		rootnode.ptr[0] = root;
		rootnode.ptr[1] = tnew;
		root = u;
		writenode(u, &rootnode);
		code = SUCCESS;
	}
	return(code);     /*  return index: SUCCESS  of DUPLICATEKEY  */
}



void freenode(long t)
{
node nod;

  readnode(t, &nod);
  nod.ptr[0] = freelist;
  freelist = t;
  writenode(t, &nod);
}



void rdstart(void)
{
  if (fseek(fptree, 0L, SEEK_SET))
	 error("fseek in rdstart");
  if (fread(start, sizeof(long), 2, fptree) == 0)
	 error("fread in rdstart");
  readnode(start[0], &rootnode);
  root = start[0];
  freelist = start[1];
}



void wrstart(void)
{

  start[0] = root;
  start[1] = freelist;
  if (fseek(fptree, 0L, SEEK_SET))
	  error("fseek in wrstart");
  if (fwrite(start, sizeof(long), 2, fptree) == 0)
	  error("fwrite in wrstart");
  if (root != NIL)
	  writenode(root, &rootnode);
}





/* Delete item x in B-tree with root t.

	Return index:

	  SUCCESS, NOTFOUND, OR UNDERFLOW
*/
status del(btuple_t x, long t)
{
int i, j, *n, *nleft, *nright, borrowleft=0, nq;
 btuple_t *k, *ltuple, *rtuple, *item, *addr;
status code;
long *p, left, right, *lptr, *rptr, q, q1;
node nod, nod1, nod2, nodL, nodR;

 if (t == NIL)
	return(NOTFOUND);
 readnode(t, &nod);
 n = & nod.cnt;
 k = nod.tuple;
 p=nod.ptr;
 i=binsearch(x, k, *n);
 /* *t is a leaf */
 if (p[0] == NIL){
	if (i == *n || strcmp(x.index,k[i].index) < 0)
		 return NOTFOUND;
	 /* x is now equal to k[i], located in a leaf:  */
	 for (j=i+1; j < *n; j++){
		 k[j-1] = k[j];
		 p[j] = p[j+1];
	 }
	 --*n;
	writenode(t, &nod);
	 return(*n >= (t==root ? 1 : M) ? SUCCESS : UNDERFLOW);
  }
  /*  t is an interior node (not a leaf): */
  item = k+i;
  left = p[i];
  readnode(left, &nod1);
  nleft = & nod1.cnt;
    /* x found in interior node.  Go to left child *p[i] and then follow a

	  path all the way to a leaf, using rightmost branches:  */
  if (i < *n && strcmp(x.index,item->index) == 0){
	  q = p[i];
	  readnode(q, &nod1);
	  nq = nod1.cnt;
	  while (q1 = nod1.ptr[nq], q1!= NIL){
			 q = q1;
			 readnode(q, &nod1);
			 nq = nod1.cnt;
	  }
	  /*  Exchange k[i] with the rightmost item in that leaf:   */
	  addr = nod1.tuple + nq -1;
	  *item = *addr;
	  *addr = x;
	  writenode(t, &nod);
	  writenode(q, &nod1);
  }

  /*  Delete x in subtree with root p[i]:  */
	code = del(x, left);
	if (code != UNDERFLOW)
		return code;
	/*  Underflow, borrow, and , if necessary, merge:  */
	if (i < *n)
		readnode(p[i+1], &nodR);
	if (i == *n || nodR.cnt == M){
	  if (i > 0){
		 readnode(p[i-1], &nodL);
		 if (i == *n || nodL.cnt > M)
			borrowleft = 1;
	  }
	}
	/* borrow from left sibling */
	if (borrowleft){
	  item = k+i-1;
		left = p[i-1];
		right = p[i];
		nod1 = nodL;
		readnode(right, &nod2);
		nleft = & nod1.cnt;
	}else{
		right = p[i+1];        /*  borrow from right sibling   */
		readnode(left, &nod1);
		nod2 = nodR;
	}
	nright = & nod2.cnt;
	ltuple = nod1.tuple;
	rtuple = nod2.tuple;
	lptr = nod1.ptr;
	rptr = nod2.ptr;
	if (borrowleft){
		rptr[*nright + 1] = rptr[*nright];
		for (j=*nright; j>0; j--){
			rtuple[j] = rtuple[j-1];
			rptr[j] = rptr[j-1];
		}
		++*nright;
		rtuple[0] = *item;
		rptr[0] = lptr[*nleft];
		*item = ltuple[*nleft - 1];
		if (--*nleft >= M){
		  writenode(t, &nod);
		  writenode(left, &nod1);
		  writenode(right, &nod2);
		  return SUCCESS;
		}
	}else
	/* borrow from right sibling */
	 if (*nright > M){
		 ltuple[M-1] = *item;
		 lptr[M] = rptr[0];
		 *item = rtuple[0];
		 ++*nleft;
		 --*nright;
		 for (j=0; j < *nright; j++){
			 rptr[j] = rptr[j+1];
			 rtuple[j] = rtuple[j+1];
		 }
		 rptr[*nright] = rptr[*nright + 1];
		 writenode(t, &nod);
		 writenode(left, &nod1);
		 writenode(right, &nod2);
		 return(SUCCESS);
	 }
	 /*  Merge   */
	 ltuple[M-1] = *item;
	 lptr[M] = rptr[0];
	 for (j=0; j<M; j++){
		ltuple[M+j] = rtuple[j];
		lptr[M+j+1] = rptr[j+1];
	 }
	 *nleft = MM;
	 freenode(right);
	 for (j=i+1; j < *n; j++){
		 k[j-1] = k[j];
		 p[j] = p[j+1];
	 }
	 --*n;
	 writenode(t, &nod);
	 writenode(left, &nod1);
	 return( *n >= (t==root ? 1 : M) ? SUCCESS : UNDERFLOW);
}




/*  Driver function for node deletion, called only in the main function.

	 Most of the work is delegated to 'del'.
*/
status delnode(char *index)
{
    long newroot;
    btuple_t x;
    strcpy(x.index,index);

	status code = del(x, root);
	if (code == UNDERFLOW){
	  newroot = rootnode.ptr[0];
	  freenode(root);
	  if (newroot != NIL)
		 readnode(newroot, &rootnode);
	  root = newroot;
	  code = SUCCESS;
	}
	return(code);  /* Return index:  SUCCESS  or NOTFOUND   */
}



void printtree(long t)
{
static int position=0;
int i, n;
btuple_t *k;
node nod;

  if (t != NIL){
	 //position += 0;
	 readnode(t, &nod);
	 k = nod.tuple;
	 n = nod.cnt;
	 //printf("%*s", position, "");
/*	 for (i=0; i<n; i++)
		printf(" [%9s] --> %li", k[i].index, k[i].value);
	 puts(""); */
	 for (i=0; i<=n; i++)
		printtree(nod.ptr[i]);
	 //position -= 6;
  }
}






























