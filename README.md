# Facial Recognition Model From Facebook Authentication

__Contents__

* [Overview](#overview)
* [How It Works](#howitworks)
* [How To Run](#running)
* [Conclusion](#conclusion)

###<a name="overview"></a>Overview

Facial recognition can add a more seamless interaction with your app. You can imagine incorporating recognition into your phone's lockscreen so that the iPhone 7 doesn't just Raise to Wake but actually unlocks as well. Perhaps you have a photosharing service that you'd like to auto-tag users in or a camera app that automatically adds facial filters to different people based on their preferences to better fit their identity. There are currently apps that make use of this technology but they require the user to capture several photos of themselves as a training pool before they can continue. Instead this project shows a proof-of-concept in which you can offer facial recognition services simply by authenticating your app with Facebook.

###<a name="howitworks"></a>How It Works

![alt text](https://github.com/WarrenGreen/FacebookFaceRecognition/blob/master/readmeImages/FaceRecog_flow.png?raw=true "Flow")

Although Facebook gives us tag information on the user, it only provides the (x,y) coordinates of the bounding box rather than the rectangle itself. So we must detect faces in the photo and determine which one is closest to (x,y) coordinates we've received. As well, the coordinates provided are scaled to a 100px, 100px image and must be scaled to our training image. Once we've cropped the user's images to just their face ROI, we can then train our recognition model.  

__Face Detection__

Face detection in this project utilizes a cascade classifer with multi-scale detection. The cascade classifer applies a series of features to the grayscale version of the image. These features can decorate the result to emphasise edges, lines, feature points, etc. 

![alt text](https://github.com/WarrenGreen/FacebookFaceRecognition/blob/master/readmeImages/haarfeatures.png?raw=true "HAAR Features")

At this point the resulting matrix is classified with the result of this classification determining the path through effectively a decision tree. A simplified example below:

* feature 1 negative
    * feature 2 negative
        * feature 3 negative -> isFace
        * feature 3 positive -> isn'tFace
    * feature 2 positive
        * feature 3 negative -> isn'tFace
        * feature 3 positive -> isFace
* feature 1 positive
    * feature 2 negative
        * feature 3 negative -> isn'tFace
        * feature 3 positive -> isFace
    * feature 2 positive
        * feature 3 negative -> isFace
        * feature 3 positive -> isFace

The multi-scale detection creates an image pyramid from the original grayscale image and creates several copies at a scaled interval. In our project we used a scale factor of 1.10 which means that each level decreases in size by 10%. The images at each scale are run through the cascading classificaiton routine.

![alt text](https://github.com/WarrenGreen/FacebookFaceRecognition/blob/master/readmeImages/imagePyramid.png?raw=true "Image Pyramid")

Once the faces have been detected in each image, the top-left (x,y) pair for each face's bounding rectangle is compared to the (x,y) pair received from Facebook. The closest match is trimmed and saved into our training positive set folder.

__Face Recognition__

Facial recognition relies on Local Binary Pattern Histograms(LBPH) implemented in the OpenCV library. LBPH first creates Local Binary Patterns by sampling image subregions, creates a histogram of these values and then compares this histogram to an incoming image through a classifer such as k-nearest neighbor(KNN). The sampling to create the LBH is done by comparing each pixel, C, to P pixels in the region to it's R neighbors defined as a (P, R) neighborhood. 

![alt text](https://github.com/WarrenGreen/FacebookFaceRecognition/blob/master/readmeImages/LBP1.png?raw=true "(P,R) Neighborhood")

If the pixel C is less than P, a 1 is added to the pattern while a 0 is added for the opposite. A histogram is generated from these patterns which contains the texture information for the image's region.

![alt text](https://github.com/WarrenGreen/FacebookFaceRecognition/blob/master/readmeImages/LBP2.png?raw=true "LBP Generation")

The use of boosters can improve efficiency and accuracy of LBPH classifiers by determining important regions to compare. These regions are then used in a KNN classifer trained by our aquired Facebook tagged photos. 

![alt text](https://github.com/WarrenGreen/FacebookFaceRecognition/blob/master/readmeImages/LBP3.png?raw=true "Sub Regions")

###<a name="running"></a>How To Run

Before downloading the information via Facebook's Graph API, you will need an authentication token for the user which will be saved to a file.

Downloading tag info which is saved to index.csv:

`python scraper.py <access token file>`

Downloading images detailed in index.csv:

`python imageDownloader.py <access token file> <output directory>`

To run the demo in main.cpp, compile and run using cmake.

###<a name="conclusion"></a>Conclusion

Rather than require a training period before you're able to use your app, authenticating with Facebook will offer a more seamless setup with a significantly larger training set than you're likely to achieve. The act of stealing models is a [growing idea](http://arxiv.org/pdf/1609.02943v1.pdf) which prompts us to consider not only how to use them but how to protect our own.

###References
1. <http://docs.opencv.org/2.4/modules/contrib/doc/facerec/facerec_tutorial.html#local-binary-patterns-histograms>
2. <http://docs.opencv.org/2.4/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html>
3. <http://www.ee.oulu.fi/mvg/files/pdf/pdf_494.pdf>
4. <http://liris.cnrs.fr/Documents/Liris-5004.pdf>
5. <https://en.wikipedia.org/wiki/Local_binary_patterns>
6. <http://docs.opencv.org/2.4/modules/contrib/doc/facerec/facerec_api.html>