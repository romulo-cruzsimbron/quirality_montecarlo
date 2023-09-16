# DFS algoruthm and averaging the data
Here we should first copy all the previous lattice outputs inside of a new Imagenes files , then run the Python script that will list the lattice we will rerun. We rerun the lattices evolution in order to got the average of at least 1000 states in equilbrium.
First create the new directories
- Distribucion
- Quiralidad_exceso
- Quiralidad_green
- Quiralidad_yellow
Run the code:
python3 listing.py
g++ *.h *.cpp -lSDL2 -lSDL2_ttf -lSDL2_image -fopemp -o Run

And to run, just type:

./Run


