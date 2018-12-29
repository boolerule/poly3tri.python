# 载入模块
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from p3t import *
import numpy as np
def load_points(file_name):
    infile = open(file_name, "r")
    points = []
    while infile:
        line = infile.readline()
        s = line.split()
        if len(s) == 0:
            break
        points.append([float(s[0]), float(s[1]), float(s[2])])
    return points
def main(file_name,hole_file_name, translate, zoom):
    points = load_points(file_name)
    hole_points = load_points(hole_file_name)
    polyline = []

    for p in points:
        p[0] = p[0]*zoom + translate[0]
        p[1] = p[1] * zoom + translate[1]
        p[2] = p[2] * zoom + translate[2]

        polyline.append(Point3(p[0],p[1],p[2] ))

    hole_polyline = []

    for p in hole_points:
        p[0] = p[0] * zoom + translate[0]
        p[1] = p[1] * zoom + translate[1]
        p[2] = p[2] * zoom + translate[2]

        hole_polyline.append(Point3(p[0], p[1],p[2]))
    cdt = CDT(polyline)
    if hole_polyline:
        print("存在孔洞！")
        # 增加一个孔
        cdt.add_hole(hole_polyline)
    triangles = cdt.triangulate()
    # 创建 3D 图形对象
    fig = plt.figure()
    ax = Axes3D(fig)
    for t in triangles:
        p0 = [t.a.x, t.a.y, t.a.z]
        p1 = [t.b.x, t.b.y, t.b.z]
        p2 = [t.c.x, t.c.y, t.c.z]
        x = [t.a.x, t.b.x, t.c.x,t.a.x]
        y = [t.a.y, t.b.y, t.c.y,t.a.y]
        z = [t.a.z, t.b.z, t.c.z,t.a.z]
        # 绘制线型图
        ax.plot(x, y, z)

    # 显示图
    plt.show()
if __name__=="__main__":
    main("data/outputA0.dat", "data/outputA1.dat", [0, 0, 0], 1)



