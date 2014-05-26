#ifndef HoTreeH
#define HoTreeH
#ifndef NULL
#define NULL 0
#endif

/**
  Funkce porovn�vaj�c� void * pointery. V�choz� pro porovn�v�n� kl��� stromu.
*/
int cmpVoid(void *a, void *b); 

/**
 T��da reprezentuj�c� prvek AVL stromu.
*/
class HoTreeNode{
  public:
    /**
      Konstruktor p�i�azen�m prom�nn�ch.
      @param key kl��
      @param data data
      @param l lev� podstrom
      @param r prav� podstrom
      @param p rodi�
      @param bal rozd�l hloubek
    */
    HoTreeNode(void *key, void *data, HoTreeNode *l = NULL,
      HoTreeNode *r = NULL, HoTreeNode *p = NULL, int bal = 0);

  /**
    Uvoln� data, pokud nejsou NULL.
  */
  void freeData();
  
  /**
    Uvoln� kl��, pokud nen� NULL.
  */
  void freeKey();

  /**
    Pr�zdn� destruktor, nepokou�� se nic dealokovat.
  */
  ~HoTreeNode() {};
  /**
    Lev� podstrom.
  */
  HoTreeNode *l;
  /**
    Prav� podstrom.
  */
  HoTreeNode *r;
  /**
    Rodi�.
  */
  HoTreeNode *p;
  /**
    Vyv�en�, rozd�l v��ek postrom�.
    1 = vlevo v�t��, 0 stejn�, -1 vpravo v�t�� 
  */
  int bal;
  /**
    Kl��, podle kter�ho se vyhled�v�. Nap��klad handle okna.
  */
  void *key;
  /**
    Ulo�en� data. Nap��klad hlib objekt okna.
  */
  void *data;
};

/**
  T��da reprezentuj�c� AVL strom s prvky jednozna�n� identifikovan�mi
  pomoc� kl��e. Duplicity nejsou p��pustn�, insert korektn� vr�t� chybu.
  Kl��e i data jsou typu void *, porovn�v� se pomoc� u�ivatelem definovan�
  funkce.
*/
class HoTree {
  public:
    /**
      Ko�en stromu.
    */
    HoTreeNode *root;
    /**
      Kdy� se ma�e prvek, maj� se dealokovat data pomoc� free? Ne nap��klad
      pokud jsou fakticky int a ne void *, nebo je chci dealokovat jinde.
    */
    int freeData;
    /**
      Kdy� se ma�e prvek, m� se dealokovat kl�� pomoc� free? Ne nap��klad
      pokud je fakticky int a ne void *, nebo ho chci dealokovat jinde.
    */
    int freeKey;
    /**
      Funkce, kter� porovn�v� dva kl��e. -1 l < r, 1 l > r, 0 l = r.
      Funkce je zadan� u�ivatelem v konstruktoru.
    */
    int (*cmp)(void *l, void *r);
    /**
      Vlo�en� nov�ho prvku.
      @return data nebo NULL v p��pad� ne�sp�chu (duplicita kl��e)
    */
    virtual void *ins(void *key, void *data);
    /**
      Smaz�n� prvku.
    */
    virtual void *del(void *key);
    /**
      Vrac� nalezen� data nebo NULL, pokud tam nejsou
    */
    virtual void *get(void *key);
    /**
      Je kl�� v podstromu.
    */
    virtual int is(void *key);
    /**
      Vrac� data minim�ln�ho kl��e.
    */
    virtual void *getMin(void);
    /**
      Vrac� minim�ln� kl��.
    */
    virtual void *getMinKey(void);
    /**
      Sma�e cel� strom.
    */
    virtual void delAll(void);
    /**
      Projde cel� strom podle velikosti prvk� a na ka�d� provede
      what(key, data, param), pokud n�jak� what vr�t� 0, pr�chod se p�eru��.
      Pozor: B�hem metody inOrder nesm� b�t m�n�na struktura stromu, tj. nesm�
      b�t ��dn� prvky p�id�v�ny ani maz�ny a to ani v r�mci funkce what.
      Tak�e nap��klad nelze pomoc� inOrderu pomoc� del z what smazat cel�
      strom.
    */
    virtual int inOrder(void *param, int (* what)(void *, void *, void *));
    /**
      Hloubka stromu.
    */
    virtual int depth();
    /**
      Po�et prvk� stromu.
    */
    virtual int size();
    /**
      Je strom pr�zdn�?
    */
    virtual int empty() {return !root;};
    /**
      Konstruktor nastaven�m prom�nn�ch.
    */
    HoTree(int (*cmp)(void *l, void *r) = cmpVoid, int freeData = 1,
      int freeKey = 1);
    /**
      Destruktor, zavol� dellAll.
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

