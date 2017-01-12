# SlabPool
A C++ template implementation of slab pool

```c

class Int {
public:
  ObjInSlab *poolNext;
  Slab<ObjInSlab> *slab;

// other data
public:
  int v;
};

int main() {
  Int *obj;
  
  SlabPool<Int> pool(20, 1);
  obj = pool.alloc();
  pool.release(obj);
  
  return 0;
}
```

