# Facial Recognition Model From Facebook Authentication

__Contents__

* [Overview](#overview)
* [How It Works](#howitworks)
* [How To Run](#running)
* [Conclusion](#conclusion)

###Overview<a name="#overview"></a>

Facial recognition can add a more seamless interaction with your app. You can imagine incorporating recognition into your phone's lockscreen so that the iPhone 7 doesn't just Raise to Wake but actually unlocks as well. Perhaps you have a photosharing service that you'd like to auto-tag users in or a camera app that automatically adds facial filters to different people based on their preferences to better fit their identity. There are currently apps that make use of this technology but they require the user to capture several photos of themselves as a training pool before they can continue. Instead this project shows a proof-of-concept in which you can offer facial recognition services simply by authenticating your app with Facebook.

###How It Works<a name="howitworks"></a>

TODO FLOWCHART

Although Facebook gives us tag information on the user, it only provides the (x,y) coordinates of the bounding box rather than the rectangle itself. So we must detect faces in the photo and determine which one is closest to (x,y) coordinates we've received. As well, the coordinates provided are scaled to a 100px, 100px image and must be scaled to our training image. Once we've cropped the user's images to just their face ROI, we can then train our recognition model.  

__Face Detection__

Face detection in this project utilizes a cascade classifer with multi-scale detection. The cascade classifer applies a series of features to the grayscale version of the image. These features can decorate the result to emphasise edges, lines, feature points, etc. At this point the resulting matrix is classified with the result of this classification determining the path through effectively a decision tree. A simplified example below:

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

Once the faces have been detected in each image, the top-left (x,y) pair for each face's bounding rectangle is compared to the (x,y) pair received from Facebook. The closest match is trimmed and saved into our training positive set folder.

__Face Recognition__

Facial recognition relies on Local Binary Pattern Histograms(LBPH) implemented in the OpenCV library. LBPH, obvious from the name, first creates Local Binary Patterns by sampling image subregions, creates a histogram of these values and then compares this histogram to an incoming image through a classifer such as k-nearest neighbor(KNN). The sampling to create the LBH is done by comparing each pixel, C, to P pixels in the region to it's R neighbors defined as a (P, R) neighborhood. If the pixel C is less than P, a 1 is added to the pattern while a 0 is added for the opposite. A histogram is generated from these patterns which contains the texture information for the image's region. The use of boosters can improve efficiency and accuracy of LBPH classifiers by determining important regions to compare. These regions are then used in a KNN classifer trained by our aquired Facebook tagged photos. 

###How To Run<a name="#running"></a>

Before downloading the information via Facebook's Graph API, you will need an authentication token for the user which will be saved to a file.

Downloading tag info which is saved to index.csv:

`python scraper.py <access token file>`

Downloading images detailed in index.csv:

`python imageDownloader.py <access token file> <output directory>`

To run the demo in main.cpp, compile and run using cmake.

###Conclusion

###References
* http://docs.opencv.org/2.4/modules/contrib/doc/facerec/facerec_tutorial.html#local-binary-patterns-histograms