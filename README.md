# Fish Game 🐟
 
 Fish Game is a fast-paced 2D action-adventure where players take control of a daring fish jumping on plattforms filled with water, landscapes filled with obstacles. Dive deep into various aquatic environments and outsmart your friends! Swim swiftly through danger, pick your weapons and avoid the bullets from your opponents!

## Build

#### Dependencies

We recommend to have the libraries installed inside your `/usr/local` directory. This directory is also default directory where cmake usually installs libaries and header files. The source code of all libaries can be found inside the `extern` directory. Installation of the SDL libaries was tested using the `configure` script. 

- SDL2
- SDL_image


#### Building fish_game

```sh
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/usr/local/ ..
make
```

## Networking Interface

#### data send to server

Player data:
- position
- Health/ alive
- weapon


#### server records

-statistics