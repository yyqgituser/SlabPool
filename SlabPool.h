#ifndef _SLAB_POOL_H
#define _SLAB_POOL_H

#include <cstdlib>
#include <MiniList.h>

// member requirement to use this SlabPool
/*
class ObjInSlab {
public:
  ObjInSlab *poolNext;
  Slab<ObjInSlab> *slab;

// other data
public:
  int v;
};
*/

template<typename T> class Slab {
  public:
    // for list
    Slab<T> *next;
    Slab<T> *prev;

    T* freeObj;
    unsigned int inuse;
    T* objArray;

    Slab() : inuse(0) {
    }
};

template<typename T> class SlabPool {
  private:
    MiniList<Slab<T> >  fullSlabs;
    MiniList<Slab<T> >  partialSlabs;
    MiniList<Slab<T> >  freeSlabs;

    unsigned int objectNumPerSlab;
    unsigned int reapThreshHold;

    unsigned int slabNum;
    unsigned int objInuse;

  public:
    unsigned int allocCnt;
    unsigned int releaseCnt;
    unsigned int sysNewCnt;
    unsigned int sysFreeCnt;

  public:
    SlabPool(unsigned int objectNumPerSlab, unsigned int reapThreshHold) :
      objectNumPerSlab(objectNumPerSlab), reapThreshHold(reapThreshHold), slabNum(0),
      objInuse(0), allocCnt(0), releaseCnt(0), sysNewCnt(0), sysFreeCnt(0) {
      if(objectNumPerSlab <= 1 || reapThreshHold == 0) {
        abort();
      }
    }

    ~SlabPool() {
      if(objInuse != 0) {
        abort();
      }

      while (!freeSlabs.isEmpty())    {
        reapOneSlab();
      }
    }


    // allocate a object from slab pool
    T* alloc() {
      Slab<T>* slab;
      T* obj;

      if (partialSlabs.isEmpty()) {
        if (freeSlabs.isEmpty()) {
          newOneSlab();
        }

        slab = freeSlabs.first();
        freeSlabs.remove(slab);
        partialSlabs.insertFirst(slab);
      }
      else {
        slab = partialSlabs.first();
      }

      obj = slab->freeObj;
      slab->freeObj = obj->poolNext;

      /* update slab status */
      slab->inuse++;
      if (slab->inuse == objectNumPerSlab) {
        partialSlabs.remove(slab);
        fullSlabs.insertLast(slab);
      }

      allocCnt++;
      objInuse++;

      return obj;
    }

    // release a object which was allocated from this slab pool
    void release(T *obj) {
      Slab<T> *slab = obj->slab;

      obj->poolNext = slab->freeObj;
      slab->freeObj = obj;

      objInuse--;
      releaseCnt++;

      if (slab->inuse == 1) {
        // if inuse == 1, we assume the slab is in partial, so we don't directly move a slab from full
        // to free, that is why each slab at least has 2 objects
        partialSlabs.remove(slab);
        // LILO may improve perf
        freeSlabs.insertFirst(slab);
      }
      else if(slab->inuse == objectNumPerSlab) {
        fullSlabs.remove(slab);
        partialSlabs.insertLast(slab);
      }
      slab->inuse--;

      if(freeSlabs.size() >= reapThreshHold) {
        reapOneSlab();
      }
    }

  private:
    void newOneSlab() {
      T *objArray = new T[objectNumPerSlab];
      Slab<T> *slab = new Slab<T>();
      slab->objArray = objArray;
      slab->freeObj = 0;
      freeSlabs.insertFirst(slab);

      for(unsigned int i=0; i < objectNumPerSlab; i++) {
        T *obj = &objArray[i];
        obj->slab = slab;
        obj->poolNext = slab->freeObj;
        slab->freeObj = obj;
      }

      slabNum++;
      sysNewCnt++;
    }

    void reapOneSlab() {
      Slab<T> *slab = freeSlabs.first();

      freeSlabs.remove(slab);

      delete[] slab->objArray;
      delete slab;

      slabNum--;
      sysFreeCnt++;
    }
};

#endif
