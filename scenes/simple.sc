Camera
eye=(0,0,500)
look_point=(0,0,0)
up=(0,1,0)
view_plane_distance=100
zoom=0.5

ColorTexture
name=green
color=(0,1,0)

ColorTexture
name=red
color=(1,0,0)

ColorTexture
name=blue
color=(0,0,1)

ColorTexture
name=cyan
color=(0,1,1)

MatteMaterial
name=mat_green
k=1
text=green

MatteMaterial
name=mat_red
k=1
text=red

MatteMaterial
name=mat_blue
k=1
text=blue

MatteMaterial
name=mat_cyan
k=1
text=cyan

Settings
subdivs=5

Triangle
a=(-500,-500,0)
b=(200,0,0)
c=(0,200,0)
mat=mat_red

Sphere
center=(0,0,200)
radius=100
mat=mat_cyan

Sphere
center=(400,400,-400)
radius=200
mat=mat_blue
