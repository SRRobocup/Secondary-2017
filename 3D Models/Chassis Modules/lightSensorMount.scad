//sensor hole = 17.5 difference
include <MakeHoles.scad>
lH = -3.9;
lW = -3.65;  
$fn = 15;
renderComplexity = 10;
renderComplexity = 10;
chassisLength = 190;
chassisWidth = 130;
sensorHoleSize = 2.3;
sensorDistanceBetween = 12.9;
sensorSize = 20.65;
legoHole = 4.8;
motorLength = 107.8;
arrayBetween = 7.215;
arraySize = 72.53;
arrayHoleToSide = 3.35;
portSize = 14.16;
motorHeight = 45.61;
motorWidth = 39.71;
motorTopToHolesRow = 18.12;
motorEndToHoles = 69.80;
motorEndToBottom = 22.25;
nxtLength = 111.54;
nxtWidth = 71.73;
nxtDepth = 39.88;
nxtDistToFirstHole = 10.97;
nxtLengthToFirstHole = 6.35;
//middle width is 105 mm
//motor width is 40mm
//gap between motors is therefore 25mm
//nxt is 72 mm in width.
//nxt leaves 16.5 mm gap on either side.
//nxt holes start 2holes after that = 16mm
function holePlace(between,holeSize) = between/2 + holeSize/2;
function determineBetween(size,between) = (size - between)/2;

//Light Array Mount
//x translated 70 
translate ([0,0,-12.5]) 
{
    difference() 
    {
        union () 
        {
            cube([10,110,5],center = true);
            translate([5,-17.5,-2.5]) 
            {
                cube([10,35,10]);
            }
            translate([5,-5,0.5]) 
            {
                cube([10,10,10]);
            }
            translate([-10,-2+holePlace(arraySize,legoHole        ),1.4]) 
            {
                cube([10,20,7.7999], center = true);
            }
            translate([-10,2-holePlace(arraySize,        legoHole),1.4]) 
            {
                cube([10,20,7.7999], center = true);
            }
            
        }
        
        translate ([10,0,7]) 
        {
            rotate([90,0,0]) 
            {
                cylinder(130,d = legoHole, center =             true);
            }
        }
        translate([-10,holePlace(arraySize,legoHole),0    ]) 
        {
            cylinder(5,d = legoHole, center = true);
        }
        translate([-10,-holePlace(arraySize,legoHole),0    ]) 
        {
            cylinder(5,d = legoHole, center = true);
        }
        
        translate ([1.8, 0, -12.5 + 12.5]) 
        {
            rotate([90,0,0])
            {
                cylinder(120,d = 2.3,center = true);
            }          
        }
        translate ([-1.8, 0, -12.5 + 12.5]) 
        {
            rotate([90,0,0])
            {
                cylinder(120,d = 2.3,center =            true);
            }
        }
        translate([-7,-40,lH + 1.4])
        {
            rotate([0,0,90])
            {
                drawBeam(11);
            }
        }
    }
}
//Mount for arm

translate ([0,0,0]) 
{
    difference() 
    {
       
        translate ([0,0,-2.5]) 
        {
            cube([10,35,21],center = true);
        }
        translate ([0,0,-2.5]) 
        {
            cube([10,15,21],center = true);
        }
        translate ([0,0,3]) 
        {
            rotate([90,0,0]) 
            {
                cylinder(130,d = legoHole, center =            true);
            }       
        }
        translate ([1.8, 0, -12.5]) 
        {
            rotate([90,0,0])
            {
                cylinder(120,d = 2.3,center = true);
            }          
        }
        translate ([-1.8, 0, -12.5]) 
        {
            rotate([90,0,0])
            {
                cylinder(120,d = 2.3,center =            true);
            }
        }
    }
}
translate ([20,0,-12.5]) {
    difference() {
        cube([10,110,5],center = true);
        for (i = [0:1])
            translate ([0,i*sensorSize+15,0]) {
                translate([0,holePlace(sensorDistanceBetween,sensorHoleSize),0]) {
                    cylinder(130,d = sensorHoleSize,center = true);
                }
                translate([0,-holePlace(sensorDistanceBetween,sensorHoleSize),0]) {
                    cylinder(130,d = sensorHoleSize,center = true);
                }
            }
       for (i = [0:-1:-1])
            translate ([0,i*sensorSize-15,0]) {
                translate([0,holePlace(sensorDistanceBetween,sensorHoleSize),0]) {
                    cylinder(130,d = sensorHoleSize,center = true);
                }
                translate([0,-holePlace(sensorDistanceBetween,sensorHoleSize),0]) {
                    cylinder(130,d = sensorHoleSize,center = true);
                }
            }
            translate ([1.8, 0, 0]) 
        {
            rotate([90,0,0])
            {
                cylinder(120,d = 2.3,center = true);
            }          
        }
        translate ([-1.8, 0, 0]) 
        {
            rotate([90,0,0])
            {
                cylinder(120,d = 2.3,center =            true);
            }
        }
    }
}