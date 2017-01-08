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
            cube([7,74,50], center = true);
            translate ([0,0,-25]) {
                rotate([90,0,0]) {
                    cylinder(74,r = 3.5,center = true);
                }
            }
        }
        translate ([0,0,-25]) {
            rotate([90,0,0]) {
                cylinder(74,r = 2.25,center = true);
            }
        }
        translate ([0,0,-17]) {
            rotate([90,0,0]) {
                cylinder(74,r = 2.25,center = true);
            }
        }
    }
}

//Draw a prism based on a 
//right angled triangle
//l - length of prism
//w - width of triangle
//h - height of triangle
module prism(l, w, h) {
       polyhedron(points=[
               [0,0,h],           // 0    front top corner
               [0,0,0],[w,0,0],   // 1, 2 front left & right bottom corners
               [0,l,h],           // 3    back top corner
               [0,l,0],[w,l,0]    // 4, 5 back left & right bottom corners
       ], faces=[ // points for all faces must be ordered clockwise when looking in
               [0,2,1],    // top face
               [3,4,5],    // base face
               [0,1,4,3],  // h face
               [1,2,5,4],  // w face
               [0,3,5,2],  // hypotenuse face
       ]);
}

translate([-58, -34,0]){prism(68, 110, 10.5);} //draw slant at angle of 5 deg