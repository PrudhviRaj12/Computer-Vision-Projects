'''
Question 2:

Names

Prudhvi Raj Dachapally (prudacha)
Rishabh Monga (rmonga)
Yaswanth Konduri (ykonduri)

Firstly, we apologize in advance for AIs and Professor since we did in Python, 
the running time will be more compared to C/ C++. 
We are trying to catch up with C++ and will try our best to do the next assignment in that.

The arbitrary and Gaussian kernel should not take more than 40 seconds each. 

The edge detection part takes around 2 minutes; 
two 40 seconds (approx.) for each direction, 
and an extra 30 seconds for normalized cross correlation, 
and 15 - 20 seconds for final detection step.

The numbers printed on the screen while the program is run are the iteration counts. 
The instructions to run our program are given below. Please do following them.

General syntax: python detect.py image.png y-dim x-dim <method name>

For testing arbitrary kernel: python detect.py SRSC.png 358 953 arbitrary

For testing the Gaussian kernel: python detect.py SRSC.png 358 953 gaussian

For starting the actual detection process: python detect.py SRSC.png 358 953 edge

Please do bear with us for this one time. Thanks.

References:

1) Gaussian Filter reference : http://stackoverflow.com/questions/13193248/how-to-make-a-gaussian-filter-in-matlab
2) http://stackoverflow.com/questions/17190649/how-to-obtain-a-gaussian-filter-in-python

'''



from PIL import Image
import numpy as np
from scipy.misc import toimage, imsave
#492 653 info
#356 938 srsc
import sys
image_name = sys.argv[1]
y_dim = int(sys.argv[2])
x_dim = int(sys.argv[3])
method = sys.argv[4]
print image_name
print y_dim, x_dim
print method
#print e
X = Image.open(image_name, 'r')
X = X.convert('L')
X = np.reshape(X, (y_dim, x_dim))
C = X
temp = Image.open('new_temp_2.png', 'r')
temp = temp.convert('L')
temp = np.reshape(temp, (37, 19))

flag = 0
if image_name == 'Plaza.png':
    temp = temp.T
    flag = 1
def get_filter_coordinates(image_size, filter_size):
    block_list = []
    counter = 0
    for i in range(0, image_size-filter_size+1):
        for j in range(filter_size, image_size+1):
            counter+=1
            if j - i == filter_size:
                block_list.append((i,j))
    return block_list

def filtering(image, filter):
    count = 0
    x_dim, y_dim = image.shape
    x_filt, y_filt = filter.shape
    block_list_x = get_filter_coordinates(x_dim, x_filt)
    block_list_y = get_filter_coordinates(y_dim, y_filt)
    print len(block_list_x) * len(block_list_y)
    new_image = np.zeros((x_dim, y_dim))
    for i in range(0, len(block_list_x)):
        for j in range(0, len(block_list_y)):
            count+=1
            print count
            x = np.floor(np.average(block_list_x[i]))
            y = np.floor(np.average(block_list_y[j]))

            convolution = np.multiply(image[block_list_x[i][0] : block_list_x[i][1], \
                                            block_list_y[j][0] : block_list_y[j][1]],  filter)

            mean = np.mean(convolution)
            new_image[x, y] = mean
    return new_image

def gaussian_filter_manual(shape, sigma):
    x, y = [edge /2 for edge in shape]
    mat = np.array([[(-( i ** 2 + j ** 2)/(2.0 * sigma ** 2)) for i in range(-x, x+1)] for j in range(-y, y+1)])
    filt = np.exp(mat)/(2.0 * np.pi * sigma ** 2)
    filt /= np.sum(filt)
    return filt

if method == 'arbitrary':
    arbit_filter = np.random.rand(3, 3)
    image = filtering(X, arbit_filter)
    imsave('arbitrary_image.png', image)

if method == 'gaussian':
   gaussian_filter = gaussian_filter_manual((5, 5), 100)
   image = filtering(X, gaussian_filter)
   imsave('gaussian_image.png', image)

if method == 'edge':
   soft_image = X
   x_side = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
   y_side = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])

   new_image = filtering(soft_image, x_side)
   new_image = abs(new_image)
   imsave('x_sobel.png', new_image)

   new_image_2 = filtering(soft_image, y_side)
   new_image_2 = abs(new_image_2)
   imsave('y_sobel.png', new_image_2)

   image = new_image + new_image_2
   imsave('unthresholded_sobel.png', image)
  
   image = image.astype(int)
   ma =  np.max(image)
   mi =  np.min(image)

   for i in range(0, image.shape[0]):
       for j in range(0, image.shape[1]):
           if image[i, j] > ma/2:
               image[i, j] = ma
           if image[i, j] < ma/4:
               image[i, j] = mi
   imsave('edges.png', image)
   
   image = Image.open('edges.png')  
   image = image.convert('L')
   image = np.reshape(image, (y_dim, x_dim))
    
   X = image
   X.flags.writeable = True
   temp.flags.writeable = True
   
   X[X == 0] = 0.01
   temp[temp == 0] = 0.01
   
   X = X/X.max(axis = 0)
   temp = temp/temp.max(axis = 0)
 
   ncc_image = filtering(X, temp)
   imsave('cross_correlation.png', ncc_image)
 
   ncc_image = Image.open('cross_correlation.png')
   ncc_image = ncc_image.convert('L')
   ncc_image = np.reshape(ncc_image, (y_dim, x_dim))

   confi_threshold = 0.001

   new_X = X
   X = ncc_image
   indexes = []
   count = 0
   for i in range(0, X.shape[0]):
       for j in range(0, X.shape[1]):
           if X[i, j] > 100:
               indexes.append((i, j))
   
   
   fil_x, fil_y = temp.shape[0], temp.shape[1]
   new_image = np.zeros((X.shape[0], X.shape[1]))
  
   for_file = []
   for ind in indexes:
       print ind

       x_top, y_top = ind[0] - fil_x/2   , ind[1] - fil_y/2
       x_bot, y_bot = ind[0] + fil_x/2 + 1 , ind[1] + fil_y/2 + 1
       
       k = C[x_top: x_bot, y_top : y_bot]
       k_new = k/k.max(axis = 0)
       confi = np.mean(temp * k_new)
       if confi > confi_threshold:
           for_file.append(x_top)
	   for_file.append(x_bot) 
	   for_file.append(fil_x)
	   for_file.append(fil_y)
	   for_file.append(confi * 10000)
           k = np.array(k)
           k[:, 1] =255; k[:, -2] = 255; k[1, :] = 255; k[-2, :] = 255
           new_X = np.array(new_X)
           new_image[x_top : x_bot, y_top : y_bot] =  k #new_X[x_top : x_bot, y_top : y_bot]
    
   imsave('only_cars.png', new_image)
   imsave('detected.png', abs(C - new_image)+ 1.1 * new_image)
   file_name = open('detected.txt', 'w')
   for item in range(0, len(for_file)):
       file_name.write("%s " % for_file[item])
       if item % 5 == 4:
	   file_name.write('\n')	
   file_name.close()


'''
Seperable Sober Operator Code.
We ignored this method because our method
is still taking the same time as two 1-D kernels on
an image.
'''

'''
pixels = y_dim * x_dim
#imsave('new_conv.png', X)

x_dir_1 = np.array([1, 2, 1])
x_dir_1 = np.reshape(x_dir_1, (3, 1))

x_dir_2 = np.array([-1, 0, 1])
x_dir_2 = np.reshape(x_dir_2, (3, 1))

y_dir_1 = np.array([1, 0, -1])
y_dir_1 = np.reshape(y_dir_1, (1, 3))

y_dir_2 = np.array([1, 2, 1])
y_dir_2 = np.reshape(y_dir_2, (1, 3))

print x_dir_1
print y_dir_1

X_along_x = np.reshape(X, (pixels, 1))
X_along_y = np.reshape(X, (1, pixels))
print X_along_y[:, 0:3]

new_X_along_y = np.zeros((1, pixels))
new_X_along_x = np.zeros((pixels, 1))

for i in range(0, pixels -3):
   current_index = ((i) + (i+3))/2
   new_X_along_y[:, current_index] = abs(np.mean(np.multiply(y_dir_1, X_along_y[:, i: i+3])))
   new_X_along_x[current_index, :] = abs(np.mean(np.multiply(x_dir_2, X_along_x[i: i+3, :])))
   print i

for i in range(0, pixels - 3):
    current_image = ((i) + (i+3))/2
    new_X_along_y[:, current_index] = (np.mean(np.multiply(x_dir_1, new_X_along_y[:, i: i+3])))
    new_X_along_x[current_index, :] = (np.mean(np.multiply(y_dir_2, new_X_along_x[i : i+3, :])))
    print i
x_img = np.reshape(new_X_along_y, (y_dim, x_dim))
y_img = np.reshape(new_X_along_x, (y_dim, x_dim))
#imsave('new_conv_1.png', np.reshape(new_X_along_y, (y_dim, x_dim)))
#imsave('new_conv_2.png', np.reshape(new_X_along_x, (y_di,, x_dim)))
imsave('new_conv.png', (x_img + y_img))

new_img = x_img + y_img
new_img = np.reshape(new_img, (pixels, 1))
ma = np.max(new_img)
for i in range(0, pixels):
    print i
    if new_img[i] > ma/2:
        new_img[i] = ma
    if new_img[i] < ma/4:
        new_img[i] = 0

new_img = np.reshape(new_img, (356, 938))
imsave('new_conv_2.png', new_img)
'''
