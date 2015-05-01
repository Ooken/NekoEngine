# NekoEngine
Parallel RayTracer/PathTracer

Projects:
1. Tree			-Generating a hierarchie tree in a (pseudo)parallel way (function adaptable for CUDA)
2. BoundingBoxes	-Generate Bounding Boxes around tringles inside a Tree
3. SDL_Win		-Simple Window Handler for SDL2
4. RayTriangle		-Test if a Ray hits a Triangle

SideProjects:
1. Timelapse		-Record the screen slower than it will be saved to the Videofile (creates the Timelapse effect). Useful for Working on stuff-Videos

External Libs:
1. SDL_Lib		-SDL2 Binary data/libs/includes for Windows

Other Files:
README.md		-this readme File
Autocommit		-Autocommit for linux (just execute this file, enter a commit description and all changes will be uploaded)
Makefile		-General Makefile for building all sub-makefiles
Makefile_sub		-Preset for creating new Projects


You are free to use my Makefile as long as you put a reference back to me :3
How To;
Place the Makefile in your dir (it's the master-file)
if you want to create sub-folders, create them and place the makefile there too, don't use spaces from your root folder on
Also Place the Makefile_sub everywhere too (this will be the template for new projects)

If you want to start a project type "make create [PROJECTNAME]" and [PROJECTNAME] will become a directory with a Makefile in it and from there on place your .cpp and .cu and .h files and whatever files you need, they can even be in subfolders, but there is one big rule: NO SPACES IN FILENAMES/DIR-NAMES!!!
if you want to compile just one project go there with cd and type "make"
if you want to compile all of them go to your folder withthe "master-Makefile" and type make there, it will build all projects :3

Greetings, neko

Done:
parallel BVH_Tree (CPU)
parallel Bounding Box calculation

Working on:

Todo:
SDL2-Window/Render manager
Ray object
Ray-BoundingBox intersection
Ray-Sphere intersection
Ray-Triangle Intersection
parallel BVH_Tree (CUDA)
