# Tower Defense Game

**I would likely just leave this project be, star this project, report issues or pull some requests and I'll probably restart the development**

## project overview

1. this project was written with **C++**, using the **allegro** library.
2. structure
    * ```src```: contains all source code file and header (note that header files are in the ```inc``` subdirectory)
    * ```resources```: contain the assets of the game
    * ```build```: it will be created after execute ```setup.sh```
       1. the executeable ```Tower_Defense``` will be here
       2. the ```resources``` subdirectory is the symlink to the ```resources``` directory at root
3. I did not created this project from scratch, as the repo name suggests, this is a homework. The game engine part and basic game machanism is predefined.
    * the empty template: [here](https://drive.google.com/drive/folders/1vfwRhzD4txgvNgLjqe2RoL5-qFSouses)
   
## How to build this project?

* requirement: ```allegro``` library at least version 5
  * [The official allegro repo](https://github.com/liballeg/allegro5)

* this project is created in Linux environment, to setup allegro, all you need to do is download it with package manager.
  1. **apt**: ```liballegro5-dev```
  2. **pacman**: ```allegro5```
  3. **dnf**: ```allegro5```

* build and run
    ```
    chmod +x setup.sh
    ./setup.sh
    ```
* the game should start running after the setup is finished, if not, run ```./build/Tower_Defense```

## Control

#### title, select and setting scene:
* The mouse is the only input that works
* click ```stage <number>``` to enter the game
* the setting value is not persistent (it sucks doesn't it?)

#### gameplay:
* the first scene you would see is mod selection
    1. click the mod icon to enable it, note that this you could select nothing and enter the game.
    2. some mod are not compatible to others
        * ez v.s hard
        * no fail v.s. perfect
    3. the mod effects
        * ez: enemies are slower, hitboxes are bigger, turret costs are lower
        * hard: enemies moves faster, have higher HP, turret costs are more expensive.
        * no fail: the base is indestructable, you'll win no matter what
        * perfect: your base is on the brink of extinction, don't let your enemies slip in!
        * relax: infinite finance, no worrie about the budget
        * flashlight: you can't see anything until put some turret down, which provides visual area
    4. credit: osu! lazer, it provides beautiful icons and this idea.

* The play scene
    1. on the left panel has several turrets and tools, which provides various functionalities. right click to pick them if you have enough money
    2. use ```M``` key to mute background music if you find it annoying
    3. use ```P``` to pause the game
    4. use ```<Tab>``` to enable debug mode and the game would show hiboxes on every entity
    5. use ```q, w, e``` key to pick up the turrets faster
    6. use ```r``` key to enable mover, ```<BackSpace>``` to enable shovel
    7. use ```1-9``` to change the tickspeed
    8. use ```<Esc>``` key to go back to title

* Win/lose scene
    1. click ```Back``` button to return to the select theme.

#### Configuration

* There are some thing you could customize:
    1. the two stages: enemy waves and map
    2. image and audio assets
    3. enemy attributes (require recompilation)

* The two stages
    1. all the level info are stored in ```resources```
    2. about ```map<number>.txt```:
       * The ```0``` is the road for enemy, which will be brown block in the game
       * The ```1``` is not available for enemy, which will be gray block in the game
       * The ```(0, 0)``` is the starting point for enemies, and our base is at bottom right
       * please leave at least one road for enemy, or the game won't work
    3. about ```enemy<number>.txt```:
       * the format is ```<enemy_type> <spawn internval> <count>```
       * type table:
         | number | enemy |
         |:------:|:-----:|
         | 1 | normal red enemy |
         | 2 | dice enemy |
         | 3 | dice duper |
         | 4 | obstacle spawner |
* Assets
  * well, this part you should figure it out by yourself, the naming scheme is messy.
  * original music:
    * [エレハモニカ remixed by kors k - Der Wald (kors k Remix)](https://www.youtube.com/watch?v=pvEBfEaBqNI)
    * [Perlence](https://youtu.be/uCZYK3jqNgU)
    * The information of other music was mentioned in the metadata of the files
    
* Attributes
  * take a look at ```src/inc/GenArg.h```, note that some attributes, such as defense and turret HP is not yet used.
    
