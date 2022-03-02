# Breakout
 
 Breakout game made in C++ using SDL2 and TinyXML.
 Levels are loaded from XML file in format : 
			
			<Level
			RowCount="30"
			ColumnCount="14"
			RowSpacing="20"
			ColumnSpacing="20"
			BackgroundTexture="Images/background.png"
			>

			<BrickTypes>
			<!-- Soft Brick -->
			<BrickType
			Id="S"
			Texture="Images/soft.png"
			HitPoints="1"
			HitSound="Sounds/soft.wav"
			BreakSound="Sounds/break.wav"
			BreakScore="50" />

			....
			</BrickTypes>

			<Bricks>
			_ _ _ _ _ _ _ _ _ _ _ _ _
			_ _ _ _ _ _ _ _ _ _ _ _ _
			_ _ _ _ _ _ _ _ _ _ _ _ _
			_ _ S S M M M M M S S _ _
			_ _ _ S S M M M S S _ _ _
			_ _ _ _ S S M S S _ _ _ _
			_ _ _ _ _ S S S _ _ _ _ _ 
			</Bricks>

			</Level>
 
Paddle bounce system is divided in four parts : https://atariage.com/2600/manuals/SuperBreakout/m_SuperBreakout_6.jpg
Bricks are expected to be the same size, although collisions will work for different brick sizes.
Ball is assumed to be real circle.