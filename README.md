# NEAT-Snake

Just a simple project to train up something that plays snake. Ended up not-so-simple...

# Usage

Clone the project:

```
git clone git@github.com:DamienHodsall/NEAT-Snake.git
```

Create and enter a build directory:

```
mkdir build
cd build
```

Generate and run build files:

```
cmake ..
make
```

Train a population (defined in src/main.cpp):

```
./NeatSnake
```

Watch it play the latest champion (on your connected Arduino Uno R4 Wifi programmed with the receiver code that isn't publicly available...):

```
./WatchGame latest.net
```
