renderComplexity = 10;
chassisLength = 190;
chassisWidth = 130;
sensorHoleSize = 2.68;
sensorDistanceBetween = 12.9;
sensorSize = 20.65;
legoHole = 4.75;
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

function holePlace(between,holeSize) = between/2 + holeSize/2;
function determineBetween(size,between) = (size - between)/2;

//Right Motor

translate ([0,-55, 0]) {
    difference() {
        translate([0,0,10]) {
            cube([190,5,50], center = true);
        }
        translate ([-90,0,4]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
        translate ([-90 + 15,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 7.5,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 22.5,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 55,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 62.5,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 70,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
    }
}

//Left Motor
translate ([0,55, 0]) {
    difference() {
        translate([0,0,10]) {
            cube([190,5,50], center = true);
        }
        translate ([-90,0,4]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
        translate ([-90 + 15,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 7.5,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 22.5,0,4 + 15]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 55,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 62.5,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
        }
         translate ([-90 + 70,0,4 -7.5]) {
            rotate ([90,0,0]) {
                cylinder(20,d = legoHole,center = true,$fn=renderComplexity);
            }
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
                cylinder(130,d = legoHole, center = true,$fn=renderComplexity);
            }
        }
    }
}

//arm for middle light sensor
translate ([70,0,3]) {
    difference () {
        union () {
            rotate([90,0,0]) {
                cylinder(15,r=5, center = true,$fn=renderComplexity);
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
            cylinder(130,d = legoHole, center = true,$fn=renderComplexity);
        }
        translate([10,0,0]) {
            rotate ([90,0,0]) {
                cylinder(10,d = legoHole, center = true,$fn=renderComplexity);
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

