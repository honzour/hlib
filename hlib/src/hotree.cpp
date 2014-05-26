#include <stdlib.h>
#include "hoset.h"
#include "hotree.h"

HoTreeNode::HoTreeNode(void *key, void *data, HoTreeNode *l, HoTreeNode *r,
  HoTreeNode *p, int bal) {
  this->key = key;
  this->data = data;
  this->l = l;
  this->r = r;
  this->p = p;
  this->bal = bal;
}

void HoTreeNode::freeData() {
  if (data) free(data);
}

void HoTreeNode::freeKey() {
  if (key) free(key);
}

HoTree::HoTree(int (*cmp)(void *l, void *r), int freeData, int freeKey) {
  this->cmp = cmp;
  this->freeData = freeData;
  this->freeKey = freeKey;
  root = NULL;
}

HoTree::~HoTree() {
  delAll();
}

void HoTree::delNode(HoTreeNode *n) {
  if (freeData) n->freeData();
  if (freeKey) n->freeKey();
  delete n;
}

void HoTree::delBranch(HoTreeNode *ro) {
  if (!ro) return;
  delBranch(ro->l);
  delBranch(ro->r);
  delNode(ro);
}

void HoTree::delAll(){
  delBranch(root);
  root = NULL;
}

void *HoTree::get(void *key) {
  int i;
  
  HoTreeNode *r = root;
  while (1) {
    if (!r) return NULL;
    if (!(i = cmp(r->key, key))) return r->data;
    if (i > 0) r = r->l; else r = r->r;
  }
}

int HoTree::is(void *key) {
  int i;

  HoTreeNode *r=root;
  while(1){
    if (!r) return 0;
    if (!(i = cmp(r->key, key))) return 1;
    if (i > 0) r = r->l; else r = r->r;
  }
}


void *HoTree::ins(void *key, void *data){
  HoTreeNode *x, *r, *o, **p, **px;
  int i;

  o = NULL;
  p = px = &root;
  r = x = root;
  while (r) {
    i = cmp(key, r->key);
    if (!i) return NULL;
    if (i < 0) p = &(r->l); else p=&(r->r);
    o = r; r = *p;
    if (r && r->bal) px = p;
  }
  x = *px;
  *p = new HoTreeNode(key, data, NULL, NULL, o, 0);
  if (!x) goto konecins;
  r = x;
  while (r) {
    i = cmp(key, r->key);
    if (i < 0) {r->bal++; r=r->l;}
    else if (i > 0) {r->bal--; r=r->r;}
    else r = NULL;
  }
  if (x->bal > 1) {
    if (cmp(key, x->l->key) < 0)
      LL(px);
    else LR(px);
  } else
  if (x->bal < -1) {
  if (cmp(key, x->r->key) > 0)
    RR(px);
    else RL(px);
  }
  konecins:;
  return data;
}

HoTreeNode **HoTree::getP(HoTreeNode  *n){
 if(!n->p)return &root;
 if(n->p->l==n) return &(n->p->l);
 if(n->p->r==n) return &(n->p->r);
 return NULL;
}

HoTreeNode *HoTree::right(HoTreeNode *ro){
// Vraci nejpravejsiho potomka ro
// predpoklada, ze ro neni NULL
 HoTreeNode *s;

 while(ro){s=ro; ro=ro->r;}
 return s;
}


void *HoTree::pomAVLDel(HoTreeNode **n)
 {
  HoTreeNode *t, *p, *o, **pp;
  void *d;
  int left, konec;

  p=*n;
// Nahrazuji nejpravejsim vlevo
  if(!(p->l)){t=p->r;
   if(t){
    o=t;left=0;
   }
   else
   {
    o=p->p;
    if(o && o->l==p)left=1;else left=0;
   }
  } /* l a r jsou a zustanou NULL*/
  else {
   if(p->l->r){
      t=right(p->l);
      o=t->p; left=0;

      if(t->l)t->l->p=t->p;

      t->p->r=t->l;
      t->l=p->l;
      t->r=p->r;
      p->l->p=t;
      p->r->p=t;
     }
   else {
     t=p->l;  o=t; left=1;
     t->r=p->r;
     if(p->r)p->r->p=t;
   }
  }
 if(t) t->bal=p->bal;
 if(t) t->p=p->p; // Pro vsechny pripady stejne
 d=p->data;
 delNode(p);
 *n=t;
/* A ted rotace*/
 konec=0;
 while(o && !konec){
  if(left){
   if(o->bal>-1){if(!o->bal)konec=1;o->bal--;}
   else {
    if(o->r->bal<1) {if(!o->r->bal)konec=1; RR(pp=getP(o),1);} else  RL(pp=getP(o),1);
    o=*pp;
   }
  }
  else
  {
   if(o->bal<1){if(!o->bal)konec=1; o->bal++;}
   else {
    if(o->l->bal>-1) {if(!o->l->bal)konec=1;LL(pp=getP(o),1);} else  LR(pp=getP(o),1);
    o=*pp;
   }
  }
  left=(o->p && o->p->l==o);
  o=o->p;
 }
 return d;
}
void *HoTree::del(void *key){
 int i, j;
 HoTreeNode *o, *s;
 if(!root)return NULL;
 if(!(i=cmp(key,root->key))) return pomAVLDel(&root);
 s=o=root;
 while(1){
  if(i<0)s=s->l; else s=s->r;
  if(!s)return NULL;
  j=cmp(key,s->key);
  if(!j)if(i<0)return pomAVLDel(&(o->l)); else return pomAVLDel(&(o->r));
  i=j;
  o=s;
 }
}

int HoTree::pomInOrder(HoTreeNode *n, void *param, int (* what)(void *, void *, void *)){
 if(!n) return 1;
 return pomInOrder(n->l, param, what) &&
        what(n->key, n->data, param) &&
        pomInOrder(n->r, param, what);
}
int HoTree::inOrder(void *param, int (* what)(void *, void *, void *)){
 return pomInOrder(root, param, what);
}
int HoTree::depth(){
 return pomDepth(root);
}

int HoTree::size(){
 return pomSize(root);
}

void *HoTree::getMin(void){
	HoTreeNode *r=root;
	if(!root)return NULL;
	while(r->l)r=r->l;
	return r->data;
}
void *HoTree::getMinKey(void){
	HoTreeNode *r=root;
	if(!root)return NULL;
	while(r->l)r=r->l;
	return r->key;
}


int HoTree::pomDepth(HoTreeNode *n){
 int i, j;
 if(!n)return 0;
 i=pomDepth(n->l);
 j=pomDepth(n->r);
 if(i>j)return ++i; else return ++j;
}

int HoTree::pomSize(HoTreeNode *n){
  if (!n) return 0;
  return pomSize(n->l) + pomSize(n->r) + 1;
}

/*
void HoTree::LL(HoTreeNode **n){
 HoTreeNode *p;

 p=*n;
 p->bal=0;
 p->l->bal=0;
 *n=p->l;
 p->l=(*n)->r;
 (*n)->r=p;

 p=*n;
 p->p=p->r->p;
 p->r->p=p;
 p=p->r;
 if(p->l)p->l->p=p;
}
*/
void HoTree::LL(HoTreeNode **n, int dl){
 HoTreeNode *p;

 p=*n;
 if(!dl){
 p->bal=0;
 p->l->bal=0;
 }else{
 p->bal-=p->l->bal;
 p->l->bal--;
 }

 *n=p->l;
 p->l=(*n)->r;
 (*n)->r=p;

 p=*n;
 p->p=p->r->p;
 p->r->p=p;
 p=p->r;
 if(p->l)p->l->p=p;
}
void HoTree::RR(HoTreeNode **n, int dl){
 HoTreeNode *p;
 p=*n;
 if(!dl){
 p->bal=0;
 p->r->bal=0;
 }else{
 p->bal-=p->r->bal;
 p->r->bal++;
 }

 *n=p->r;
 p->r=(*n)->l;
 (*n)->l=p;

 p=*n;
 p->p=p->l->p;
 p->l->p=p;
 p=p->l;
 if(p->r)p->r->p=p;
}

void HoTree::LR(HoTreeNode **n, int dl){
 HoTreeNode *p;

 p=*n;
 if(dl)p->bal++;
 *n=p->l->r;
 p->l->r=(*n)->l;
 (*n)->l=p->l;
 p->l=(*n)->r;
 (*n)->r=p;

 p=*n;

 if(p->bal>-1)p->l->bal++;else p->l->bal+=2;
 if(p->bal==1)p->r->bal=-1; else p->r->bal=0;
 p->bal=0;

 p->p=p->r->p;
 p->r->p=p;
 p->l->p=p;
 if(p->l->r)p->l->r->p=p->l;
 if(p->r->l)p->r->l->p=p->r;
}

void HoTree::RL(HoTreeNode **n, int dl){
 HoTreeNode *p;

 p=*n;
 if(dl)p->bal--;
 *n=p->r->l;
 p->r->l=(*n)->r;
 (*n)->r=p->r;
 p->r=(*n)->l;
 (*n)->l=p;
  p=*n;

 if(p->bal<1)p->r->bal--;else p->r->bal-=2;
 if(p->bal==-1)p->l->bal=1; else p->l->bal=0;
 p->bal=0;

 p->p=p->l->p;
 p->l->p=p;
 p->r->p=p;
 if(p->l->r)p->l->r->p=p->l;
 if(p->r->l)p->r->l->p=p->r;
}

int cmpVoid(void *a, void *b) {
  if (a < b) return -1;
  if (a > b) return 1;
  return 0;
}
