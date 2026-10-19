# N body Simulation 

Gravitational N-body simulation in C++ with raylib, from the three-body problem to (trying to) collide galaxies.

![Galaxy collision with 2000 entities](media/galaxy-collision.gif)

## Build

```shell
g++ main.cpp -O2 -o nbody -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```
## Usage

```shell
# on linux 
./nbody <number of bodies> [-3D] [-GlxCl] [-fps] [--seed N]
```

- `bodies`: number of bodies (default 3)
- `-3D`: start in 3D view
- `-GlxCl`: galaxy collision mode
- `-fps`: show the number of fps
- `--seed N`: random seed (default 4)
- `-h, --help`: show help option

Press `Space` to switch between 2D and 3D.

## Examples

```shell
./nbody 3
./nbody 2000 -GlxCl -3D
./nbody 3000 -GlxCl --seed 34
```

