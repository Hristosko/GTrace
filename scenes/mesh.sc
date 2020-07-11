# Scene used for testing meshes

Camera
eye=(-500,0,0)
look_point=(0,0,0)
up=(0,1,0)
view_plane_distance=100
zoom=0.0025

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
subdivs=1

Mesh
mat=mat_green
use_normals=false
file=C:\code\Gtrace\Gtrace\scenes\bunny.obj