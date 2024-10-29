# Fish Game 🐟
 
 Fish Game is a fast-paced 2D action-adventure where players take control of a daring fish jumping on plattforms filled with water, landscapes filled with obstacles. Dive deep into various aquatic environments and outsmart your friends! Swim swiftly through danger, pick your weapons and avoid the bullets from your opponents!

## Build

#### Dependencies

We use conan to install the Dependencies.
So first install conan then the buildilng commands should work successfully

The Dependencies are specified in the conanfile.txt file

#### Installing Conan 

First install conan by using pipx:

```
pipx install conan
```

Then create a conan profile

```
conan profile detect --force
```

Now you can install all Dependencies using conan for Release and Debug configurations:

```
conan install . --build=missing
conan install . --build=missing -s build_type=Debug
```


#### Building fish_game using presets requirering cmake>=3.23


```
cmake --preset conan-debug
cmake -B build/Debug --preset conan-debug
./build/Debug/fish_game


cmake --preset conan-release
cmake -B build/Release --preset conan-release
./build/Release/fish_game
```

