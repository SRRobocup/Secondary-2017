//base of dump
//rotate([0,15,0]) {
translate ([-3,0,0]) {
    cube([110,74,5],center = true);
}

translate([-2.5,35.5,25]) {
    cube([110,3,55], center = true);
}

translate([-2.5,-35.5,25]) {
    cube([110,3,55], center = true);
}

//movable wall
translate([58,0,25]) {
    difference() {
        union() {
            cube([7,74,55], center = true);
            translate ([0,0,-27.5]) {
                rotate([90,0,0]) {
                    cylinder(74,r = 3.5,center = true);
                }
            }
        }
        translate ([0,0,-27]) {
            rotate([90,0,0]) {
                cylinder(74,r = 2.25,center = true);
            }
        }
        translate ([0,0,-20]) {
            rotate([90,0,0]) {
                cylinder(74,r = 2.25,center = true);
            }
        }
    }
}
//}