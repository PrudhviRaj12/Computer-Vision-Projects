#include "CImg.h"
#include <cstdlib>

#include <sstream>

#include <iostream>

#include <vector>

#include <fstream>
#include <string>


using namespace std;

//TO get oveerfeat binary

const string batch_overfeat = "./overfeat/bin/linux_64/batch_overfeat ";
const string overfeat = "./overfeat/bin/linux_64/overfeat ";

class NeuralNetwork : public Classifier {
public:
  NeuralNetwork(const vector<string> &_class_list) : Classifier(_class_list) {}
  

  virtual string classify(const string &filename, string algo) {
    
    ofstream fout("test_n_tmp.data");
    string f = filename;
    replace(f.begin(),f.end(),'/','_');
    f = "./n/"+f;
    get_file(filename).save(f.c_str());
    int i = get_index(filename,class_list);
    


    string out = run_system((overfeat+" -f " + f).c_str());
    //    remove("overtmp.jpg");
    build_svm_n(out,fout,i+1);
    fout.close();
    system("./svm_multiclass_classify test_n_tmp.data svm_neural_train classify.tmp");
    ifstream cl("classify.tmp");
    string line;
    getline(cl,line);
    

    int num ;
    stringstream tmp;
    tmp << line;
    tmp >> num;
    cl.close();
    remove ("test_n_tmp.data");
    remove("classify.tmp");
    return class_list[num-1];
  }


  // RUnning general SVM train using build_svm_n
  void build_svm_n(string out,ofstream &fout,int cVal=1) {
    stringstream tmp;
    tmp<< out ;
    int a,b,c;
    tmp >> a; tmp>> b; tmp >> c;
    int i =  1;
    fout << cVal << " ";
    while(a>=0) {
      double t;
      tmp >> t;
      fout << i << ":"<< t << " ";
      a--;
      i++;
    }
    fout << endl;
  }
  int get_index(string f,vector<string> class_list) {
    for (int i = 0; i < class_list.size();i++) {
      size_t found = f.find(class_list[i]);
      if (found!=std::string::npos)
  return i;
    }
    return 0;
  }


  virtual void load_model() {}

  virtual void train(const Dataset &filenames, string algo)
  {
    int cVal =1;

    ofstream fout("train_n_svm.data");
    for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter) {
      int k = 0;
      for(int i=0; i<c_iter->second.size(); ++i) {
      	string fname = c_iter->second[i];

	string f = fname;

	replace(f.begin(),f.end(),'/','_');
	
  f = "./n/"+f;
      	get_file(fname).save(f.c_str());
	
  cout << " Current file = " << f << endl;
      	string out = run_system(overfeat+" -f "+f);
      	
	
  build_svm_n(out, fout, 1);
  
    }
      cVal++;
    }
    fout.close();
    
    system("./svm_multiclass_learn -c 1 train_n_svm.data svm_neural_train");
  }




protected:
  // extract features , resamples
  // vectorizing image data
  string run_system(string cmd) {
    string f = " > test.txt";
    system((cmd +f).c_str());
    ifstream t("test.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    remove("test.txt");
    return buffer.str();
  }

  CImg<double> get_file(const string &filename) {
    CImg<double> img(filename.c_str());
    return img.resize(size,size);
  }

  CImg<double> extract_features(const string &filename)
  {
    // In grayscale
    CImg<double> img(filename.c_str());
    // img._spectrum = 1;
    return img.resize(size,size).unroll('x');
  }
  //subsampling images and defingin training models
  static const int size=231;  
  map<string, CImg<double> > models; 
};
