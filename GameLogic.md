# Fish Game - Game Logic

## Movement Physics

For the implementation the following order should be considered:

**1. Phase: Movement + physics**

- All entities (fishies, weapons and bullets) are affected by gravity -> GravityComponent: update()
- Fishies ignore gravity while in water -> MoveComponent: update()
- Fishies can move -> MoveComponent: "movements"()

**2. Phase: Collision detections**

- set gravity of all entities to 0 (when it is positive so swimming upwards still works) when a plattform is underneath -> Collision Logic &rarr; change velocity (or position) of transform
- check if the updated position would be causing a collision and stop them if it's the case (maybe handle x and y separately) -> Colligsion Logic &rarr; change velocity (or position) of tansform


**3. Phase: Actually update position**

<!-- - probably need to keep track of original position and the position for the next step or reverse the calculation if something collides  -->
- TransformComponent: update()