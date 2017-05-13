/*
 File Creation : https://gist.github.com/maemre/63a944ea86905d2333c4
 */

#include <iostream>
#include <fstream>
#include <string>
class PCA : public Classifier
{
public:
  PCA(const vector<string> &_class_list) : Classifier(_class_list) {}
  
  // Nearest neighbor training. All this does is read in all the images, resize
  // them to a common size, convert to greyscale, and dump them as vectors to a file
  
  virtual void train(const Dataset &filenames, string algo)
  {
      fstream fs; 
      fs.open("pca_total_files_train", ios::out);
      for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter)
      {
          for (int i=0; i < c_iter->second.size(); i++)
	  {
	    cout << c_iter->second[i].c_str() << endl;
	    CImg<double> current_image = extract_features(c_iter->second[i].c_str());
	    cout << current_image.size() << endl;
            for (int m = 0; m < current_image.size(); m++)
            {
                //fs << ' ' << m+1 << ':' << current_image[m];
		fs << current_image[m] << ' ' ;
            }
            fs << '\n';

	  }	
      }
      fs.close();
 
      system("python pca_testing.py pca_total_files_train train 20");
      system("python svm_convert.py pca_20_dim_train train");
      /*
      std :: ifstream file("pca_50_dim");
      std :: string str;

      while(std :: getline(file, line))
      {
	std::stringstream linestream(line);
	std::string data;
	std:: getline(linestream, data, ' ');

	cout << linestream << ' ' ;
      }
      
      std :: string file_contents;
      while(std :: getline(file, str))
      {
	file_contents += str;
	cout<< file_contents.size() << endl;
      }*/
      
      //format(filenames, "pca_train_data.dat");
      system("./svm_multiclass_learn -c 1.0 pca_svm_train_format pca_model");
  }
  
  virtual void test(const Dataset &filenames, string algo)
  {
      fstream fs;
      fs.open("pca_total_files_test", ios::out);
      for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter)
      {
          for (int i=0; i < c_iter->second.size(); i++)
          {
            cout << c_iter->second[i].c_str() << endl;
            CImg<double> current_image = extract_features(c_iter->second[i].c_str());
            cout << current_image.size() << endl;
            for (int m = 0; m < current_image.size(); m++)
            {
                //fs << ' ' << m+1 << ':' << current_image[m];
                fs << current_image[m] << ' ' ;
            }
            fs << '\n';
          }
      }
      fs.close();
      system("python pca_testing.py pca_total_files_test test 20");
      system("python svm_convert.py pca_20_dim_test test");
	//cout << "test " << endl;
	//format(filenames, "svm_test_data.dat");
      system("./svm_multiclass_classify pca_svm_test_format pca_model pca_predictions");
      system("python confusion.py pca_predictions");
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
      return (CImg<double>(filename.c_str())).resize(size,size,1,3).unroll('x');
    }	
   
  void format(const Dataset &filenames, const char* filename){}
 /* {
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
    }*/


  static const int size=5;  // subsampled image resolution
  map<string, CImg<double> > models; // trained models
};
