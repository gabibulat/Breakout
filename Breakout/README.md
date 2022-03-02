# BreakoutCpp
Small project in C++/ SDL2/ TinyXML

*SDL2 and TinyXMl installed via vcpkg

XMLfiles are containing Level files. 
Format of level files: 
      
      <Level
        RowCount=""                 // Number of bricks possible in a column 
        ColumnCount=""              // Number of bricks possible in a row 
        RowSpacing=""               // Space between brick rows      
        ColumnSpacing=""            // Space between brick columns
        BackgroundTexture=""        // Background texture in .png
        >
        
        <BrickTypes>
        <BrickType
        
        Id=""                 // One letter identifier e.g. "S" for soft brick
        Texture=""            // Brick texture in .png
        HitPoints=""          // Number of times it takes to hit a brick in order to break it
        HitSound=""           // Sound when hit
        BreakSound=""         // Sound when brick breaks
        BreakScore="" />      // Score gained when a brick breaks
       
       </BrickTypes>
        
        <Bricks>
    e.g. S S S S S S S S S S S S S S       //"S" means brick type ID. " " means column spacing and _ means no brick there.
         S S S S S _ _ _ _ S S S S S       // Level is loaded as drawn on here
         S S S S S _ _ _ _ S S S S S  	  
        </Bricks>

        </Level>
        
There's no UI as this is just a demo project, so after win or lose it just restarts.
Paddle bounce logic/ball directions of 4 equal parts is same as in breakout atari.
