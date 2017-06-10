include <MakeHoles.scad>
lH = -3.9;
lW = -3.65; 
renderComplexity = 10;
chassisLength = 190;
chassisWidth = 130;
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

//*********************************************//
//IMPORTANT//
sensorHoleSize = 2.3 + 0.1;
$fn = 4;
//IF PRINTING $fn = 25;//
//IF ETHAN, sensorHoleSize = 2.3 + 0.1;//
//ELSE, sensorHoleSize = 2.3;//
//*********************************************//
module lightArm(rc)
{
$fn=rc;

translate ([0,0,3]) {
    difference () {
        union () {
            rotate([90,0,0]) {
                cylinder(15,r=5, center = true);
            }
            translate ([0,-5,-5]) {
                cube([15,10,10]);
            }
            translate ([10,-5,0]) {
                cube([35.5,10,5]);
            }
            /*
            translate([35,0,-5]) {
                cube([5,5,18],center = true);
            }
            translate([35,0,-15.5]) {
                cube([5,20,5],center = true);
            }
            */
        }
        //rotate ([90,0,0]) {
            //cylinder(130,d = legoHole, center = true,$fn=renderComplexity);
        //}
        translate ([0,-lH+(15-7.7799)/2,lW]) {
            rotate([90,0,0]){
                drawBeam(1);
            }
        }
        translate ([0,-lH-(15-7.7799)/2,lW]) {
            rotate([90,0,0]){
                drawBeam(1);
            }
        }
        
        translate([10,0,0]) {
            rotate ([90,0,0]) {
                cylinder(25,d = legoHole, center = true);
            }
        }
        
        translate([43,1.8,0]) {
            cylinder(130,d = sensorHoleSize,center = true);
        }
        translate([43,-1.8,0]) {
            cylinder(130,d = sensorHoleSize,center = true);
        }
        
    }
}
}

//lightArm(20);