include <MakeHoles.scad>
lH = -3.9;
lW = -3.65; 
$fn = 15;
move = -60;
difference()
{
    union()
    {
        translate([0, 0, -17])
        {
            cube([140, 7.7999, 60], center = true);
        }
    }
    translate([-60 - 1.8, 0, 0])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8, 0, 0])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 13*8, 0, 0])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 13*8, 0, 0])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 15*8, 0, -32])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 15*8, 0, -32])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([25, 0, -33])
    {
    translate ([-85,-lH,0]) {
            rotate([90,0,0]){
                drawBeam(7);
            }
        }
        translate ([-85,-lH,16]) {
            rotate([90,0,0]){
                drawBeam(7);
            }
        }
        translate ([-85 + 40,-lH,-8]) {
            rotate([90,0,0]){
                drawBeam(7);
            }
        }
    }
}
