cdef class CDT:

    cdef c_CDT *me
    cdef point_vec polyline
    
    def __init__(self, polyline):
        print("CDT---__init__---CDT")
        self.polyline = pointvec_factory(0)
        for point in polyline:
            self.polyline.push_back(new_Point(point.x, point.y, point.z))
        self.me = new_CDT(self.polyline)
        
    def triangulate(self):
        cdef Point3 a,b,c
        self.me.Triangulate()
        cdef triangle_vec tri_list = self.me.GetTriangles()
        cdef list tris = []
        for i in range(tri_list.size()):
            a = Point3(tri_list.get(i).GetPoint(0).x, tri_list.get(i).GetPoint(0).y, tri_list.get(i).GetPoint(0).z)
            b = Point3(tri_list.get(i).GetPoint(1).x, tri_list.get(i).GetPoint(1).y, tri_list.get(i).GetPoint(1).z)
            c = Point3(tri_list.get(i).GetPoint(2).x, tri_list.get(i).GetPoint(2).y, tri_list.get(i).GetPoint(2).z)
            tris.append(Triangle3(a, b, c))
        return tris
    
    def add_hole(self, polyline):
        cdef point_vec hole = pointvec_factory(0)
        for point in polyline:
            hole.push_back(new_Point(point.x, point.y, point.z))
        self.me.AddHole(hole)
        
    def add_point(self, point):
        cdef c_Point* p = new_Point(point.x, point.y, point.z)
        self.me.AddPoint(p)

    def __dealloc__(self):
        del_CDT(self.me)
