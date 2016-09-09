#include "opencv2/xphoto.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

const char *keys = { "{help h usage ? |         | print this message}"
                     "{i              |         | input image name  }"
                     "{o              |         | output image name }"
                     "{a              |grayworld| color balance algorithm (simple, grayworld, grayedge or learning_based)}"
                     "{ge             |grayworld| gray edge mode (grayworld, maxgrb, shades, general, grayedge, grayedge2) }"
                     "{m              |         | path to the model for the learning-based algorithm (optional) }" };

int main(int argc, const char **argv)
{
    CommandLineParser parser(argc, argv, keys);
    parser.about("OpenCV color balance demonstration sample");
    if (parser.has("help") || argc < 2)
    {
        parser.printMessage();
        return 0;
    }

    string inFilename = parser.get<string>("i");
    string outFilename = parser.get<string>("o");
    string algorithm = parser.get<string>("a");
    string modelFilename = parser.get<string>("m");
    string geMode = parser.get<string>("ge");

    if (!parser.check())
    {
        parser.printErrors();
        return -1;
    }

    Mat src = imread(inFilename, 1);
    if (src.empty())
    {
        printf("Cannot read image file: %s\n", inFilename.c_str());
        return -1;
    }

    Mat res;
    Ptr<xphoto::WhiteBalancer> wb;
    if (algorithm == "simple")
        wb = xphoto::createSimpleWB();
    else if (algorithm == "grayworld")
        wb = xphoto::createGrayworldWB();
    else if (algorithm == "learning_based")
        wb = xphoto::createLearningBasedWB(modelFilename);
    else if (algorithm == "grayedge")
    {
        if (geMode == "grayworld")
            wb = xphoto::createGrayEdge(xphoto::GrayEdgeWB::GrayWorld);
        else if (geMode == "maxgrb")
            wb = xphoto::createGrayEdge(xphoto::GrayEdgeWB::MaxRGB);
        else if (geMode == "shades")
            wb = xphoto::createGrayEdge(xphoto::GrayEdgeWB::ShadesOfGray);
        else if (geMode == "general")
            wb = xphoto::createGrayEdge(xphoto::GrayEdgeWB::GeneralGrayWorld);
        else if (geMode == "grayedge")
            wb = xphoto::createGrayEdge(xphoto::GrayEdgeWB::GrayEdge);
        else if (geMode == "grayedge2")
            wb = xphoto::createGrayEdge(xphoto::GrayEdgeWB::GrayEdge2);
        else
        {
            printf("Unsupported GrayEdge mode: %s\n", geMode.c_str());
            return -1;
        }
    }
    else
    {
        printf("Unsupported algorithm: %s\n", algorithm.c_str());
        return -1;
    }

    wb->balanceWhite(src, res);

    if (outFilename == "")
    {
        namedWindow("after white balance", 1);
        imshow("after white balance", res);

        waitKey(0);
    }
    else
        imwrite(outFilename, res);

    return 0;
}
