include <MakeHoles.scad>
lH = -3.9;
lW = -3.65;  
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
$fn = 25;
function holePlace(between,holeSize) = between/2 + holeSize/2;
function determineBetween(size,between) = (size - between)/2;

//nxtMount
translate ([0, 0, 0]) {
   difference() {
        translate([0,0,0]) {
            cube([10,105,7.799], center = true);
        }
        /*
        translate ([0, -24, 0]) {
            cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
        }
        translate ([0, -16, 0]) {
            cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
        }
        translate ([0, -8, 0]) {
            cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
        }
        translate ([0, 8, 0]) {
            cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
        }
        translate ([0, 16, 0]) {
            cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
        }
        translate ([0, 24, 0]) {
            cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
        }
                */
        translate ([-lW,-24,lH]) {
            rotate([0,0,90]){
                drawBeam(7);
            }
        }
        //1.8 cm between 2.3 = diameter
        translate ([-1.8, 0, 0]) {
            rotate([90,0,0]){
                cylinder(105,d = 2.3,center = true);
            }
        }
        translate ([1.8, 0, 0]) {
            rotate([90,0,0]){
                cylinder(105,d = 2.3,center = true);
            }
        }
    }   
}