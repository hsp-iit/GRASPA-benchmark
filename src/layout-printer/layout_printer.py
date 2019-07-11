import numpy as np
from stl import mesh
import cv2
import xml.etree.ElementTree as et
from matplotlib import pyplot as plt
from mpl_toolkits import mplot3d
import os

def projectMeshOnImage(image, mesh_filename, transform, density):

    input_mesh = mesh.Mesh.from_file(mesh_filename)

    input_mesh.transform(transform)

    # Create a new plot

    #figure = plt.figure()
    #ax1 = mplot3d.Axes3D(figure)

    # Load the STL files and add the vectors to the plot

    #ax1.add_collection3d(mplot3d.art3d.Poly3DCollection(input_mesh.vectors))

    # Auto scale to the mesh size

    # scale = input_mesh.points.flatten(-1)
    # ax1.auto_scale_xyz(scale, scale, scale)

    #plt.show()

    resolution_img_x = image.shape[1]
    resolution_img_y = image.shape[0]

    #   Alternative approach, with faces

    vectors = input_mesh.points
    indexed_mesh_points = np.int32(vectors*1000*density)

    #   Draw each face as a polygon

    for point_idx in range(indexed_mesh_points.shape[0]):
        poly = np.array([indexed_mesh_points[point_idx, 0:2], indexed_mesh_points[point_idx, 3:5],indexed_mesh_points[point_idx, 6:8]], np.int32)
        cv2.fillConvexPoly(image, poly, 0)



ROOT_DIR = '/home/fbottarel/robot-code/RAL-benchmark-code'

scene_filename = os.path.join(ROOT_DIR, 'data/scenes/grasping/3D_scenes/layout_0_mod.xml')

canvas_size_x = 594 # mm
canvas_size_y = 420 # mm

density = 10 #dots/mm

resolution_img_x = canvas_size_x*density
resolution_img_y = canvas_size_y*density

marker_root_origin_x = 589 # mm
marker_root_origin_y = 415 # mm

image = np.ones((resolution_img_y, resolution_img_x), np.uint8)*255

#   Draw the aruco board
#   the bottom right of the aruco board will have to be placed on marker_root_origin
#   markers have 3 units for edge, 1 unit for spacing, therefore to fit them in an image without white borders
#   5826x4100 is a good size (also including the 50 pixel margin by both sides), but we need to pad that
#   Incidentally, the padding is 14 pixels total (7 per side)

size_aruco_x = resolution_img_x - 114
size_aruco_y = resolution_img_y - 100

dictionary = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_100)
aruco_board = cv2.aruco.GridBoard_create(7, 5, 300, 100, dictionary)

aruco_img = aruco_board.draw((size_aruco_x, size_aruco_y), 0)

#   remove the inner rows and cols of aruco markers

aruco_img[862: 4100-862, 862: 5826-862] = 255

#   paste the aruco image as a background

image[50 : 4200-50, 50+7 : 5940-57] = aruco_img

#   parse the xml file

tree = et.parse(scene_filename)
root = tree.getroot()

#   list the manipulation objects
print('ManipulationObjects in scene:')
for manip_object in root.findall('ManipulationObject'):
    print (manip_object.attrib['name'])

    #   parse .stl filename

    filename_element = manip_object.find('File')
    mesh_filename = os.path.join(ROOT_DIR, filename_element.text)
    mesh_filename = mesh_filename.replace('.xml', '.stl')

    #   parse object transform

    matrix_4x4 = manip_object.find('GlobalPose').find('Transform').find('Matrix4x4')
    transform = np.identity(4, np.float32)
    for row_idx in range(3):
        transform[row_idx, 0] = np.float32(matrix_4x4[row_idx].attrib['c1'])
        transform[row_idx, 1] = np.float32(matrix_4x4[row_idx].attrib['c2'])
        transform[row_idx, 2] = np.float32(matrix_4x4[row_idx].attrib['c3'])
        transform[row_idx, 3] = np.float32(matrix_4x4[row_idx].attrib['c4']) / 1000 # pose is in mm, we need meters

    #   benchmark board origin is at the bottom right, indexes start from top left.
    #   we need to flip the ref system around the x axis and translate it

    theta = np.radians(-180)
    c, s = np.cos(theta), np.sin(theta)
    matfix = np.identity(4)
    matfix[1:3, 1:3] = np.array(((c, -s), (s, c)), np.float32)

    #   fix the offset of the ref frame (in meters). The object poses will be transformed around this point

    matfix[0,3] = marker_root_origin_x/1000
    matfix[1,3] = marker_root_origin_y/1000

    print(transform)

    transform = np.dot(matfix, transform)

    projectMeshOnImage(image, mesh_filename, transform, density)

cv2.imwrite('output.png', image)

#   size of a4 page in pixels

a4_size_x = 297 * density
a4_size_y = 210 * density

#image_a4 = 255* np.ones((a4_size_y*2, a4_size_x*2), np.uint8)

# divide it in 4 a4 pages

cv2.imwrite('output-page1.png', image[0:2100, 0:2970])
cv2.imwrite('output-page2.png', image[2100:4200, 0:2970])
cv2.imwrite('output-page3.png', image[0:2100, 2970:5940])
cv2.imwrite('output-page4.png', image[2100:4200, 2970:5940])





