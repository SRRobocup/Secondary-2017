include <Global.scad>
$fn = 200;

difference()
{
    cylinder(h = 10,r = 500, center = true);
    translate([0,60,0])
    {
        cylinder(h = 10,r = 400, center = true);
    }
}