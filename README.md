# SlabPool
A C++ template implementation of slab pool

```c

class ObjInSlab {
public:
  ObjInSlab *poolNext;
  Slab<ObjInSlab> *slab;

// other data
public:
  int v;
};
```

