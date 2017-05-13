/*
 File Creation : https://gist.github.com/maemre/63a944ea86905d2333c4
 http://stackoverflow.com/questions/28114662/opencv-kmeans-n-k-exception-error-215
 http://docs.opencv.org/2.4/modules/core/doc/xml_yaml_persistence.html
 */

#include <iostream>
#include <fstream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

class BagofWords : public Classifier
{
public:
  BagofWords(const vector<string> &_class_list) : Classifier(_class_list) {}
  
  // Nearest neighbor training. All this does is read in all the images, resize
  // them to a common size, convert to greyscale, and dump them as vectors to a file
 
   int index_clusters(vector<float> descriptor)
  {
     int cluster = 0;
     float max_distance = FLT_MAX;
     for(int i = 0; i < means.rows; i++)
     {
        float current_distance = 0.0;
        for(int j =0; j <128; j++)
            current_distance += (means.at<float>(i, j) - descriptor[j]) * (means.at<float>(i, j) - descriptor[j]);
        if (current_distance < max_distance)
        {
            max_distance = current_distance;
            cluster = i;
        }
     }
     return cluster;
  }
 
  virtual void train(const Dataset &filenames, string algo)
  {
	std :: vector<std :: vector<float> > sift_descriptors;
	std :: vector<int> sift_counter;
	int counter = 0;
	
	for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter)
	{
	    for (int i = 0; i<c_iter->second.size(); i++)	
	    {	
		CImg<double> current_image(c_iter->second[i].c_str());	
		std :: vector<SiftDescriptor> current_descriptors = Sift :: compute_sift(current_image);
		
		
		int count_descriptors = 0;
		counter += 1;
		for (int j = 0; j < current_descriptors.size(); j++)
		{
		     sift_descriptors.push_back(current_descriptors[j].descriptor);
		     count_descriptors += 1;
		}
		
	    cout << i << ' ' << counter << ' ' << count_descriptors << endl;
	    sift_counter.push_back(count_descriptors);
	    }
	}
	cout << ' ' << sift_descriptors.size() << endl;
	int excluded = 2327128;
	cv :: Mat mat_descriptors((sift_descriptors.size() - excluded), 128, CV_32F);
	cv :: Mat targets;

	for(int i=0; i <sift_descriptors.size() - excluded; i++)
	{
	     for(int j=0; j <128; j++)
	     {
		cout << sift_descriptors.size() - excluded << ' '<< i << ' ' << j << ' ' << sift_descriptors[i][j] << endl;
		mat_descriptors.at<float>(i, j) = sift_descriptors[i][j];
		//descriptors[i][j] = sift_descriptors[i][i];
		//cout << descritpors[i][j] << endl;
	     }
	}
		
	int clusters = 500;
	
	cv ::kmeans(mat_descriptors, clusters, targets, cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 10,0.1), 1, cv::KMEANS_PP_CENTERS, means);

	
	cv :: FileStorage f("means.xml", cv ::FileStorage::WRITE);
	f << "means" << means;
	cout << targets.rows << ' ' << targets.cols << endl;

	//Histogram Counts
	
	Dataset :: const_iterator c_iter = filenames.begin();
        fstream fs;
	fs.open ("train_bow_svm", ios::out);
	int reset = 0, start =0, class_number = 1;

	for(int j = 0; j < sift_counter.size(); j++, reset++)
	{
	    vector<float> current_histogram;
	    for(int i = 0; i < clusters; i++)
	    {
		current_histogram.push_back(0);
	    }
	    if (reset == c_iter->second.size())
	    {
		class_number += 1;
		c_iter++;
		//Reset Counter after each class
		reset = 0;
	    }
	    fs << class_number;
	    for(int i = start;  i < start + sift_counter[j] && i < targets.rows; i++)
	    {
		if (targets.at<int>(i) < clusters)
		    current_histogram[targets.at<int>(i)] ++;
	    }
	    cout << j << ' ' << current_histogram.size() << endl;

	    for(int i =0; i<clusters; i++)
	    {
		float val = current_histogram[i]/sift_counter[j];
		fs << ' ' << i + 1 << ':' << val;
	    }
	    fs << '\n';
	    start += sift_counter[j];
	} 
	fs.close();
      //iformat(filenames, "svm_train_data.dat");
      system("./svm_multiclass_learn -c 1.0 train_bow_svm train_svm_model_bow");
  }
  
  virtual void test(const Dataset &filenames, string algo)
  {
	cv :: FileStorage f("means.xml", cv :: FileStorage :: READ);
    	f["means"] >> means;
	fstream fs;
	fs.open("test_bow_svm", ios::out);
	int k = 1;
	for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter, ++k)
	{
	    for(int i = 0; i < c_iter->second.size(); i++)
	    {
		cout << c_iter->second[i].c_str() << endl;
		//cout << "1" << endl;
	    	CImg<float> current_image(c_iter->second[i].c_str());
		//cout << "2" << endl;
	    	fs  << k;
		//cout << "3" << endl;
	    	vector<double> current_histogram;
	    	for(int j=0; j<means.rows; j++)
	    	{
		    //cout << i << ' ' << j << endl;
		    current_histogram.push_back(0);
	    	}	
		
		vector <SiftDescriptor> current_descriptor = Sift :: compute_sift(current_image);
		cout << i << ' ' << current_descriptor.size() << endl;
		for (int j = 0; j < current_descriptor.size(); j++)
		{
		    int cluster_index = index_clusters(current_descriptor[j].descriptor);
		    current_histogram[cluster_index] += 1;
		}
		
		for(int j = 0; j <current_histogram.size(); j++)
		{
		    float val = current_histogram[j]/current_descriptor.size();	
		    fs << ' ' << j+1 << ':' << val;
		}
		fs << '\n';
	    }	
	}
	fs.close();

	//cout << "test " << endl;
	//format(filenames, "svm_test_data.dat");
  	system("./svm_multiclass_classify test_bow_svm train_svm_model_bow bow_predictions");
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
    //cv :: FileStorage f("means.xml", cv :: FileStorage :: READ);
    //f["means"] >> means;
    //for(int c=0; c < class_list.size(); c++)
    //models[class_list[c] ] = (CImg<double>(("nn_model." + class_list[c] + ".png").c_str()));
  }
  
protected:
  // extract features from an image, which in this case just involves resampling and 
  // rearranging into a vector of pixel data.
  
  cv :: Mat means;
  CImg<double> extract_features(const string &filename)
    {
      return (CImg<double>(filename.c_str())).resize(size,size,1,3).unroll('x');
    }
 
  void format(const Dataset &filenames, const char* filename)
  {
    /*
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
      fs.close();*/
    };


  static const int size=60;  // subsampled image resolution
  map<string, CImg<double> > models; // trained models
};
