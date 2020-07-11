# Scene used for testing meshes

Camera
eye=(500,0,-100)
look_point=(0,0,0)
up=(0,0,-1)
view_plane_distance=100
zoom=0.25

DirectionalLight
direction=(-1,0,0)
color=(1,1,1)

DirectionalLight
direction=(0,0,1)
color=(0.5,0.5,0.5)

ColorTexture
name=green
color=(0,1,0)

MatteMaterial
name=mat_green
k=1
text=green

Settings
subdivs=10

Piramid
vert1=(200,0,0)
vert2=(0,100,0)
vert3=(0,-100,0)
vert4=(0,0,300)
mat=mat_green

Mesh
mat=mat_green
use_normals=false
file=C:\code\Gtrace\Gtrace\scenes\bunny.obj