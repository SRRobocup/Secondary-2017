/*      
        Customizable Straight LEGO Technic Beam
        Based on Parametric LEGO Technic Beam by "projunk"
        www.thingiverse.com/thing:203935
        Modified by Steve Medwin
        January 2015
*/

// Parameters modified for best print on my Replicator 2 with PLA
Pitch = 8*1.0;
Radius1 = 5.0/2;
Radius2 = 6.1/2;
Height = 7.8*1.0;
Depth = 0.85*1.0;
Width = 7.3*1.0; 

// user parameters
// number of holes =
//Holes = 9; // [2:20]

module drawBeam(x)
{
        union()
	{
            for (i = [1:x])
            {
                translate([(i-1)*Pitch, Width/2,0]) 
                {
                    cylinder(r=Radius2,h=Depth);
                    cylinder(r=Radius1,h=Height);
                    
                    translate([0,0,Height-Depth]) 
                    cylinder(r=Radius2,h=Depth);
                    
                    translate([0,0,Depth])
                    cylinder(h=(Radius2 - Radius1), r1=Radius2, r2=Radius1);
                }
            }
        }
        
}

