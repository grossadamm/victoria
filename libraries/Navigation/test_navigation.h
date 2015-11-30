#include "Navigation.h"

test(Navigation_CRC8) 
{
  byte checksum = 240;
  Waypoint w = {123.0, 456.1, 20, 0};
  
  Navigation *nav = new Navigation(0,0,0);

  byte result = nav->CRC8((byte *) &w, sizeof(w));
  assertEqual(result, checksum);
}