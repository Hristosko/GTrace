PinholeCamera
eye=(0,0,1000)
look_point=(0,0,0)
up=(0,1,0)
view_plane_distance=100
zoom=0.25

DirectionalLight
direction=(0, 0,-1)
color=(1,1,1)

ColorTexture
name=green
color=(0,1,0)

MatteMaterial
name=mat_green
k=1
text=green

Plane
position=(0,0,200)
x=1000
y=1000
mat=mat_green
rotation=(-45,0,0)