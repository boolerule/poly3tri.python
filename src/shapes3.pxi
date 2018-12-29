cdef class Point3:

    cdef float x, y, z
    
    def __init__(self, float x, float y, float z):
        self.x = x
        self.y = y
        self.z = z
        
    property x:
        def __get__(self): return self.x
        def __set__(self, x): self.x = x
        
    property y:
        def __get__(self): return self.y
        def __set__(self, y): self.y = y
    property z:
        def __get__(self): return self.z
        def __set__(self, z): self.z = z
        
    def debug_print(self):
        print "(" + str(self.x) + ", " + str(self.y)+ ", " + str(self.z) + ")",
        
cdef class Triangle3:
    
    cdef Point3 a, b, c
    
    def __init__(self, Point3 a, Point3 b, Point3 c):
        self.a = a
        self.b = b
        self.c = c
        
    property a:
        def __get__(self): return self.a
    
    property b:
        def __get__(self): return self.b
    
    property c:
        def __get__(self): return self.c
        
    def debug_print(self):
        self.a.debug_print()
        self.b.debug_print()
        self.c.debug_print()
        print
        