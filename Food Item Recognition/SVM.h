/*
 File Creation : https://gist.github.com/maemre/63a944ea86905d2333c4
 */

#include <iostream>
#include <fstream>

class SVM : public Classifier
{
public:
  SVM(const vector<string> &_class_list) : Classifier(_class_list) {}
  
  // Nearest neighbor training. All this does is read in all the images, resize
  // them to a common size, convert to greyscale, and dump them as vectors to a file
  
  virtual void train(const Dataset &filenames, string algo)
  {
      format(filenames, "svm_train_data");
      system("./svm_multiclass_learn -c 1.0 svm_train_data train_svm_model");
  }
  
  virtual void test(const Dataset &filenames, string algo)
  {
	cout << "test " << endl;
	format(filenames, "svm_test_data");
  	system("./svm_multiclass_classify svm_test_data train_svm_model svm_predictions");
	system("python confusion.py svm_predictions");
  }
   
  virtual string classify(const string &filename, string algo)
  {
    /*
    CImg<double> test_image = extract_features(filename);
	      
    // figure nearest neighbor
    pair<string, double> best("", 10e100);
    double this_cost;
    for(int c=0; c<class_list.size(); c++)
      for(int row=0; row<models[ class_list[c] ].height(); row++)
	if((this_cost = (test_image - models[ class_list[c] ].get_row(row)).magnitude()) < best.second)
	  best = make_pair(class_list[c], this_cost);

    return best.first;
    */
  }
  
  virtual void load_model()
  {
    //for(int c=0; c < class_list.size(); c++)
    //models[class_list[c] ] = (CImg<double>(("nn_model." + class_list[c] + ".png").c_str()));
  }
  
protected:
  // extract features from an image, which in this case just involves resampling and 
  // rearranging into a vector of pixel data.
  CImg<double> extract_features(const string &filename)
    {

	CImg<double> current_image(filename.c_str());
	CImg<double> current_image_gs = current_image.get_RGBtoHSI().get_channel(2);
	return(current_image_gs).resize(size, size, 1, 1).unroll('x');
      //return (CImg<double>(filename.c_str())).resize(size,size,1,3).unroll('x');
    }
 
  void format(const Dataset &filenames, const char* filename)
  {
    fstream fs;
    fs.open(filename, ios::out);
    int class_number = 0;
    for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter)
      {
        class_number +=1 ;
        cout << "Processing " << c_iter->first << endl;
        CImg<double> class_vectors(size*size*3, filenames.size(), 1);
        cout << c_iter->second.size() << endl;
        for(int i=0; i<c_iter->second.size(); i++)
        {
            cout << c_iter->second[i].c_str() << endl;
            CImg<double> current_image = extract_features(c_iter->second[i].c_str());
            fs << class_number;
            for (int m = 0; m < current_image.size(); m++)
            {
                fs << ' ' << m+1 << ':' << current_image[m];

            }
            fs << '\n';
        }
      }
      fs.close();
    }


  static const int size= 10;  // subsampled image resolution
  map<string, CImg<double> > models; // trained models
};
