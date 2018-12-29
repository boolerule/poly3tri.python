cdef extern from "../poly2tri/common/shapes3.h":

    ctypedef struct c_Point "p3t::Point3":
        double x, y ,z
    c_Point* new_Point "new p3t::Point3" (double x, double y, double z)
    void del_Point "delete" (c_Point* p)
    
    ctypedef struct c_Triangle "p3t::Triangle3":
        inline c_Point* GetPoint(int index)
    c_Triangle *new_Triangle "new p3t::Triangle3" (c_Point a, c_Point b, c_Point c)
    void del_Triangle "delete" (c_Triangle* t)
    
    ctypedef struct triangle_vec "std::vector<p3t::Triangle3*>":
        void (* push_back)(c_Triangle* elem)
        inline c_Triangle* get "operator[]" (int i)
        int size()
    triangle_vec trianglevec_factory "std::vector<p3t::Triangle3*>"(int len)
    
    ctypedef struct point_vec "std::vector<p3t::Point3*>":
        void (* push_back)(c_Point* elem)
        inline c_Point* get "operator[]" (int i)
        int size()
    point_vec pointvec_factory "std::vector<p3t::Point3*>"(int len)