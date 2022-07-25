#ifndef DICOMTAGS_H
#define DICOMTAGS_H
#include <map>

#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>

struct tagsDictionary {
    std::string i_orientation;                   //(0020,0037) decimal
    std::string i_position;                      //(0020,0032) decimal
    std::string Window_Center;               //0028,1050 decimal
    std::string Window_Width;                //0028,1051 decimal
    int Rescale_Intercept;                  //0028,1052 decimal
    int Rescale_Slope;                      //0028,1053 decimal
    std::string Rescale_Type;                //0028,1054 long
    std::string Slice_Thickness;             //0018, 0050 decimal
    std::string Pixel_Spacing;               //0028, 0030 decimal
    unsigned short int Samples_per_Pixel;  //(0028,0002) shall be 1
    unsigned short int bits_Allocated;     //(0028,0100) , 1 or 8
    unsigned short int bits_Stored;        //(0028,0101) , 1 or 8
    unsigned short int High_bit;           //(0028,0102) 1 or less than bit_stored
    unsigned short int Pixel_Representation; // (0028,0103)


};
class DicomTags{
    public:
        std::map<std::string,std::string>dicomTags;
        tagsDictionary dict;
    public:
        DicomTags();
        ~DicomTags();
        void loadInfo(std::string);
        void readFromTags(std::map<std::string, std::string> originalTags, tagsDictionary& dict);
        void Reset();
};

#endif
