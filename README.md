### How to build this project?

* requirement: ```allegro``` library at least 5

* build and run (assume your allegro library has been properly configured):
    ```
    chmod +x setup.sh
    ./setup.sh
    ```

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
        * ez: enemies are slower, hitboxes are bigger, costs are lower
        * hard: enemies moves faster, have higher HP, turret costs are more expensive.
        * no fail: the base is indestructable, you'll win no matter what
        * perfect: your base is on the brink of extinction, don't let your enemies enter it!
        * relax: infinite finance, no worrie about the budget
        * flashlight: you can't see anything until put some turret down, which provides visual area
    4. credit: osu! lazer, it provides beautiful icons and this idea.

* The play scene
    1. on the left panel has several turrets and tools, which provides various functionalities. right click to pick them if you have enough money
    2. use ```M``` key to mute background music if you find it annoying
    3. use ```P``` to pause the game
    4. use ```<Tab>``` to enable debug mode and the game would show hiboxes on every entity
    5. use ```q, w, e``` key to pick the turrets
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
