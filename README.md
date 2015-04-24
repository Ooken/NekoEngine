# NekoEngine
Parallel RayTracer/PathTracer

You are free to use my Makefile as long as you put a reference back to me :3
How To;
Place the Makefile in your dir (it's the master-file)
if you want to create sub-folders, create them and place the makefile there too, don't use spaces from your root folder on
Also Place the Makefile_sub every too (this will be the template for new projects)

If you want to start a project type "make create [PROJECTNAME]" and [PROJECTNAME] will become a directory with a Makefile in it and from there on place your .cpp and .cu and .h files and whatever files you need, they can even be in subfolders, but there is one big rule: NO SPACES IN FILENAMES/DIR-NAMES!!!
if you want to compile just one project go there with cd and type "make"
if you want to cumpile all of them go to your folder withthe "master-Makefile" and type make there, it will build all projects :3

Greetings, neko

Done:
parallel BVH_Tree (CPU)
parallel Bounding Box calculation

Working on:
parallel BVH_Tree (CUDA)

Todo:
SDL2-Window/Render manager
