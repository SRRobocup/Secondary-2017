include <MakeHoles.scad>
lH = -3.9;
lW = -3.65; 
$fn = 25;
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
function holePlace(between,holeSize) = between/2 + holeSize/2;
function determineBetween(size,between) = (size - between)/2;
rotate([0,90,0]){
difference()
{ 
union()
{
    translate([0,0,0]) 
    {
        cube([5,7.5,18],center = true);
    }
    translate([0,0,-10.5]) 
    {
        cube([5,20,5],center = true);
    }
}
translate([0,holePlace(sensorDistanceBetween,sensorHoleSize),0]) {
            cylinder(30,d = sensorHoleSize,center = true);
        }
        translate([0,-holePlace(sensorDistanceBetween,sensorHoleSize),0]) {
            cylinder(30,d = sensorHoleSize,center = true);
        }
 
    translate([0,1.8,0]) {
            cylinder(30,d = sensorHoleSize,center = true);
        }
        translate([0,-1.8,0]) {
            cylinder(30,d = sensorHoleSize,center = true);
        }
    }
}