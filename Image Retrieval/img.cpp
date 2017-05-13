// B657 assignment 2 skeleton code
//
// Compile with: "make"
//
// See assignment handout for command line and project specifications.

/*
References:
1)Visualizing SIFT Matches : http://www.cs.ubc.ca/~lowe/keypoints/siftDemoV4.zip
2)Map Sorting :  http://stackoverflow.com/questions/19842035/stdmap-how-to-sort-by-value-then-by-key
3)Printing Map in Reverse Order : http://www.cplusplus.com/reference/map/map/rbegin/

Code Executables:

The Part 1 contains three parts, 

part1.1 which takes two images and returns SIFT visualization between  those images. 
part1.2 takes a query image and a set of test images and returns all the images in the non-increasing 
order of the matched SIFT features.
part1.3 is our final image retrival algorithm. It returns only the top 10 images with respect
to their matched features.

Executable:

part1.1

./a2 part1.1 part1_images/bigben_2.jpg part1_images/bigben_3.jpg

Returns three files, sift_12 - first image with its SIFT matches
sift_21 - second image with its SIFT matches
combined_test - the combined image with lines showcasing SIFT matches.

part1.2

./a2 part1.2 part1_images/bigben_2.jpg part1_images/ *

Returns a list of all the images along with the number of
SIFT matches in descending order.

part1.3
./a2 part1.3 part1_images/bigben_2.jpg part1_images/ * 

Returns the top 10 images which are the best matches for 
our query image.

Part 2 has two parts.

Executable:

part2.2

./a2 part2.2 part1_images/bigben_2.jpg part1_images/ *

Implements the quantized projection on the SIFT vectors and returns 
the top 10 images which are best matches for our query image.

*/

//Link to the header file
#include "CImg.h"
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <algorithm>
#include <map>

CImg<double> Image_Warp(CImg<double> input);

//Use the cimg namespace to access the functions easily
using namespace cimg_library;
using namespace std;


int main(int argc, char **argv)
{
	//cout << sizeof(argc);
	try {

		if(argc < 2)
		{
			cout << "Insufficent number of arguments; correct usage:" << endl;
			cout << "    a2 part_id ..." << endl;
			return -1;
		}

		string part = argv[1];
		string inputFile_1 = argv[2];
		string inputFile_2 = argv[3];
		if (part == "part1.1")
		{
		    std :: map <int, string> order;
                    //string inputFile_1 = argv[2];
                    CImg<double> input_image_1(inputFile_1.c_str());
                    CImg<double> gray_1 = input_image_1.get_RGBtoHSI().get_channel(2);
                    	vector<SiftDescriptor> descriptors_1 = Sift :: compute_sift(gray_1);

		    CImg<double> input_image_2(inputFile_2.c_str());
                    CImg<double> gray_2 = input_image_2.get_RGBtoHSI().get_channel(2);
                        vector<SiftDescriptor> descriptors_2 = Sift :: compute_sift(gray_2);
		
		    int test[descriptors_2.size()][128];

                                for (int i = 0; i < descriptors_2.size(); i++)
                                {
                                        for(int j =0; j < 128; j++)
                                        {
                                                test[i][j] = descriptors_2[i].descriptor[j];
                                        }
                                }

                                std :: vector <int> points_1;
                                std :: vector <int> points_2;

                                int count = 0;
                                for (int c = 0; c < descriptors_1.size(); c++)
                                {
                                        int example[128];
                                        for (int d = 0; d < 128; d++)
                                        {
                                                example[d] = descriptors_1[c].descriptor[d];
                                        }
                                        int distance[descriptors_2.size()];

                                        for (int i =0; i < descriptors_2.size(); i++)
                                        {
                                                int sum = 0;
                                                for (int j=0; j < 128; j ++)
                                                {
                                                        int square = 0;
                                                        square = (example[j] - test[i][j]) * (example[j] - test[i][j]);
                                                        sum += square;
                                                }
                                                distance[i] = sum;
                                        }

                                        std::map<int, int> distances;
				
					for (int i = 0; i < descriptors_2.size(); i++)
                                                distances[distance[i]] = i;

                                        sort(distance, distance + descriptors_2.size());
						
					double chooser = 0.0;
                                        chooser = double(distance[1]) / double(distance[0]);
					if (chooser > 1.5) 
                                        {
                                                count += 1;
                                                int sift_match = distances[distance[0]];
						points_1.push_back(c);
						points_2.push_back(sift_match);
					}
				}
					cout << points_1.size() << endl;
					cout << points_2.size() << endl;
				for(int i = 0; i < points_1.size(); i++)
                		{
                    		    for(int j =0; j <5; j++)
                        		for(int k = 0; k < 5; k++)
                            		    if (j == 2 || k == 2)
                                		for (int p=0; p <3; p++)
                                    			input_image_1(descriptors_1[points_1[i]].col+k, descriptors_1[points_1[i]].row+j-1, 0, p) = 0;

                		}

            			for(int i = 0; i < points_2.size(); i++)
                		{
                    		    for(int j =0; j <5; j++)
                        		for(int k = 0; k < 5; k++)
                            		    if (j == 2 || k == 2)
                                		for (int p=0; p <3; p++)
                                    		    input_image_2(descriptors_2[points_2[i]].col+k, descriptors_2[points_2[i]].row+j-1, 0, p) = 0;

                		}

            			input_image_1.get_normalize(0,255).save("sift_12.png");
                		input_image_2.get_normalize(0,255).save("sift_21.png");
					
					
				// Visualization Part Taken from http://people.cs.ubc.ca/~lowe/keypoints/
				// and Jacob Senitza's implementation of C++ version (https://github.com/JCSHoosier)
				// Begin
				CImg<double> output(input_image_1,"xyzc",0);
				double max_height = max(input_image_1.height(),input_image_2.height());
		
				output.resize(input_image_1.width()+input_image_2.width(),max_height);

				for(int i = 0; i < input_image_1.width() + input_image_2.width(); i++)
				{
				     for(int j = 0;j < max_height; j++)
				     {
				         if(i < input_image_1.width() && j < input_image_1.height())
					 {
				
					    if(input_image_1.spectrum() == 3)
					    {
						output(i,j,0,0) = input_image_1(i,j,0,0);
						output(i,j,0,1) = input_image_1(i,j,0,1);
						output(i,j,0,2) = input_image_1(i,j,0,2);
					    }
					    else if(input_image_1.spectrum() == 1)
					    {
						output(i,j,0) = input_image_1(i,j,0);
					    }
					
					}
					else if(i < input_image_1.width() && j >= input_image_1.height())
					{
					    output(i,j,0,0) = 0;
					    output(i,j,0,1) = 0;
					    output(i,j,0,2) = 0;			
			
					}
					else if(i >= input_image_1.width() && j < input_image_2.height())
					{
					     if(input_image_2.spectrum() == 3)
					     {
						output(i,j,0,0) = input_image_2(i - input_image_1.width(),j,0,0);
						output(i,j,0,1) = input_image_2(i - input_image_1.width(),j,0,1);
						output(i,j,0,2) = input_image_2(i - input_image_1.width(),j,0,2);
					     }
					     else if(input_image_2.spectrum() == 1)
					     {
						output(i,j,0) = input_image_2(i - input_image_1.width(),j,0);
					     }	
				
					}
					else
					{
					    output(i,j,0,0) = 0;
				    	    output(i,j,0,1) = 0;
					    output(i,j,0,2) = 0;
					}
				}
			}
			// End

			double color[] = {50, 255, 50};
			double x_1, y_1, x_2, y_2;
			for (int i = 0; i < points_1.size(); i++)
			{
			    x_1 = descriptors_1[points_1[i]].col;
			    x_2 = input_image_1.width() + descriptors_2[points_2[i]].col;
			    y_1 = descriptors_1[points_1[i]].row;
			    y_2 = descriptors_2[points_2[i]].row;
				
			    output.draw_line(x_1, y_1, x_2, y_2, color);
			}	
			
			output.get_normalize(0, 255).save("combined_test.png");
	

		}

		if(part == "part1.2" || part == "part1.3")
		{
			double threshold;
			if (part == "part1.2")
			    threshold = 1.1;	
			if (part == "part1.3")
			    threshold = 1.7;
			// This is just a bit of sample code to get you started, to
			// show how to use the SIFT library.

			std :: map <int, string> order;
			string inputFile_1 = argv[2];
			CImg<double> input_image_1(inputFile_1.c_str());
			CImg<double> gray_1 = input_image_1.get_RGBtoHSI().get_channel(2);
			vector<SiftDescriptor> descriptors_1 = Sift :: compute_sift(gray_1);

			for (int a = 3; a < 103; a++)
			{
				string inputFile_2 = argv[a];
				cout <<"Current File : " <<  inputFile_2 << endl;
				CImg<double> input_image_2(inputFile_2.c_str());

				// convert image to grayscale
				CImg<double> gray_2 = input_image_2.get_RGBtoHSI().get_channel(2);
				vector<SiftDescriptor> descriptors_2 = Sift::compute_sift(gray_2);


				int test[descriptors_2.size()][128];

				for (int i = 0; i < descriptors_2.size(); i++)
				{
					for(int j =0; j < 128; j++)
					{
						test[i][j] = descriptors_2[i].descriptor[j];
					}
				}

				std :: vector <int> points_1;
				std :: vector <int> points_2;

				int count = 0;
				for (int c = 0; c < descriptors_1.size(); c++)
				{
					int example[128];
					for (int d = 0; d < 128; d++)
					{
						example[d] = descriptors_1[c].descriptor[d];
					}
					int distance[descriptors_2.size()];

					for (int i =0; i < descriptors_2.size(); i++)
					{
						int sum = 0;
						for (int j=0; j < 128; j ++)
						{
							int square = 0;
							square = (example[j] - test[i][j]) * (example[j] - test[i][j]);
							sum += square;
						}
						distance[i] = sum;
					}

					std::map<int, int> distances;

					//cout << endl;
					for (int i = 0; i < descriptors_2.size(); i++)
						distances[distance[i]] = i;

					sort(distance, distance + descriptors_2.size());
					/*
                    for (std :: map<int, int> :: const_iterator iter = distances.begin();
                         iter != distances.end(); ++iter)

                    cout <<iter->first<<'\t'<<iter->second<<'\n';
                    */
					double chooser = 0.0;
					chooser = double(distance[1]) / double(distance[0]);

					//cout << distance[0] << endl;
					//cout << distance[1] << endl;
					//cout << chooser << endl;

					if (chooser > threshold) //1.6
					{
						count += 1;
						int sift_match = distances[distance[0]];
						//cout << "current point: " << c << '\t' <<"Sift Match: " << sift_match << endl;
						//cout <<"x_1: " <<descriptors_1[c].col << endl;
						//cout <<"y_1: " << descriptors_1[c].row << endl;
						//cout << "x: " <<descriptors_2[sift_match].col << endl;
						//cout << "y: " <<descriptors_2[sift_match].row << endl;
						points_1.push_back(c);
						points_2.push_back(sift_match);
					}

					//cout << "Total Matches: " <<count << endl;
					//cout << inputFile_2 << '\t' << count;
					//cout << "Size : " << points_1.size() << '\t' << points_2.size() << endl;
				}
				if (inputFile_2 != inputFile_1)
				order[count] = inputFile_2;
				//cout << inputFile_2 << '\t' << count << endl;

			}
			int breaker = 0;
			int ranker = 0;
			//for (std :: map<int, string> :: const_iterator iter = order.begin();
			//     iter != order.end(); ++iter)
			std :: map <int, string> :: reverse_iterator iter;
			if (part == "part1.3")
			{
			cout << "Rank" << '\t' << "Matches" << '\t' << "\tImage" << endl;
			for (iter = order.rbegin(); iter != order.rend(); ++iter)
			{
				breaker += 1;
				if (breaker < 11 )
					cout << ++ranker << '\t' <<iter->first<<'\t'<<iter->second<<'\n';
				//else
				//    break;
			}
			}
			if (part == "part1.2")
			{
				 for (iter = order.rbegin(); iter != order.rend(); ++iter)
                                //breaker += 1
                                { 
                                //if (breaker > 1 && breaker < 12 )
                                        cout <<iter->first<<'\t'<<iter->second<<'\n';
				}
                               
                        }

					}
		else if(part == "part2.2")
		{
			//cout << "testing" <<endl;

			std :: map <int, string> order;
			//std :: map<string, int> order;
			string inputFile_1 = argv[2];
			CImg<double> input_image_1(inputFile_1.c_str());
			CImg<double> gray_1 = input_image_1.get_RGBtoHSI().get_channel(2);
			vector<SiftDescriptor> descriptors_1 = Sift :: compute_sift(gray_1);
			/*
            for (int i=0; i < descriptors_1.size(); i++)
            {
                //cout << endl;
                for(int j=0; j < 128; j++)
                {
                cout << descriptors_1[i].descriptor[j] << ' ';
                }
            }*/
			std :: vector<double> ran;
			for(int i = 0; i < 128; i++)
				ran.push_back(double(rand())/RAND_MAX);

			//for (int i =0; i < 128; i++)
			//    cout << ran[i] << endl;
			// do something here!
			std :: vector<int> example;
			//cout << descriptors_1[1].descriptor[1];
			for (int i = 0; i < descriptors_1.size(); i++)
			{
				int summer = 0;
				for(int j = 0; j < 128; j++)
				{
					//cout << i << ' ' << j << endl;
					int temp = 0;
					temp = descriptors_1[i].descriptor[j] * ran[j];
					summer += temp; //+ (descriptors_1[i].descriptor[j] * ran[j]);
				}
				//cout << summer << endl;
				example.push_back(summer);

			}
			//cout << "ended" << endl;

			// target images mapping
			for(int a = 3; a < 103; a++)
			{
				string inputFile_2 = argv[a];
				cout << "Current File : " << inputFile_2 << endl;
				CImg<double> input_image_2(inputFile_2.c_str());
				CImg<double> gray_2 = input_image_2.get_RGBtoHSI().get_channel(2);
				vector<SiftDescriptor> descriptors_2 = Sift :: compute_sift(gray_2);

				/*for (int i=0; i < descriptors_2.size(); i++)
                {
                    cout << endl;
                    for(int j=0; j < 128; j++)
                    {
                        cout << descriptors_2[i].descriptor[j] << ' ';
                    }
            }*/

				std :: vector<int> target;
				for (int i = 0; i < descriptors_2.size(); i++)
				{
					int summer = 0;
					for(int j = 0; j < 128; j++)
					{
						int temp = 0;
						temp = descriptors_2[i].descriptor[j] * ran[j];
						summer += temp;
					}
					//cout << summer << endl;
					target.push_back(summer);
				}
				//cout << "ended" << endl;
				//cout << example.size() << ' ' << descriptors_1.size() << endl;
				//cout << target.size() << ' ' << descriptors_2.size() <<endl;

				// Begin similarity counter

				std :: vector<int> example_points;
				std :: vector<int> target_points;

				for (int i = 0; i < example.size(); i++)
				{
					for(int j = 0; j < target.size(); j++)
					{
						if(example[i] == target[j])
						{
							//cout << i << ' ' << example[i] << ' ' << j << ' ' << target[j] << endl;
							example_points.push_back(i);
							target_points.push_back(j);
						}
					}
				}
				//cout << example_points.size() << endl;
				//cout << target_points.size() << endl;
				int counter = 0;
				for (int i =0; i < example_points.size(); i++)
				{
					int summer = 0;
					for (int j =0; j < 128; j++)
					{
						double val = (descriptors_1[example_points[i]].descriptor[j] - descriptors_2[target_points[i]].descriptor[j]);
						val = val * val;
						summer += val;
					}
					if (summer < 140000) // <140000
					{
						counter +=1;
						//cout << example_points[i] << ' ' << target_points[i] << ' ' << summer << endl;
					}
				}
				//cout << "ended" << endl;
				if (inputFile_2 != inputFile_1)
					order[counter] = inputFile_2;
				//order[inputFile_2] = counter;
			}

			int breaker = 0;
			std :: map <int, string> :: reverse_iterator iter;
			cout << "Rank" << '\t' << "Matches" << '\t' << "\tImage" << endl;

			for (iter = order.rbegin(); iter != order.rend(); ++iter)
			{
				breaker += 1;
				if (breaker < 11 )
					cout << breaker << '\t' <<iter->first<<'\t'<<iter->second<<'\n';
			}
		}
		else if(part == "part3")
		{
			//Image Warping

			char* inputFile = argv[2];
			char* outputFile = argv[3];

			//give args to  CIMG structure
			CImg<double> input(inputFile);
			// to save output file lincoln_warped.png
			CImg<double> output;


			output = Image_Warp(input);
			output.save(outputFile);

			//cout<<output<<endl;
		}
		else
			throw std::string("unknown part!");

		// feel free to add more conditions for other parts (e.g. more specific)
		//  parts, for debugging, etc.
	}
	catch(const string &err) {
		cerr << "Error: " << err << endl;
	}
}

CImg<double>Image_Warp(CImg<double>input){

	double width = input.width();
	double height = input.height();
	int x,y;
	double w;

	CImg<double> output(input,"xyzc",255);

	//double m[3][3];
	double warp_matrix[3][3];


	double m[3][3] = {
			{0.907,0.258,-182},
			{-0.153,1.44,58},
			{-0.000306,0.000731,1.0}
	};

	double determinant = m[0][0]*(m[1][1]*m[2][2] - m[2][1]*m[1][2]) -
						 m[0][1]*(m[1][0]*m[2][2] - m[2][0]*m[1][2]) +
						 m[0][2]*(m[1][0]*m[2][1] - m[2][0]*m[1][1]);


// procedure for taking inverse


	double determinantOfInverse = 1/determinant;

//take inverse
	warp_matrix[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * determinantOfInverse;
	warp_matrix[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * determinantOfInverse;
	warp_matrix[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * determinantOfInverse;
	warp_matrix[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * determinantOfInverse;
	warp_matrix[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * determinantOfInverse;
	warp_matrix[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * determinantOfInverse;
	warp_matrix[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * determinantOfInverse;
	warp_matrix[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * determinantOfInverse;
	warp_matrix[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * determinantOfInverse;


	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){

			w = (i * warp_matrix[2][0] + j * warp_matrix[2][1] + warp_matrix[2][2]);

			x = (i * warp_matrix[0][0] + j * warp_matrix[0][1] + warp_matrix[0][2])/w;

			y = (i * warp_matrix[1][0] + j * warp_matrix[1][1] + warp_matrix[1][2])/w;


			if( x >= 0 && x < width && y >= 0 && y < height){
				output(i,j,0,0) = input(x,y,0,0);

				output(i,j,0,1) = input(x,y,0,1);

				output(i,j,0,2) = input(x,y,0,2);

			}

		}
	}

	return output;

}
