include <MakeHoles.scad>
lH = -3.9;
lW = -3.65; 
$fn = 4;
move = -60;
sliceSize = 10;
difference()
{
    union()
    {
        translate([5, 0, 3])
        {
            cube([150, 7.7999, 90], center = true);
        }
    }
    translate([move - 1.8, 0, 20])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8, 0, 20])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 12*8, 0, 20])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 12*8, 0, 20])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8, 0, 30])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8, 0, 30])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 12*8, 0, 30])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 12*8, 0, 30])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8, 0, 40])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8, 0, 40])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 12*8, 0, 40])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 12*8, 0, 40])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 16*8, 0, -22])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 16*8, 0, -22])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 16*8, 0, -32])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 16*8, 0, -32])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 16*8, 0, -37])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 16*8, 0, -37])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 16*8, 0, 20])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 16*8, 0, 20])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 16*8, 0, 30])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 16*8, 0, 30])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 16*8, 0, 40])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 16*8, 0, 40])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 6*8, 0, 20])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 6*8, 0, 20])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 6*8, 0, 30])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 6*8, 0, 30])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move - 1.8 + 6*8, 0, 40])
    {
        rotate([90,0,0])
        {   
            cylinder(d = 2.3,h = 10, center = true);
        }
    }
    translate([move + 1.8 + 6*8, 0, 40])
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
        translate ([-85 + 8*9,-lH,0]) {
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
    translate([-75 + 5,0,-45 + 3])
    {
        rotate([0,45,0])
        {
            cube([sliceSize, 10, sliceSize], center = true);
        }
    }
    translate([75 + 5,0,-45 + 3])
    {
        rotate([0,45,0])
        {
            cube([sliceSize, 10, sliceSize], center = true);
        }
    }
    translate([-75 + 5,0,45 + 3])
    {
        rotate([0,45,0])
        {
            cube([sliceSize, 10, sliceSize], center = true);
        }
    }
    translate([75 + 5,0,45 + 3])
    {
        rotate([0,45,0])
        {
            cube([sliceSize, 10, sliceSize], center = true);
        }
    }
    /*
    translate([30, 0, -5])
    {
        cube([60,10, 30], center = true);
    }
    translate([-30, 0, -5 + 7.5])
    {
        cube([60,10, 15], center = true);
    }
    translate([0, 0,  0])
    {
        cube([15,10, 30], center = true);
    }
      */  
}
