#ifndef HoTreeH
#define HoTreeH
#ifndef NULL
#define NULL 0
#endif

/**
  Funkce porovnávající void * pointery. Výchozí pro porovnávání klíèù stromu.
*/
int cmpVoid(void *a, void *b); 

/**
 Tøída reprezentující prvek AVL stromu.
*/
class HoTreeNode{
  public:
    /**
      Konstruktor pøiøazením promìnných.
      @param key klíè
      @param data data
      @param l levý podstrom
      @param r pravý podstrom
      @param p rodiè
      @param bal rozdíl hloubek
    */
    HoTreeNode(void *key, void *data, HoTreeNode *l = NULL,
      HoTreeNode *r = NULL, HoTreeNode *p = NULL, int bal = 0);

  /**
    Uvolní data, pokud nejsou NULL.
  */
  void freeData();
  
  /**
    Uvolní klíè, pokud není NULL.
  */
  void freeKey();

  /**
    Prázdný destruktor, nepokou¹í se nic dealokovat.
  */
  ~HoTreeNode() {};
  /**
    Levý podstrom.
  */
  HoTreeNode *l;
  /**
    Pravý podstrom.
  */
  HoTreeNode *r;
  /**
    Rodiè.
  */
  HoTreeNode *p;
  /**
    Vyvá¾ení, rozdíl vý¹ek postromù.
    1 = vlevo vìt¹í, 0 stejné, -1 vpravo vìt¹í 
  */
  int bal;
  /**
    Klíè, podle kterého se vyhledává. Napøíklad handle okna.
  */
  void *key;
  /**
    Ulo¾ená data. Napøíklad hlib objekt okna.
  */
  void *data;
};

/**
  Tøída reprezentující AVL strom s prvky jednoznaènì identifikovanými
  pomocí klíèe. Duplicity nejsou pøípustné, insert korektnì vrátí chybu.
  Klíèe i data jsou typu void *, porovnává se pomocí u¾ivatelem definované
  funkce.
*/
class HoTree {
  public:
    /**
      Koøen stromu.
    */
    HoTreeNode *root;
    /**
      Kdy¾ se ma¾e prvek, mají se dealokovat data pomocí free? Ne napøíklad
      pokud jsou fakticky int a ne void *, nebo je chci dealokovat jinde.
    */
    int freeData;
    /**
      Kdy¾ se ma¾e prvek, má se dealokovat klíè pomocí free? Ne napøíklad
      pokud je fakticky int a ne void *, nebo ho chci dealokovat jinde.
    */
    int freeKey;
    /**
      Funkce, která porovnává dva klíèe. -1 l < r, 1 l > r, 0 l = r.
      Funkce je zadaná u¾ivatelem v konstruktoru.
    */
    int (*cmp)(void *l, void *r);
    /**
      Vlo¾ení nového prvku.
      @return data nebo NULL v pøípadì neúspìchu (duplicita klíèe)
    */
    virtual void *ins(void *key, void *data);
    /**
      Smazání prvku.
    */
    virtual void *del(void *key);
    /**
      Vrací nalezená data nebo NULL, pokud tam nejsou
    */
    virtual void *get(void *key);
    /**
      Je klíè v podstromu.
    */
    virtual int is(void *key);
    /**
      Vrací data minimálního klíèe.
    */
    virtual void *getMin(void);
    /**
      Vrací minimální klíè.
    */
    virtual void *getMinKey(void);
    /**
      Sma¾e celý strom.
    */
    virtual void delAll(void);
    /**
      Projde celý strom podle velikosti prvkù a na ka¾dý provede
      what(key, data, param), pokud nìjaké what vrátí 0, prùchod se pøeru¹í.
      Pozor: Bìhem metody inOrder nesmí být mìnìna struktura stromu, tj. nesmí
      být ¾ádné prvky pøidávány ani mazány a to ani v rámci funkce what.
      Tak¾e napøíklad nelze pomocí inOrderu pomocí del z what smazat celý
      strom.
    */
    virtual int inOrder(void *param, int (* what)(void *, void *, void *));
    /**
      Hloubka stromu.
    */
    virtual int depth();
    /**
      Poèet prvkù stromu.
    */
    virtual int size();
    /**
      Je strom prázdný?
    */
    virtual int empty() {return !root;};
    /**
      Konstruktor nastavením promìnných.
    */
    HoTree(int (*cmp)(void *l, void *r) = cmpVoid, int freeData = 1,
      int freeKey = 1);
    /**
      Destruktor, zavolá dellAll.
    */
    virtual ~HoTree();
  protected:
    virtual int pomInOrder(HoTreeNode *n, void *param,
      int (* what)(void *, void *, void *));
    virtual void delBranch(HoTreeNode *ro);
    virtual HoTreeNode **getP(HoTreeNode  *n);
    virtual void *pomAVLDel(HoTreeNode **n);
    virtual int pomDepth(HoTreeNode *n);
    virtual int pomSize(HoTreeNode *n);
    virtual HoTreeNode *right(HoTreeNode *ro);
    virtual void LL(HoTreeNode **n, int dl = 0);
    virtual void RR(HoTreeNode **n, int dl = 0);
    virtual void LR(HoTreeNode **n, int dl = 0);
    virtual void RL(HoTreeNode **n, int dl = 0);
    virtual void delNode(HoTreeNode *n);
};
#endif

