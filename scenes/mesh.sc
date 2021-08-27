# Scene used for testing meshes

PinholeCamera
eye=(-500,0,0)
look_point=(0,0,0)
up=(0,1,0)
view_plane_distance=100
zoom=0.0025

DirectionalLight
direction=(1,0,0)
color=(1,1,1)

@
DirectionalLight
direction=(0,0,1)
color=(1,1,1)

DirectionalLight
direction=(0,1,0)
color=(1,1,1)
@
ColorTexture
name=green
color=(0,1,0)

MatteMaterial
name=mat_green
k=1
text=green

Settings
max_subdivs=10
std_treshhold=0.01

Mesh
mat=mat_green
use_normals=true
#file=/mnt/c/code/GTrace/scenes/bunny.obj
file=C:\code\GTrace\scenes\bunny.obj
rotation=(0,-90,0)
