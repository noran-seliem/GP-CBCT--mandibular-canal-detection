#include "tags.h"


Tags::Tags() {}

Tags::~Tags() {}

void Tags::readFromTags(map<string, string> originalTags, tagsDictionary& dict) {

	dict.i_orientation = originalTags["0020|0037"];
	dict.i_position = originalTags["0020|0032"];
	dict.Window_Center = originalTags["0028|1050"];
	dict.Window_Width = originalTags["0028|1051"];
	dict.Rescale_Intercept = originalTags["0028|1052"];
	dict.Rescale_Slope = originalTags["0028|1053"];
	dict.Rescale_Type = originalTags["0028|1054"];
	dict.Slice_Thickness = originalTags["0018|0050"];
	dict.Pixel_Spacing = originalTags["0028|0030"];


	dict.Samples_per_Pixel = stoi(originalTags["0028|0002"]);
	dict.bits_Allocated = stoi(originalTags["0028|0100"]);
	dict.bits_Stored = stoi(originalTags["0028|0101"]);
	dict.High_bit = stoi(originalTags["0028|0102"]);
	dict.Pixel_Representation = stoi(originalTags["0028|0103"]);
}

int main() {
	return 0;
}