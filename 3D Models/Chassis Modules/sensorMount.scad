length = 17.78;
height = 12.7;
holeGap = 12.7;
borderT = 2.5;
buff = 0.25;

//*********************************************//
//IMPORTANT//
sensorHoleSize = 2.3 + 0.1;
$fn = 4;
//IF PRINTING $fn = 25;//
//IF ETHAN, sensorHoleSize = 2.3 + 0.1;//
//ELSE, sensorHoleSize = 2.3;//
//*********************************************//

difference()
{
    union()
    {
        cube([5, length + 2*borderT + 2*buff, height + 2*borderT + 2*buff], center = true);
        translate([5,(2*buff + length + borderT)/2,0])
        {
            cube([5, borderT, height + 2*borderT + 2*buff], center = true);
        }
        translate([5,-(2*buff + length + borderT)/2,0])
        {
            cube([5, borderT, height + 2*borderT + 2*buff], center = true);
        }
        translate([5,0,(2*buff + height + borderT)/2])
        {
            cube([5, length + 2*borderT + 2*buff, borderT], center = true);
        }
        translate([5,0,-(2*buff + height + borderT)/2])
        {
            cube([5, length + 2*borderT + 2*buff, borderT], center = true);
        }
    }
    translate([2.5,0,(2*buff + height + borderT)/2])
        {
            cube([5, length + 2*buff, borderT + 5], center = true);
        }
    translate([0, 1.8, 0])
    {
        rotate([90,0,90])
        {   
            cylinder(d = sensorHoleSize,h = 10, center = true);
        }
    }
    translate([0, -1.8, 0])
    {
        rotate([90,0,90])
        {   
            cylinder(d = sensorHoleSize,h = 10, center = true);
        }
    }
    translate([0, 1.8, -height/2 + 1.5])
    {
        rotate([90,0,90])
        {   
            cylinder(d = sensorHoleSize,h = 10, center = true);
        }
    }
    translate([0, -1.8, -height/2 + 1.5])
    {
        rotate([90,0,90])
        {   
            cylinder(d = sensorHoleSize,h = 10, center = true);
        }
    }
    translate([0, holeGap/2, -height/2 + 1.5])
    {
        rotate([90,0,90])
        {   
            cylinder(d = sensorHoleSize,h = 10, center = true);
        }
    }
    translate([0,-holeGap/2 , -height/2 + 1.5])
    {
        rotate([90,0,90])
        {   
            cylinder(d = sensorHoleSize,h = 10, center = true);
        }
    }
}