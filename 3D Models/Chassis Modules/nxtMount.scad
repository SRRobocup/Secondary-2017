include <MakeHoles.scad>
lH = -3.9;
lW = -3.65;  

nxtMount_sideLength = 107 - 32 +4;


//*********************************************//
//IMPORTANT//
sensorHoleSize = 2.3;
$fn = 25;
//IF PRINTING $fn = 25;//
//IF ETHAN, sensorHoleSize = 2.3 + 0.1;//
//ELSE, sensorHoleSize = 2.3;//
//*********************************************//
module nxtMount(rc)
{
$fn=rc;

translate ([0, 0, 0]) {
   difference() {
        translate([0,0,0]) {
            cube([10,nxtMount_sideLength,7.799], center = true);
        }

        translate ([-lW,-24,lH]) {
            rotate([0,0,90]){
                drawBeam(7);
            }
        }
        //1.8 cm between sensorHoleSize = diameter
        translate ([-1.8, 0, 0]) {
            rotate([90,0,0]){
                cylinder(110,d = sensorHoleSize,center = true);
            }
        }
        translate ([1.8, 0, 0]) {
            rotate([90,0,0]){
                cylinder(110,d = sensorHoleSize,center = true);
            }
        }
    }   
}
}
//nxtMount(50);
