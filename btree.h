#define M 2
#define MM 4
#define NIL (-1L)
#define KEY_LEN 60

typedef enum {
   INSERTNOTCOMPLETE, SUCCESS, DUPLICATEKEY,
   UNDERFLOW, NOTFOUND
} status;

typedef struct {
    char index[KEY_LEN];
    long value;
} btuple_t;

typedef struct{
  int cnt;
  btuple_t tuple[MM];
  long ptr[MM+1];
} node;

extern node rootnode;
extern long start[2];
extern long root;
extern long freelist;
extern FILE *fptree;

/************************FUNCTION PROTOTYPES****************************/
void error(char *str);
void readnode(long t, node *pnode);
void writenode(long t, node *pnode);
long getnode(void);
void found(long t,  int i);
void notfound(int x);
int binsearch(btuple_t x, btuple_t *a, int n);
long search(char *index);
status ins(btuple_t x, long t, btuple_t *y, long *u);
status insert(char *index, long value);
void freenode(long t);
void rdstart(void);
void wrstart(void);
status del(btuple_t x, long t);
status delnode(char *index);
void printtree(long t);
btuple_t gen_tuple(char *index, long value);
/**********************END FUNCTION PROTOTYPES***************************/
