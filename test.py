#!/usr/bin/env python2.6

import sys
from time import clock

from p3t import *

# PyGame Constants
import pygame
from pygame.gfxdraw import trigon, line
from pygame.locals import *
from pygame import Color
import numpy as np

import pointinpolygon
head_hole = [[325, 437],[320, 423], [329, 413], [332, 423]]
chest_hole = [[320.72342,480],[338.90617,465.96863],[347.99754,480.61584],
              [329.8148,510.41534], [339.91632,480.11077],[334.86556,478.09046]]

def load_points(file_name):
    infile = open(file_name, "r")
    points = []
    while infile:
        line = infile.readline()
        s = line.split()
        if len(s) == 0:
            break
        points.append([float(s[0]), float(s[1])])
    return points

def main(file_name,hole_file_name, translate, zoom):
    
    SCREEN_SIZE = 1096,840
    pygame.init()
    screen = pygame.display.set_mode(SCREEN_SIZE,0,8)
    pygame.display.set_caption('poly2tri demo')
    
    pygame.mouse.set_visible(True)
    
    black = Color(0,0,0)
    red = Color(255, 0, 0)
    green = Color(0, 255, 0)
    
    screen.fill(black)
    
    points = load_points(file_name)
    hole_points = load_points(hole_file_name)
    points = np.array(points) *1000
    hole_points = np.array(hole_points) * 1000
    hole_polyX = [x[0] for x in hole_points]
    hole_polyY = [x[1] for x in hole_points]
    points_polyX = [x[0] for x in points]
    points_polyY = [x[1] for x in points]
    test = pointinpolygon.Wall(len(hole_polyX), hole_polyX, hole_polyY)
    point_in_polygon = pointinpolygon.Wall(len(points_polyX), points_polyX, points_polyY)
    test.precalc_values()
    point_in_polygon.precalc_values()
    polyline = []

    for p in points:
        p[0] = p[0]*zoom + translate[0]
        p[1] = p[1]*zoom + translate[1]

        polyline.append(Point3(p[0],p[1],1))

    hole_polyline = []

    for p in hole_points:
        p[0] = p[0] * zoom + translate[0]
        p[1] = p[1] * zoom + translate[1]

        hole_polyline.append(Point3(p[0], p[1],10))

    # initialize clock
    t0 = clock()
   
    ##
    ## Step 1: Initialize
    ## NOTE: polyline must be a simple polygon. The polyline's points
    ## constitute constrained edges. No repeat points!!!
    ##
    cdt = CDT(polyline)
    if hole_polyline:
        print("存在孔洞！")
        # 增加一个孔
        cdt.add_hole(hole_polyline)

    
    ##
    ## Step 2: 如有必要，增加孔和内衬的施泰纳点
    ##
    if file_name == "data/dude.dat":
        hole = []  
        for p in head_hole:
            p[0] = p[0]*zoom + translate[0]
            p[1] = p[1]*zoom + translate[1]
            hole.append(Point(p[0],p[1]))
        # 增加一个孔
        cdt.add_hole(hole)
        hole = []  
        for p in chest_hole:
            p[0] = p[0]*zoom + translate[0]
            p[1] = p[1]*zoom + translate[1]
            hole.append(Point(p[0],p[1]))
        # 增加一个孔
        cdt.add_hole(hole)
        # 添加一个内部施泰纳点
        x = 361*zoom + translate[0]
        y = 381*zoom + translate[1]
        cdt.add_point(Point(x, y))
         
    ##
    ## Step 3: Triangulate
    ##
    triangles = cdt.triangulate()
    
    print ("Elapsed time (ms) = " + str(clock()*1000.0))
        
    # The Main Event Loop
    done = False

    while not done:
        index = 0
        index_1 = 0
        index_2 = 0
        index_point = 0
        count = 0
        # 画三角形
        for t in triangles:
            index_point += 3

            p0 = [t.a.x,t.a.y]
            p1 = [t.b.x,t.b.y]
            p2 = [t.c.x,t.c.y]
            # print("p0:", p0)
            # print("p1:", p1)
            # print("p2:", p2)
            # if test.pointInPolygon(p0[0], p0[1]):
            #     print("p0:%d,p1:%d,p1:%d"%( index,index_1,index_2))
            #     index += 1
            # if test.pointInPolygon(p1[0], p1[1]):
            #     print("p0:%d,p1:%d,p1:%d" % (index, index_1, index_2))
            #     index_1 += 1
            # if test.pointInPolygon(p2[0], p2[1]):
            #     print("p0:%d,p1:%d,p1:%d" % (index, index_1, index_2))
            #     index_2 += 1
            # if point_in_polygon.pointInPolygon(p0[0], p0[1]):
            #
            #     count += 1
            # if point_in_polygon.pointInPolygon(p1[0], p1[1]):
            #
            #     count += 1
            # if point_in_polygon.pointInPolygon(p2[0], p2[1]):
            #
            #     count += 1


            # if p0 in points :
            #   print("sb:",index)
            #   index += 1

            x1 = int(t.a.x/1000)
            y1 = int(t.a.y/1000)
            x2 = int(t.b.x/1000)
            y2 = int(t.b.y/1000)
            x3 = int(t.c.x/1000)
            y3 = int(t.c.y/1000)
            trigon(screen, x1, y1, x2, y2, x3, y3, red)
        #print("Face:%d,index_point:%d" % (index_point / 3, index_point))
        #print("sum:%d,hole_poly:%d,point_in_polygon:%d" % ((index_1+index_2+index)+count,(index_1+index_2+index), count))

        # 绘制轮廓
        # for i in range(len(points)):
        #     j = i+1 if i < len(points) - 1 else 0
        #     x1 = int(points[i][0])
        #     y1 = int(points[i][1])
        #     x2 = int(points[j][0])
        #     y2 = int(points[j][1])
        #     line(screen, x1, y1, x2, y2, green)
        
        # 如有需要，可打洞
        if file_name == "data/dude.dat":
            for i in range(len(head_hole)):
              j = i+1 if i < len(head_hole) - 1 else 0
              x1 = int(head_hole[i][0])
              y1 = int(head_hole[i][1])
              x2 = int(head_hole[j][0])
              y2 = int(head_hole[j][1])
              line(screen, x1, y1, x2, y2, green)
            for i in range(len(chest_hole)):
              j = i+1 if i < len(chest_hole) - 1 else 0
              x1 = int(chest_hole[i][0])
              y1 = int(chest_hole[i][1])
              x2 = int(chest_hole[j][0])
              y2 = int(chest_hole[j][1])
              line(screen, x1, y1, x2, y2, green)
              
        # 更新屏幕
        pygame.display.update()
            
        # 事件处理
        events = pygame.event.get( )
        for e in events:
            if( e.type == QUIT ):
                done = True
                break
            elif (e.type == KEYDOWN):
                if( e.key == K_ESCAPE ):
                    done = True
                    break
                if( e.key == K_f ):
                    pygame.display.toggle_fullscreen()
        
    return

if __name__=="__main__":
    if len(sys.argv) == 5:
      file_name = sys.argv[1]
      tx = float(sys.argv[2])
      ty = float(sys.argv[3])
      zoom = float(sys.argv[4])
      main(file_name, [tx, ty], zoom)
      exit()
    main("data/outputA0.dat","data/outputA4.dat", [0, 0], 2.5)
    print()
    print("  Usage: filename translate-x translate-y zoom")
    print("Example: python test.py data/dude.dat 100 -200 1")
    print("         python test.py data/nazca_monkey.dat 400 300 4.5")  

