Welcome to my dungeon game.

how to play:
  - clone the repository
  - decide between make compile and make run
  - make run runs the program immediately, which starts the default level 1
  - make compile, then ./dungeon levelname, to start your desired level
  - i have some predefined levels available in the level folder
  - for example ./dungeon level/2.txt starts the second level
  - basic controlls W, A, S, D

how to create a level yourself:
  - create a normal .txt file
  - # are walls
      - it is important that there are no gaps in the created walls
      - to get a better feel for whats possible just take a look into the level folder
  - S is the player
      - only one player is allowed
  - A is the exit
      - you can place several exits
  - <, >, ^, v are the monsters and their movement direction
  
  which options are avaliable:
  - -i inputfilename
    - enables you to predefine a sequence of inputs W, A, S, D in an .txt file
  - -o outputfilename
    - enables you to choose an outputfile
  - for example ./dungeon -i inputs.txt -o output.txt level/3.txt  
  



