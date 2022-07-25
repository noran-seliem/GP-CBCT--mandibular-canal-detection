
#ifndef TAGS_H
#define TAGS_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <variant>
using namespace std;

struct tagsDictionary {
    string i_orientation;                   //(0020,0037) decimal
    string i_position;                      //(0020,0032) decimal
    string Window_Center;               //0028,1050 decimal
    string Window_Width;                //0028,1051 decimal
    string Rescale_Intercept;           //0028,1052 decimal
    string Rescale_Slope;               //0028,1053 decimal
    string Rescale_Type;                //0028,1054 long
    string Slice_Thickness;             //0018, 0050 decimal
    string Pixel_Spacing;               //0028, 0030 decimal
    unsigned short int Samples_per_Pixel;  //(0028,0002) shall be 1
    unsigned short int bits_Allocated;     //(0028,0100) , 1 or 8
    unsigned short int bits_Stored;        //(0028,0101) , 1 or 8
    unsigned short int High_bit;           //(0028,0102) 1 or less than bit_stored
    unsigned short int Pixel_Representation; // (0028,0103)


};
class Tags {

public:
    Tags();
    ~Tags();
    void readFromTags(map<string, string> originalTags, tagsDictionary& dict);



};



#endif 