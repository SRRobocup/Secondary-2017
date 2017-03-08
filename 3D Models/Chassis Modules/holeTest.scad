//*********************************************//
//IMPORTANT//
sensorHoleSize = 2.3 + 0.1;
$fn = 25;
//IF PRINTING $fn = 25;//
//IF ETHAN, sensorHoleSize = 2.3 + 0.1;//
//ELSE, sensorHoleSize = 2.3;//
//*********************************************//

difference()
{
    cube([10, 10, 10], center = true);
    translate ([-1.8, 0, 0]) 
    {
        rotate([90,0,0])
        {
            cylinder(10,d = sensorHoleSize,center = true);
        }
    }
    translate ([1.8, 0, 0]) 
    {
        rotate([90,0,0])
        {
            cylinder(10,d = sensorHoleSize,center = true);
        }
    }
}