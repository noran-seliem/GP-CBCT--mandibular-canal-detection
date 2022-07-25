#include<DicomTags.hpp>
DicomTags::DicomTags(){}
DicomTags::~DicomTags(){}
void DicomTags::loadInfo(std::string Directory){
    using PixelType = signed short;
    constexpr unsigned int Dimension = 3;

    using ImageType = itk::Image<PixelType, Dimension>;
    using ReaderType = itk::ImageSeriesReader<ImageType>;
    ReaderType::Pointer reader = ReaderType::New();
    using ImageIOType = itk::GDCMImageIO;
    ImageIOType::Pointer dicomIO = ImageIOType::New();
    reader->SetImageIO(dicomIO);

    using NamesGeneratorType = itk::GDCMSeriesFileNames;
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
    nameGenerator->SetInputDirectory(Directory);

    using FileNamesContainer = std::vector<std::string>;
    FileNamesContainer fileNames = nameGenerator->GetInputFileNames();

    reader->SetFileNames(fileNames);
    reader->Update();
    using DictionaryType = itk::MetaDataDictionary;
    const DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
    using MetaDataStringType = itk::MetaDataObject<std::string>;

    auto itr = dictionary.Begin();
    auto end = dictionary.End();

    while (itr != end)
    {
        itk::MetaDataObjectBase::Pointer entry = itr->second;
        MetaDataStringType::Pointer entryvalue = dynamic_cast<MetaDataStringType *>(entry.GetPointer());
        if (entryvalue)
        {
            std::string tagkey = itr->first;
            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
            this->dicomTags.insert(std::pair<std::string, std::string>(tagkey, tagvalue));
//            std::cout<<"key is "<<tagkey<<"  value is "<<tagvalue<<std::endl;
        }

        ++itr;
    }
    this->readFromTags(dicomTags,this->dict);
}

void DicomTags::readFromTags(std::map<std::string, std::string> originalTags, tagsDictionary& dict) {

    dict.i_orientation = originalTags["0020|0037"];
    dict.i_position = originalTags["0020|0032"];
    dict.Window_Center = originalTags["0028|1050"];
    dict.Window_Width = originalTags["0028|1051"];

    try{
        dict.Rescale_Intercept = stoi(originalTags["0028|1052"]);
    }
    catch(std::out_of_range& e){
        dict.Rescale_Intercept =0;
    }

    try{
        dict.Rescale_Slope = stoi(originalTags["0028|1053"]);
    }
    catch(std::out_of_range& e){
        dict.Rescale_Slope =1;
    }

    try{
        dict.Rescale_Type = originalTags.at("0028|1054");
    }
    catch(std::out_of_range& e){
        dict.Rescale_Type ="None";
    }

    dict.Slice_Thickness = originalTags["0018|0050"];
    dict.Pixel_Spacing = originalTags["0028|0030"];


    dict.Samples_per_Pixel = stoi(originalTags["0028|0002"]);
    dict.bits_Allocated = stoi(originalTags["0028|0100"]);
    dict.bits_Stored = stoi(originalTags["0028|0101"]);
    dict.High_bit = stoi(originalTags["0028|0102"]);
    dict.Pixel_Representation = stoi(originalTags["0028|0103"]);
}

void DicomTags::Reset(){
    if(!(this->dicomTags.empty())){
        this->dicomTags.clear();
    }
}
