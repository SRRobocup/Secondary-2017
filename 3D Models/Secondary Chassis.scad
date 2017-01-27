$fn = 100;
include "Global.scad"
//middle width is 105 mm
//motor width is 40mm
//gap between motors is therefore 25mm
//nxt is 72 mm in width.
//nxt leaves 16.5 mm gap on either side.
//nxt holes start 2holes after that = 16mm
function holePlace(between,holeSize) = between/2 + holeSize/2;
function determineBetween(size,between) = (size - between)/2;

//Right Motor
translate ([0,-55, 0]) {
    difference() {
        translate([0,0,30]) {
            cube([190,5,105], center = true);
        }
        translate ([-90,0,4]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
        translate ([-90 + 16,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 8,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 24,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 55,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 63,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 71,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
    }
}

//Left Motor
translate ([0,55, 0]) {
    difference() {
        translate([0,0,30]) {
            cube([190,5,105], center = true);
        }
        translate ([-90,0,4]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
        translate ([-90 + 16,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 8,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 24,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 55,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 63,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
         translate ([-90 + 71,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true);
            }
        }
    }
}
//nxtMount
translate ([-85, 0, 50]) {
   difference() {
        translate([0,0,0]) {
            cube([20,105,5], center = true);
        }
        translate ([0, -24, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, -16, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, -8, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, 8, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, 16, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, 24, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        
    }
}
translate ([10, 0, 50]) {
   difference() {
        translate([0,0,0]) {
            cube([20,105,5], center = true);
        }
        translate ([0, -24, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, -16, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, -8, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, 8, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, 16, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        translate ([0, 24, 0]) {
            cylinder(20,d = legoHole,center = true);
        }
        
    }
}

//Light Sensor Mount
translate ([90,0,-12.5]) {
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
    }
}

//Light Array Mount
translate ([70,0,-12.5]) {
    difference() {
        union () {
            cube([10,110,5],center = true);
            translate([5,-17.5,-2.5]) {
                cube([10,35,10]);
            }
            translate([5,-5,0.5]) {
                cube([10,10,10]);
            }
            translate([-10,holePlace(arraySize,legoHole),0]) {
                cube([10,10,5], center = true);
            }
            translate([-10,-holePlace(arraySize,legoHole),0]) {
                cube([10,10,5], center = true);
            }
        }
        translate ([10,0,7]) {
            rotate([90,0,0]) {
                cylinder(130,d = legoHole, center = true);
            }
        }
        translate([-10,holePlace(arraySize,legoHole),0]) {
            cylinder(5,d = legoHole, center = true);
        }
        translate([-10,-holePlace(arraySize,legoHole),0]) {
            cylinder(5,d = legoHole, center = true);
        }
        cube([10,15,5],center = true);
    }
}

//Mount for arm
translate ([70,0,0]) {
    difference() {
        translate ([0,0,-2.5]) {
            cube([10,35,21],center = true);
        }
        translate ([0,0,-2.5]) {
            cube([10,15,21],center = true);
        }
        translate ([0,0,3]) {
            rotate([90,0,0]) {
                cylinder(130,d = legoHole, center = true);
            }
        }
    }
}

//arm for middle light sensor
translate ([70,0,3]) {
    difference () {
        union () {
            rotate([90,0,0]) {
                cylinder(15,r=5, center = true);
            }
            translate ([0,-5,-5]) {
                cube([15,10,10]);
            }
            translate ([10,-5,0]) {
                cube([27.5,10,5]);
            }
            translate([35,0,-5]) {
                cube([5,5,18],center = true);
            }
            translate([35,0,-15.5]) {
                cube([5,20,5],center = true);
            }
        }
        rotate ([90,0,0]) {
            cylinder(130,d = legoHole, center = true);
        }
        translate([10,0,0]) {
            rotate ([90,0,0]) {
                cylinder(10,d = legoHole, center = true);
            }
        }
        translate([35,holePlace(sensorDistanceBetween,sensorHoleSize),0]) {
            cylinder(130,d = sensorHoleSize,center = true);
        }
        translate([35,-holePlace(sensorDistanceBetween,sensorHoleSize),0]) {
            cylinder(130,d = sensorHoleSize,center = true);
        }
    }
}

