
PinholeCamera
eye=(0,0,500)
look_point=(0,0,0)
up=(0,1,0)
view_plane_distance=100
zoom=0.25

@
OrthogonalCamera
up=(0,1,0)
look_direction=(0,0,-1)
height=500
zoom=2
@

DirectionalLight
direction=(0, 0,-1)
color=(1,1,1)

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

ColorTexture
name=white
color=(1,1,1)

ColorTexture
name=black
color=(0,0,0)

NoiseTexture
name=noise
text1=white
text2=black

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
name=mat_noise
k=1
text=noise

Settings
subdivs=10

Triangle
a=(-500,-500,0)
b=(200,0,0)
c=(0,200,0)
mat=mat_red
position=(0,0,-300)
rotation=(180,0,0)

Triangle
a=(-500,-500,0)
b=(200,0,0)
c=(0,200,0)
mat=mat_blue
position=(0,0,-300)
rotation=(0,0,0)

Sphere
position=(0,0,200)
radius=100
mat=mat_noise

Sphere
position=(400,400,-400)
radius=200
mat=mat_blue
