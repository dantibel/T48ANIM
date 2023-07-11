typedef struct tagSTOCK
{
  STOCK_MTL mtls;
  STOCK_PRIM prms;
  STOCK_TEXTURES texs;
} STOCK;

typedef struct tagMTL_LIST2 MTL_LIST2;
struct tagMTL_LIST2
{
  MTL_LIST2 *Next;
  MTL_LIST2 *Prev;
  MTL_LIST2 *mtl;
}

typedef struct tagMTL_LIST MTL_LIST;
struct tagMTL_LIST
{
  MTL_LIST *Next;
  MTL_LIST *mtl;
}

typedef struct tagMTLSTOCK
{
  MTL_LIST2 *MtlStock;
  MTL_LIST *FirstFree;
  MTL_LIST *FirstToDelete;
} MTL_STOCK;

VOID MtlStockInit( VOID );
VOID MtlAddToStock( MTL *Mtl );
VOID MtlStockClose( VOID );


