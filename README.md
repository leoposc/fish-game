# Fish Game 🐟
 
 Fish Game is a fast-paced 2D action-adventure where players take control of a daring fish jumping on plattforms filled with water, landscapes filled with obstacles. Dive deep into various aquatic environments and outsmart your friends! Swim swiftly through danger, pick your weapons and avoid the bullets from your opponents!

## Build

#### Dependencies

We use conan to install the Dependencies.
So first install conan then the buildilng commands should work successfully

The Dependencies are specified in the conanfile.txt file

#### Installing Conan

First install conan by using pip:

```
pip install conan
```

Then create a conan profile

```
conan profile new default --detect
```

Now you can install all Dependencies using conan:

```
conan install .
```


#### Building fish_game


```sh
mkdir build
cd build

cmake -DCMAKE_PREFIX_PATH=/usr/local/ ..
cmake --build .
```

