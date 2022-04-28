import numpy as np
from scipy.spatial.transform import Rotation as R
from stl import mesh
import cv2
import xml.etree.ElementTree as et
import os
import argparse

def projectMeshOnImage(image, mesh_filename, transform, density):

    input_mesh = mesh.Mesh.from_file(mesh_filename)

    transform_rot = transform[:3,:3]
    transform_rot_fixed = R.from_matrix(transform_rot).as_matrix()
    transform[:3,:3] = transform_rot_fixed

    input_mesh.transform(transform)

    resolution_img_x = image.shape[1]
    resolution_img_y = image.shape[0]

    #   Alternative approach, with faces

    vectors = input_mesh.points
    indexed_mesh_points = np.int32(vectors*1000*density)

    #   Draw each face as a polygon

    for point_idx in range(indexed_mesh_points.shape[0]):
        poly = np.array([indexed_mesh_points[point_idx, 0:2], indexed_mesh_points[point_idx, 3:5],indexed_mesh_points[point_idx, 6:8]], np.int32)
        cv2.fillConvexPoly(image, poly, 0)

parser = argparse.ArgumentParser(description='Render a 2D projection of the layout on top of the custom aruco board')
parser.add_argument('layout_filename', metavar='layout', type=str, help='Path to the layout file')
parser.add_argument('data_dir', metavar='datadir', type=str, help='Path to the repo root directory')
parser.add_argument('-c', '--cross', action='store_false', help='Exclude the central marker row and column in the board')

args = parser.parse_args()

#   Check for path existence

scene_filename = args.layout_filename
if not os.path.isfile(scene_filename):
    raise FileNotFoundError(scene_filename)

data_dir = args.data_dir
if not os.path.isdir(data_dir):
    raise NotADirectoryError(data_dir)

canvas_size_x = 594 # mm
canvas_size_y = 420 # mm

density = 10 #dots/mm

resolution_img_x = canvas_size_x*density
resolution_img_y = canvas_size_y*density

image = np.ones((resolution_img_y, resolution_img_x), np.uint8)*255

#   Draw the aruco board
#   the bottom right of the aruco board will have to be placed on marker_root_origin
#   markers have 3 units for edge, 1 unit for spacing

marker_unit_size = 190
marker_size = marker_unit_size * 3
marker_spacing = marker_unit_size
marker_count_x = 7
marker_count_y = 5

size_aruco_x = marker_count_x * marker_size + (marker_count_x-1) * marker_spacing
size_aruco_y = marker_count_y * marker_size + (marker_count_y-1) * marker_spacing

dictionary = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_100)
aruco_board = cv2.aruco.GridBoard_create(marker_count_x, marker_count_y, marker_size, marker_spacing, dictionary)

aruco_img = aruco_board.draw((size_aruco_x, size_aruco_y), 0)

#   remove the central row and col of aruco markers

if not args.cross:
    aruco_img[:, 3*marker_size + 3*marker_spacing: 4*marker_size + 3*marker_spacing] = 255
    aruco_img[2*marker_size + 2*marker_spacing: 3*marker_size + 2*marker_spacing , :] = 255

#   paste the aruco image as a background

border_x = np.int32((resolution_img_x - size_aruco_x) / 2)
border_y = np.int32((resolution_img_y - size_aruco_y) / 2)

image[border_y : border_y + size_aruco_y , border_x : border_x + size_aruco_x] = aruco_img

#   origin of ref frame is at the bottom right of the aruco board

marker_root_origin_x = (border_x + size_aruco_x) / density # mm
marker_root_origin_y = (border_y + size_aruco_y) / density # mm

#   parse the xml file

tree = et.parse(scene_filename)
root = tree.getroot()

#   list the manipulation objects
print('ManipulationObjects in scene:')
for manip_object in root.findall('ManipulationObject'):
    print (manip_object.attrib['name'])

    #   parse .stl filename

    filename_element = manip_object.find('File')
    mesh_filename = os.path.join(data_dir, filename_element.text)
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

    print(mesh_filename)
    print(transform)


    transform = np.dot(matfix, transform)

    projectMeshOnImage(image, mesh_filename, transform, density)

layout_base_name = os.path.split(os.path.splitext(scene_filename)[0])[1]
printout_base_dir = os.path.join(data_dir, 'data', 'scenes', 'grasping', 'printable_layouts', layout_base_name)
base_output_filename = os.path.join(printout_base_dir, layout_base_name)

if not os.path.isdir(printout_base_dir):
    print(f"Creating output directory {printout_base_dir}")
    os.makedirs(printout_base_dir, exist_ok=True)

if not args.cross:
    base_output_filename+='_no_central'

cv2.imwrite(base_output_filename + '_printout.png', image)

# divide it in 4 a4 pages

cv2.imwrite(base_output_filename + '_printout_page_1.png', image[0:2100, 0:2970])
cv2.imwrite(base_output_filename + '_printout_page_2.png', image[2100:4200, 0:2970])
cv2.imwrite(base_output_filename + '_printout_page_3.png', image[0:2100, 2970:5940])
cv2.imwrite(base_output_filename + '_printout_page_4.png', image[2100:4200, 2970:5940])

print(f"Written output in {printout_base_dir}")



