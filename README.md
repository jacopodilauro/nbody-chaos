# N body Simulation 

Gravitational N-body simulation in C++ with [raylib](#credits), from the three-body problem to trying to collide galaxies.  See [how it works](#how-it-works).

![Gif galaxy](media/Galaxy2D_3MB.gif)

## Build

```shell
# cmake
cmake -B build
cmake --build build

# or if you don't like cmake (linux only) 
g++ main.cpp -O2 -o nbody -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```
## Usage

```shell
# running cmake
make -C build run ARGS="[OPTIONS]"
# or classic c++
./nbody [OPTIONS] #for Win nbody.exe [OPTIONS]

# demo 1
make -C build galaxy1
# demo 2
make -C build galaxy2

```
## Options

| Flag | Description |
|:-|:-:|
| `<N>` | set number of bodies (default 3) |
| `-3D` | start in 3D view |
| `-GlxCl` | Enable galaxy collision mode |
| `-fps` | Show FPS |
| `-blend` | Enable blending mode |
|`--g1tilt <DEG>`| Tilt angle for the first galaxy (in degrees) |
|`--g2tilt <DEG>`| Tilt angle for the second galaxy (in degrees) |
| `--seed <N>` | random seed (default 4) |
| `-h, --help` | show help option |

Press `Space` to switch between 2D and 3D.

## Examples

```shell
./nbody 4000 #1
./nbody -GlxCl -blend --seed 34 -fps 2000 #2
./nbody 1000 -GlxCl -blend -3D --g2tilt 90 #3
make -C build galaxy1 #4
```

## How it works

Each body feels the gravitational pull of every other body, with a softening term $\varepsilon$ to avoid infinite forces when two bodies get too close:

$$
\mathbf{a}_i = \sum_{j \neq i} \frac{G \, m_j \, (\mathbf{r}_j - \mathbf{r}_i)}{\left(|\mathbf{r}_j - \mathbf{r}_i|^2 + \varepsilon^2\right)^{3/2}}
$$

Velocities and positions are then updated with semi-implicit Euler integration:

$$
\mathbf{v}_i \leftarrow \mathbf{v}_i + \mathbf{a}_i \, \Delta t, \qquad \mathbf{r}_i \leftarrow \mathbf{r}_i + \mathbf{v}_i \, \Delta t
$$

$G$ and $\varepsilon$ are in simulation units (`G = 500`, `eps = 20`), not real-world values. In galaxy mode stars are massless test particles: only the two central black holes attract.

## Credits

Built with [raylib](https://www.raylib.com/) by [@raysan5](https://github.com/raysan5) and contributors,
licensed under the [zlib/libpng license](https://github.com/raysan5/raylib/blob/master/LICENSE).

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

