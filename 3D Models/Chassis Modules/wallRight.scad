include <MakeHoles.scad>
lH = -3.9;
lW = -3.65; 
$fn = 15;
renderComplexity = 10;
chassisLength = 190;
chassisWidth = 130;
sensorHoleSize = 2.68;
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

//Right Wall
translate ([0,-55, 0]) {
    difference() {
        translate([0,0,30]) {
            cube([190,7.799,105], center = true);
        }
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