include <Global.scad>
$fn = 50;
module drawClaw(height) {
    difference()
    {
        cylinder(h = height,r = 60, center = true);
        translate([0,0,0])
        {
            cylinder(h = height,r = 52, center = true);
        }
        translate([1000,0,0]) 
        {
            cube([2000,2000,1000], center = true);
        }
        translate([-15,40,-1000]) 
        {
            cube([1000,4000,2000]);
        }
    }
}
translate ([-10,0,0])
    difference()  {
            union()
            {
                drawClaw(15);
                translate ([-20,52,0]) {
                    cylinder(h = 40, d = legoHole, center = true);
                }
                translate([-5,-75,-7.5]) {
                    cube([5,20,15]);
                }
                translate ([-10,-56,0]) {
                    cylinder(h = 40, d = legoHole, center = true);
                }
            }
    }

translate([10,0,0])
    mirror([1,0,0])
    {
        difference()  {
            union()
            {
                drawClaw(15);
                translate ([-20,52,0]) {
                    cylinder(h = 40, d = legoHole, center = true);
                }
                translate([-5,-75,-7.5]) {
                    cube([5,20,15]);
                }
                translate ([-10,-56,0]) {
                    cylinder(h = 40, d = legoHole, center = true);
                }
            }
        }
    }

//translate([-100,-2000,-1000]) 
//{
//    cube([100,4000,2000]);
//}