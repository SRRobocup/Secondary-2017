include <MakeHoles.scad>
shankD1 = 2.1;
shankD2 = 2.3; // winner
shankD3 = 2.4;
lH = -3.9;
lW = -3.65; 
$fn = 25;
 difference() {
        cube([10, 10, 7.7999], center = true);
     /*
        translate ([-10,0,0]) {
            rotate ([90,0,0]) {
                cylinder(50,d =  shankD1,center = true,$fn=renderComplexity);
            }
        }
        translate ([1,0,0]) {
            rotate ([90,0,0]) {
                cylinder(50,d =  shankD2,center = true,$fn=renderComplexity);
            }
        }
        translate ([10,0,0]) {
            rotate ([90,0,0]) {
                cylinder(50,d =  shankD3,center = true,$fn=renderComplexity);
            }
        }
     */
        translate ([0,lW,lH]) {drawBeam(1);}
        translate ([-1.8, 0, 0]) {
            rotate([90,0,0]){
                cylinder(10,d = 2.3,center = true);
            }
        }
        translate ([1.8, 0, 0]) {
            rotate([90,0,0]){
                cylinder(10,d = 2.3,center = true);
            }
    }
}
