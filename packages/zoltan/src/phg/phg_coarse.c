/*****************************************************************************
 * Zoltan Library for Parallel Applications                                  *
 * Copyright (c) 2000,2001,2002, Sandia National Laboratories.               *
 * For more info, see the README file in the top-level Zoltan directory.     *
 *****************************************************************************/
/*****************************************************************************
 * CVS File Information :
 *    $RCSfile$
 *    $Author$
 *    $Date$
 *    $Revision$
 ****************************************************************************/

 

#ifdef __cplusplus
/* if C++, define the rest of this header file as extern C */
extern "C" {
#endif

#include "phg.h"

/****************************************************************************/

/* when initial development is done, move this to phg_hypergraph.h */
typedef struct {
  float weight;    /* vertex weight */
  int gno;         /* global number of vertex */
  int nHEdge;      /* number of hyperedges (non zeros) in this rowblock */
  int HEdge[1];    /* array containing hyperedge gno's */
  } VInfo;

typedef struct Hash_Node {
  int gno;         /* key */
  int lno;         /* data associated with key */
  struct Hash_Node *next;
  } Hash_Node;
  
static int phg_hash (int key, int size)
{
  return (key * 2654435761U) % size;
}

static int phg_hash_lookup (Hash_Node **htable, int key, int n)
{
  Hash_Node *ptr;

  for (ptr = htable[(key * 2654435761U) % n]; ptr != NULL; ptr = ptr->next)
    if (key == ptr->gno)
      return (ptr->lno);
  return -1;
}

  

#define MSG_TAG1 28337     /* arbitrary message tags */
#define MSG_TAG2 28338
#define MSG_TAG3 28339

/***************************************************************************/

    
    
/* Procedure to coarsen a hypergraph based on a matching. All vertices of one
   match are clustered to one vertex. Identical hyperedges are collapsed to a
   single hyperedge with combined weight. The array LevelMap is the mapping of
   the old vertices to the new vertices. It will be used to pass a partition
   of the coarse graph back to the original graph.                         */
   
int Zoltan_PHG_Coarsening
( ZZ *zz,               /* the Zoltan data structure */
  PHGraph  *hg,         /* information about hypergraph, weights, etc. */
  int      *match,      /* Matching, Packing or Grouping array */
  PHGraph  *c_hg,       /* points to a working copy of hg structure */
  int      *LevelMap)   /* information to reverse coarsenings later */
{
  int i, j, k, old, vertex, new_vertex, maxgno, mingno, edge;
  int err, nrec, rsize, size, gno;
  int *used_edges=NULL, *c_vindex=NULL, *c_vedge=NULL;
  int *x_lno=NULL, *x_dest=NULL, *x_count=NULL, x_total;
  float *c_ewgt=NULL;
  char *sbuffer=NULL, *rbuffer=NULL, *position=NULL;
  VInfo **vlist;
  Hash_Node **htable=NULL, *hash_nodes;
  ZOLTAN_COMM_OBJ *plan=NULL;  
  char *yo = "Zoltan_PHG_Coarsening";

  ZOLTAN_TRACE_ENTER (zz, yo);
  
  Zoltan_PHG_HGraph_Init (c_hg);   /* initializes working copy of hypergraph info */
  c_hg->info  = hg->info + 1;      /* for debugging */
  c_hg->ratio = hg->ratio;         /* for "global" recursive bisectioning */
  c_hg->redl  = hg->redl;          /* to stop coarsening near desired count */
  
  maxgno = hg->dist_x[hg->comm->myProc_x + 1] - 1;    /* last  gno of my column */
  mingno = hg->dist_x[hg->comm->myProc_x];            /* first gno of my column */

  /* allocate x_lno[], x_count, x_dest[] to worst case sizes. Another (pre) pass 
     over match[] (below) could allow allocating the exact size arrays instead! */
  if (!(x_lno   = (int*) ZOLTAN_MALLOC (hg->nVtx * sizeof(int)))
   || !(x_dest  = (int*) ZOLTAN_MALLOC (hg->nVtx * sizeof(int)))  
   || !(x_count = (int*) ZOLTAN_MALLOC (hg->nVtx * sizeof(int))))  {  
     Zoltan_Multifree (__FILE__, __LINE__, 3, x_lno, x_dest, x_count);
     ZOLTAN_PRINT_ERROR(zz->Proc, yo, "Insufficient memory.");
     ZOLTAN_TRACE_EXIT (zz, yo);
     return ZOLTAN_MEMERR;
  }
    
  /* Assume that every column has the entire (column's) matching information */
  /* Calculate the number of coarse vertices. match[vertex] -> -match[vertex]-1 */
  c_hg->nVtx = 0;   /* counts number of new (coarsened) vertices */
  x_total = 0;      /* counts number of off processor vertices I need */
  size = 0;
  for (i = 0; i < hg->nVtx; i++)  {    /* loop over every local vertice */
    if (match[i] < 0)
      continue;              /* already took care of this vertex */
      
    if (match[i] > maxgno || match[i] < mingno)  {  /* external processor match */
      int gx, lx, proc_x;
      gx = VTX_TO_PROC_X (hg, match[i]);
      lx = hg->comm->myProc_x;
      
      /* rule to determine "ownership" of coarsened vertices across procs */      
      proc_x = ((mingno+i+match[i]) & 1) ? MIN(gx, lx) : MAX(gx, lx);
      if (proc_x != lx)   {             /* another processor owns this vertex */
        x_count [x_total] = hg->vindex[i+1] - hg->vindex[i];      
        x_dest  [x_total] = proc_x;        /* need to send it my vertex info */
        x_lno   [x_total] = i;             /* my vertex's local number */
        size += x_count [x_total++] ;      /* counts number of edge nonzeros */
      }
      else { 
        /* myProc owns a matching (NOT grouping or packing) across processors */ 
        c_hg->nVtx++;
        match[i] = -match[i] - 1;
      }
    }
    else  {      /* allow for possible (local only) packing and groupings */
      c_hg->nVtx++;
      vertex = i;
      while (match[vertex] >= 0) {
        old        =  vertex;
        vertex     =  match[old] - mingno;    /* local number of vertex */
        match[old] = -match[old] - 1;
      }
    }
  }
  if (!(c_hg->vwgt = (float*) ZOLTAN_CALLOC (c_hg->nVtx, sizeof(float))))  {
    Zoltan_Multifree (__FILE__, __LINE__, 4, x_lno, x_dest, x_count, c_hg->vwgt);
    ZOLTAN_PRINT_ERROR(zz->Proc, yo, "Insufficient memory.");
    ZOLTAN_TRACE_EXIT (zz, yo);
    return ZOLTAN_MEMERR;
  }

  /* size and allocate the send buffer */
  size += (x_total * (sizeof(VInfo) - sizeof (int)));
  if (!(sbuffer = (char*) ZOLTAN_MALLOC (size))) {
    Zoltan_Multifree (__FILE__, __LINE__, 5, x_lno, x_dest, x_count, c_hg->vwgt,
     sbuffer);
    ZOLTAN_PRINT_ERROR(zz->Proc, yo, "Insufficient memory.");
    ZOLTAN_TRACE_EXIT (zz, yo);
    return ZOLTAN_MEMERR;
  }
  
  /* fill send buffer */
  position = sbuffer;
  for (i = 0; i < x_total; i++)   {
    VInfo *v  = (VInfo*) position;
    v->gno    = x_lno[i] + mingno;
    v->weight = hg->vwgt[x_lno[i]];
    v->nHEdge = x_count[i];

    /* now fill in the variable count of hyperedge lno's */
    j = 0;
    for (k = hg->hindex[x_lno[i]]; k < hg->hindex[x_lno[i] +1]; k++)
       v->HEdge[j++] = hg->hvertex[k];

    size = ((x_count[i]-1) * sizeof(int) + sizeof (VInfo));    
    position   += size;
    x_count[i]  = size;  /* convert from count to size for Zoltan_Comm_Resize */
  }
    
  Zoltan_Comm_Create (&plan, x_total, x_dest, zz->Communicator, MSG_TAG1, &nrec);
  Zoltan_Comm_Resize (plan, x_count, MSG_TAG2, &rsize);
  
  /* allocate the receive buffer */
  if (!(rbuffer = (char*) ZOLTAN_MALLOC (rsize*sizeof(int)))) {
    Zoltan_Multifree (__FILE__, __LINE__, 6, x_lno, x_dest, x_count, c_hg->vwgt,
     sbuffer, rbuffer);  
    ZOLTAN_PRINT_ERROR(zz->Proc, yo, "Insufficient memory.");
    ZOLTAN_TRACE_EXIT (zz, yo);
    return ZOLTAN_MEMERR;
  }
    
  err = Zoltan_Comm_Do (plan, MSG_TAG3, sbuffer, 1, rbuffer);  
  Zoltan_Comm_Destroy (&plan);
  
  if (!(vlist      = (VInfo**)     ZOLTAN_MALLOC (nrec * sizeof(int)))
   || !(hash_nodes = (Hash_Node*)  ZOLTAN_MALLOC (nrec * sizeof(Hash_Node)))
   || !(htable     = (Hash_Node**) ZOLTAN_MALLOC (nrec * sizeof(Hash_Node))))  {
    Zoltan_Multifree (__FILE__, __LINE__, 8, x_lno, x_dest, x_count, c_hg->vwgt,
     sbuffer, rbuffer, vlist, htable);   
    ZOLTAN_PRINT_ERROR(zz->Proc, yo, "Insufficient memory.");
    ZOLTAN_TRACE_EXIT (zz, yo);
    return ZOLTAN_MEMERR;
  }  

  /* index all received data and fill lookup hash table */
  position = rbuffer;
  for (i = 0; i < nrec; i++)  {
    vlist[i] = (VInfo*) position;     
    hash_nodes[i].gno = vlist[i]->gno;
    hash_nodes[i].lno = i;
    position += (sizeof(VInfo) + sizeof(int) * (vlist[i]->nHEdge-1)) ;
    }
  for (i = 0; i < nrec; i++)  {
    j = phg_hash (hash_nodes[i].gno, nrec);
    hash_nodes[i].next = htable[j];
    htable[j] = &hash_nodes[i];
  }
  
  if (!(used_edges = (int*)   ZOLTAN_CALLOC (c_hg->nEdge,     sizeof(int)))
   || !(c_ewgt     = (float*) ZOLTAN_MALLOC (hg->nEdge      * sizeof(float)))
   || !(c_vindex   = (int*)   ZOLTAN_MALLOC ((hg->nVtx+1)   * sizeof(int)))
   || !(c_vedge    = (int*)   ZOLTAN_MALLOC (hg->nNonZero   * sizeof(int)))) {
      Zoltan_Multifree (__FILE__, __LINE__, 12, x_lno, x_dest, x_count, c_hg->vwgt,
       sbuffer, rbuffer, vlist, htable, &used_edges, &c_ewgt, &c_vindex, &c_vedge);
      ZOLTAN_PRINT_ERROR(zz->Proc, yo, "Insufficient memory.");
      ZOLTAN_TRACE_EXIT (zz, yo);
      return ZOLTAN_MEMERR;
  }  

  /* Construct the LevelMap; match[vertex] is changed back to original value */
  /* Coarsen vertices (create vindex, vedge), sum up coarsened vertex weights */
  c_hg->nNonZero = 0;   /* count of coarsened pins */
  c_hg->nVtx     = 0;   /* count of coarsened vertices */
  new_vertex     = 0;       /* counts LevelMap entries */
  for (i = 0; i < hg->nVtx; i++)  {
    gno = -match[i] - 1;
    if (match[i] >= 0)                             /* match to external vertex */
      LevelMap [new_vertex++] = gno;  /* negative value entry is external flag */
                  
    else if (gno < mingno || gno > maxgno) {     /* match from external vertex */
      LevelMap[i] = new_vertex;
      
      c_hg->vwgt  [c_hg->nVtx] = hg->vwgt ? hg->vwgt[i] : 1.0;
      c_hg->vindex[c_hg->nVtx] = c_hg->nNonZero;
      
      for (j = hg->vindex[i]; j < hg->vindex[i+1]; j++)  {
        used_edges [hg->vedge[j]]     = i+1;
        c_hg->vedge[c_hg->nNonZero++] = hg->vedge[j];
      }
      
      k = phg_hash_lookup (htable, gno, hg->nVtx);           
      c_hg->vwgt[c_hg->nVtx] += vlist[k]->weight;    
      for (j = 0; j < vlist[k]->nHEdge; j++)  {
        edge = vlist[k]->HEdge[j];
        if (used_edges[edge] <= i)
          c_hg->vedge[c_hg->nNonZero++] = edge;
      } 
      new_vertex++;
      c_hg->nVtx++;          
    }
    else  {                           /* match, pack, group internal vertices */
      c_hg->vindex[c_hg->nVtx] = c_hg->nNonZero;
      
      vertex = i;
      while (match[vertex] < 0)  {    
        LevelMap[vertex] = new_vertex;      
        c_hg->vwgt[c_hg->nVtx] += hg->vwgt ? hg->vwgt[vertex] : 1.0;  

        for (j = hg->vindex[i]; j < hg->vindex[i+1]; j++)  {
          if (used_edges [hg->vedge[j]] <= i)  {
            used_edges [hg->vedge[j]]     = i+1;          
            c_hg->vedge[c_hg->nNonZero++] = hg->vedge[j];
          }              
        }                       
        match[vertex] = -match[vertex] - 1;
        vertex        =  match[vertex];
      }
      new_vertex++;
      c_hg->nVtx++;
    }
  }
  ZOLTAN_FREE ((void**) &used_edges);

  /* Done if there are no remaining vertices */
  if (c_hg->nVtx == 0)  {
    c_hg->ewgt = NULL;
    if (!(c_hg->vindex = (int*) ZOLTAN_CALLOC (1, sizeof(int))))  {
      ZOLTAN_PRINT_ERROR(zz->Proc, yo, "Insufficient memory.");
      ZOLTAN_TRACE_EXIT (zz, yo);
      return ZOLTAN_MEMERR;
    }
    c_hg->vedge = NULL;
    Zoltan_Multifree (__FILE__, __LINE__, 11, x_lno, x_dest, x_count, c_hg->vwgt,
     sbuffer, rbuffer, vlist, htable, &c_ewgt, &c_vindex, &c_vedge);    
    ZOLTAN_TRACE_EXIT (zz, yo);
    return Zoltan_PHG_Create_Mirror(zz, c_hg);
  }

  Zoltan_Multifree (__FILE__, __LINE__, 11, x_lno, x_dest, x_count, c_hg->vwgt,
   sbuffer, rbuffer, vlist, htable, &c_ewgt, &c_vindex, &c_vedge);  
  /* RTHRTH: NOTE removed code per Umit's speedup hack from serial version HERE*/  
  c_hg->ewgt   = c_ewgt;
  c_hg->vindex = c_vindex;
  c_hg->vedge  = c_vedge;

  ZOLTAN_TRACE_EXIT (zz, yo);
  return Zoltan_PHG_Create_Mirror(zz, c_hg);
}



#ifdef __cplusplus
} /* closing bracket for extern "C" */
#endif
