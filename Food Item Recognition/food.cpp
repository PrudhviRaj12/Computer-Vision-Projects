// B657 assignment 3 skeleton code, D. Crandall
//
// Compile with: "make"
//
// This skeleton code implements nearest-neighbor classification
// using just matching on raw pixel values, but on subsampled "tiny images" of
// e.g. 20x20 pixels.
//
// It defines an abstract Classifier class, so that all you have to do
// :) to write a new algorithm is to derive a new class from
// Classifier containing your algorithm-specific code
// (i.e. load_model(), train(), and classify() methods) -- see
// NearestNeighbor.h for a prototype.  So in theory, you really
// shouldn't have to modify the code below or the code in Classifier.h
// at all, besides adding an #include and updating the "if" statement
// that checks "algo" below.
//
// See assignment handout for command line and project specifications.

/*
 References:
 1) http://stackoverflow.com/questions/19177456/include-c-header-files-in-opencv
 2) CV Undefined Reference : https://ubuntuforums.org/showthread.php?t=205188i9
 3) http://stackoverflow.com/questions/19177456/include-c-header-files-in-opencv
 4) http://answers.opencv.org/question/14611/example-of-makefile/
 5) http://stackoverflow.com/questions/2684603/how-do-i-initialize-a-float-to-its-max-min-value 
 6) http://docs.opencv.org/2.4/doc/tutorials/core/file_input_output_with_xml_yml/file_input_output_with_xml_yml.html
 7) http://docs.opencv.org/2.4/modules/core/doc/xml_yaml_persistence.html
 8) http://stackoverflow.com/questions/10321780/opencv-storagefile-error
 9) http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_ml/py_kmeans/py_kmeans_opencv/py_kmeans_opencv.html
 10) http://docs.opencv.org/2.4.8/modules/core/doc/clustering.html
 11) http://stackoverflow.com/questions/15782196/kmeans-for-image-clustering
 12) Confusion Matrix : https://github.iu.edu/cs-b551-fa2016/hdchapan-mthatte-prudacha-a5
 
 
*/
#include "CImg.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <sys/types.h>
#include <dirent.h>
#include <map>
#include <numeric>
#include <opencv2/core/core.hpp>

//Use the cimg namespace to access the functions easily
using namespace cimg_library;
using namespace std;

// Dataset data structure, set up so that e.g. dataset["bagel"][3] is
// filename of 4th bagel image in the dataset
typedef map<string, vector<string> > Dataset;

#include <Classifier.h>
#include <NearestNeighbor.h>
#include <SVM.h>
#include <PCA.h>
#include <BagofWords.h>
#include <HaarClassifier.h>
#include <NeuralNetwork.h>
#include <SVM2.h>
// Figure out a list of files in a given directory.
//
vector<string> files_in_directory(const string &directory, bool prepend_directory = false)
{
  vector<string> file_list;
  DIR *dir = opendir(directory.c_str());
  if(!dir)
    throw std::string("Can't find directory " + directory);
  
  struct dirent *dirent;
  while ((dirent = readdir(dir))) 
    if(dirent->d_name[0] != '.')
      file_list.push_back((prepend_directory?(directory+"/"):"")+dirent->d_name);

  closedir(dir);
  return file_list;
}

int main(int argc, char **argv)
{
  try {
    if(argc < 3)
      throw string("Insufficent number of arguments");

    string mode = argv[1];
    string algo = argv[2];

    // Scan through the "train" or "test" directory (depending on the
    //  mode) and builds a data structure of the image filenames for each class.
    Dataset filenames; 
    vector<string> class_list = files_in_directory(mode);
    for(vector<string>::const_iterator c = class_list.begin(); c != class_list.end(); ++c)
      filenames[*c] = files_in_directory(mode + "/" + *c, true);

    // set up the classifier based on the requested algo
    Classifier *classifier=0;
    //if(algo == "nn")
    //  classifier = new NearestNeighbor(class_list);
     if(algo == "baseline")
      classifier = new SVM(class_list);
     else if (algo == "bow")
      classifier = new BagofWords(class_list);
     else if (algo == "eigen")
      classifier = new PCA(class_list);
    //else if (algo == "haar")
      //classifier = new HaarClassifier(class_list); 
     else if (algo == "deep")
      classifier = new NeuralNetwork(class_list);    
    else
      throw std::string("unknown classifier " + algo);

    // now train or test!
    if(mode == "train")
      classifier->train(filenames, algo);
    else if(mode == "test")
      classifier->test(filenames, algo);
    else
      throw std::string("unknown mode!");
  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}








