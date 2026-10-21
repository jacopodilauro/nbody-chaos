# N body Simulation 

Gravitational N-body simulation in C++ with [raylib](#credits), from the three-body problem to trying to collide galaxies.  See [how it works](#how-it-works).

<p align="center">
  <img src="media/galaxy2D_3MB.gif" alt="gif galaxy" width="400"/>
</p>

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
# or classic c
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

| Code | Example |
|:-|:-:|
|```./nbody 3000```|<img src="media/nbody3000.png" width=400px>|
|```./nbody -GlxCl -blend --seed 34 -fps 2000```|<img src="media/img2D.png" width=400px>|
|```make -C build run ARGS="-blend 10"```|<img src="media/img10.png" width=400px>|
|```./nbody 1000 -GlxCl -blend -3D --g2tilt 90```|<img src="media/img3d-n.jpeg" width=400px>|
|```make -C build galaxy1```|<img src="media/cmakeG1.png" width="400">|
|```...```||

## How it works

Each body feels the gravitational pull of every other body, with a softening term $\varepsilon$ to avoid infinite forces when two bodies get too close

Decreasing $\varepsilon$ makes gravitational force much stronger in close distance, this leads to more extreme encounters and chaotic trajectories.

$$
\mathbf{a}_i = \sum_{j \neq i} \frac{G \, m_j \, (\mathbf{r}_j - \mathbf{r}_i)}{\left(|\mathbf{r}_j - \mathbf{r}_i|^2 + \varepsilon^2\right)^{3/2}}
$$

$G$ and $\varepsilon$ are in simulation units, not real-world values.


Velocities and positions are then updated with semi-implicit Euler integration:

$$
\mathbf{v}(t+ \Delta t)_i = \mathbf{v}(t)_i + \mathbf{a}(t)_i \, \Delta t \qquad | \qquad \mathbf{p}(t+ \Delta t)_i = \mathbf{p}(t)_i + \mathbf{v}(t)_i \, \Delta t
$$


The simulation supports two solver algorithm: the exact ***Direct Particle-Particle*** method with $\mathcal{O}(N^2)$ complexity, and the ***Barnes-Hut octree*** approximation, which groups distant bodies by their center of mass to reduce complexity to $\mathcal{O}(N \log N)$.

## Credits

Built with [raylib](https://www.raylib.com/) by [@raysan5](https://github.com/raysan5) and contributors,
licensed under the [zlib/libpng license](https://github.com/raysan5/raylib/blob/master/LICENSE).

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

