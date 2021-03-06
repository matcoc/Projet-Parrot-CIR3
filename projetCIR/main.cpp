#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <vector>
#include <string>
#include <control.h>
#include <image.h>
#include <unistd.h>
#include <math.h>
//Notre classe cercle permettant de retourner le resultat d'une reconnaissance
#include "recognition/detection.h"

using namespace cv;
using namespace std;
cv::Mat frame;


class ImageProcessing : public sumo::Image
{
public:
	void handleImage(const struct sumo::image *, const uint8_t *buffer, size_t size)
	{
    //Transformation du buffer en objet Mat pour l'exploitation openCV
    std::vector<uint8_t> v;
    v.assign(buffer, buffer+size);
    frame = cv::imdecode(v, CV_LOAD_IMAGE_COLOR);
		//fprintf(stderr, "received image of %zu bytes at %p\n", size, buffer);
	}
};
int H_MIN_GREEN = 0;
int H_MAX_GREEN = 255;
int S_MIN_GREEN = 0;
int S_MAX_GREEN = 255;
int V_MIN_GREEN = 0;
int V_MAX_GREEN = 255;

int H_MIN_RED = 0;
int H_MAX_RED = 255;
int S_MIN_RED = 0;
int S_MAX_RED = 255;
int V_MIN_RED = 0;
int V_MAX_RED = 255;

int H_MIN_GREEN_DEFAULT = 0;
int H_MAX_GREEN_DEFAULT = 255;
int S_MIN_GREEN_DEFAULT = 0;
int S_MAX_GREEN_DEFAULT = 255;
int V_MIN_GREEN_DEFAULT = 0;
int V_MAX_GREEN_DEFAULT = 255;

int H_MIN_RED_DEFAULT = 0;
int H_MAX_RED_DEFAULT = 255;
int S_MIN_RED_DEFAULT = 0;
int S_MAX_RED_DEFAULT = 255;
int V_MIN_RED_DEFAULT = 0;
int V_MAX_RED_DEFAULT = 255;


int sumoOk=0;



bool readDefaultCalibration(){
	ifstream infile("../default.par", ios::in);
	if(infile){
		string line;

		getline(infile, line);
		H_MIN_GREEN_DEFAULT = atoi(line.c_str());
		cout << "H_MIN GREEN DEFAULT" <<  H_MIN_GREEN_DEFAULT << endl;
		getline(infile, line);
		H_MAX_GREEN_DEFAULT = atoi(line.c_str());
		getline(infile, line);
		S_MIN_GREEN_DEFAULT = atoi(line.c_str());
		getline(infile, line);
		S_MAX_GREEN_DEFAULT = atoi(line.c_str());
		getline(infile, line);
		V_MIN_GREEN_DEFAULT = atoi(line.c_str());
		getline(infile, line);
		V_MAX_GREEN_DEFAULT = atoi(line.c_str());

		getline(infile, line);
		H_MIN_RED_DEFAULT = atoi(line.c_str());
		getline(infile, line);
		H_MAX_RED_DEFAULT = atoi(line.c_str());
		getline(infile, line);
		S_MIN_RED_DEFAULT = atoi(line.c_str());
		getline(infile, line);
		S_MAX_RED_DEFAULT = atoi(line.c_str());
		getline(infile, line);
		V_MIN_RED_DEFAULT = atoi(line.c_str());
		getline(infile, line);
		V_MAX_RED_DEFAULT = atoi(line.c_str());
		infile.close();
		return true;
	}

	return false;

}

bool writeDefaultCalibration(){
	ofstream infile("../default.par", ios::out);
	if(infile){
		infile << H_MIN_GREEN << endl;
		infile << H_MAX_GREEN << endl;
		infile << S_MIN_GREEN << endl;
		infile << S_MAX_GREEN << endl;
		infile << V_MIN_GREEN << endl;
		infile << V_MAX_GREEN << endl;

		infile << H_MIN_RED << endl;
		infile << H_MAX_RED << endl;
		infile << S_MIN_RED << endl;
		infile << S_MAX_RED << endl;
		infile << V_MIN_RED << endl;
		infile << V_MAX_RED << endl;
		infile.close();
		return true;
	}
	return false;
}


void on_trackbar(int, void*)
{//This function gets called whenever a
	// trackbar position is changed
	//cout <<"H: " <<  H_MIN << ", " << H_MAX << endl;
}
void createTrackbars()
{
	String trackbarWindowName = "TrackBars";
	readDefaultCalibration();
	namedWindow(trackbarWindowName, WINDOW_NORMAL);
	createTrackbar("H_MIN_GREEN", trackbarWindowName, &H_MIN_GREEN, H_MAX_GREEN, on_trackbar);
	createTrackbar("H_MAX_GREEN", trackbarWindowName, &H_MAX_GREEN, H_MAX_GREEN, on_trackbar);
	createTrackbar("S_MIN_GREEN", trackbarWindowName, &S_MIN_GREEN, S_MAX_GREEN, on_trackbar);
	createTrackbar("S_MAX_GREEN", trackbarWindowName, &S_MAX_GREEN, S_MAX_GREEN, on_trackbar);
	createTrackbar("V_MIN_GREEN", trackbarWindowName, &V_MIN_GREEN, V_MAX_GREEN, on_trackbar);
	createTrackbar("V_MAX_GREEN", trackbarWindowName, &V_MAX_GREEN, V_MAX_GREEN, on_trackbar);

	createTrackbar("H_MIN_RED", trackbarWindowName, &H_MIN_RED, H_MAX_RED, on_trackbar);
	createTrackbar("H_MAX_RED", trackbarWindowName, &H_MAX_RED, H_MAX_RED, on_trackbar);
	createTrackbar("S_MIN_RED", trackbarWindowName, &S_MIN_RED, S_MAX_RED, on_trackbar);
	createTrackbar("S_MAX_RED", trackbarWindowName, &S_MAX_RED, S_MAX_RED, on_trackbar);
	createTrackbar("V_MIN_RED", trackbarWindowName, &V_MIN_RED, V_MAX_RED, on_trackbar);
	createTrackbar("V_MAX_RED", trackbarWindowName, &V_MAX_RED, V_MAX_RED, on_trackbar);

	setTrackbarPos("H_MIN_GREEN", trackbarWindowName, H_MIN_GREEN_DEFAULT);
	setTrackbarPos("H_MAX_GREEN", trackbarWindowName, H_MAX_GREEN_DEFAULT);
	setTrackbarPos("S_MIN_GREEN", trackbarWindowName, S_MIN_GREEN_DEFAULT);
	setTrackbarPos("S_MAX_GREEN", trackbarWindowName, S_MAX_GREEN_DEFAULT);
	setTrackbarPos("V_MIN_GREEN", trackbarWindowName, V_MIN_GREEN_DEFAULT);
	setTrackbarPos("V_MAX_GREEN", trackbarWindowName, V_MAX_GREEN_DEFAULT);

	setTrackbarPos("H_MIN_RED", trackbarWindowName, H_MIN_RED_DEFAULT);
	setTrackbarPos("H_MAX_RED", trackbarWindowName, H_MAX_RED_DEFAULT);
	setTrackbarPos("S_MIN_RED", trackbarWindowName, S_MIN_RED_DEFAULT);
	setTrackbarPos("S_MAX_RED", trackbarWindowName, S_MAX_RED_DEFAULT);
	setTrackbarPos("V_MIN_RED", trackbarWindowName, V_MIN_RED_DEFAULT);
	setTrackbarPos("V_MAX_RED", trackbarWindowName, V_MAX_RED_DEFAULT);
}

void makeSequence(vector<Point2i> listPointDraw, int sumoOk, sumo::Control * Sumo){
	double theta;
	cout << "taille liste: " << listPointDraw.size() << endl;
	double dist;
	double oldgeneral = 180*atan2((listPointDraw[1].y-listPointDraw[0].y),(listPointDraw[1].x-listPointDraw[0].x))/M_PI;
	for (unsigned int i = 2; i < listPointDraw.size()+1; i++) {
			dist = sqrt(pow((listPointDraw[i].y-listPointDraw[i-1].y),2)+pow((listPointDraw[i].x-listPointDraw[i-1].x),2));
			theta = oldgeneral+(180*atan2((listPointDraw[i].y-listPointDraw[i-1].y),(listPointDraw[i].x-listPointDraw[i-1].x)))/M_PI;
			if (i==2){
				theta=0;
			}
			//theta = (int)theta%360;
			//if (theta>180){
			//	theta = -(360-theta);
			//}
			oldgeneral = 180*atan2((listPointDraw[i].y-listPointDraw[i-1].y),(listPointDraw[i].x-listPointDraw[i-1].x))/M_PI;
						cout << (int)theta%180 << endl;


			theta = (int)theta%180;
			/*if (theta>=90){
				cout << "angle: " << -(int)th << endl;
			}
			if (theta<90){
				cout << "angle: " << (int)theta%90 << endl;
			}*/
			//v = 10;
			//if(sumoOk && Sumo){
				//Sumo->move(v,theta);
		//	}
			// wait?
				if(sumoOk && Sumo){
					Sumo->move(0,0);
					Sumo->quickTurn((M_PI*theta/180));
					usleep(1000000);
					Sumo->move(20,0);
					usleep(0.2*(dist/20)*1000000);
				}
		//oldPoint = listPointDraw[i];
	}
}

int main()
{
		createTrackbars();
		on_trackbar(0, 0);

		//Ouverture du sumo
		sumo::Control * Sumo;
	  Sumo = new sumo::Control(new ImageProcessing);

		if(sumoOk == 1){
			Sumo->open();
		}



    VideoCapture stream1(0);
		stream1.set(CV_CAP_PROP_FRAME_WIDTH, 800);
		stream1.set(CV_CAP_PROP_FRAME_HEIGHT, 600);

    int trace=0;
    vector<Point2i> listPointDraw;
    double deltaT=0;
    double deltaT2=0;
    double fps = 15;
    OurCircle oldRedPoint;
    stream1.set(CV_CAP_PROP_FPS, fps);

		bool calibration = true;
		while (calibration){
			Mat redFilter;
			Mat greenFilter;
			Mat cameraFrame;
			stream1.read(cameraFrame);
			flip(cameraFrame, cameraFrame, 1);

			detectColor(cameraFrame, redFilter, H_MIN_RED, H_MAX_RED, S_MIN_RED, S_MAX_RED, V_MIN_RED, V_MAX_RED);
			detectColor(cameraFrame, greenFilter, H_MIN_GREEN, H_MAX_GREEN, S_MIN_GREEN, S_MAX_GREEN, V_MIN_GREEN, V_MAX_GREEN);
			Point2i smallWord;
			smallWord.x = 10;//cameraFrame.cols/2;
			smallWord.y = 10;

			//putText(cameraFrame, "Calibration vert", Point(20,20), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);

			imshow("Green calibration", greenFilter);
			imshow("Red calibration", redFilter);

			if(waitKey(30)>=0){
				writeDefaultCalibration();
				destroyAllWindows();
				break;
			}
		}

    while(true){
      if(!frame.empty()){
        imshow("Sumo view", frame); //Affichage
      }
      if(waitKey(30)>=0 && trace==0){
        trace=1;
        cout << "Traçage du chemin..." << endl;
      } else if (waitKey(30)>=0 && trace==1){
        trace=0;
        makeSequence(listPointDraw, sumoOk, Sumo);
        cout << "Fin du dessin du chemin " << endl;
        listPointDraw.clear();
      }
      string battery = " ";
      if(Sumo && sumoOk){
        battery= to_string(Sumo->batteryLevel())+"%";
      }
      deltaT +=1/fps;

      Mat cameraFrame;
			Mat redFilter;
			Mat greenFilter;
      stream1.read(cameraFrame);
      flip(cameraFrame, cameraFrame, 1);
      Mat cameraFrameOrigin = cameraFrame;
      detectColor(cameraFrame, redFilter, H_MIN_RED, H_MAX_RED, S_MIN_RED, S_MAX_RED, V_MIN_RED, V_MAX_RED);
			detectColor(cameraFrame, greenFilter, H_MIN_GREEN, H_MAX_GREEN, S_MIN_GREEN, S_MAX_GREEN, V_MIN_GREEN, V_MAX_GREEN);
			OurCircle redPoint;
			redPoint = findPoint(redFilter);
       putText(cameraFrame, battery, Point(20,35) , FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 1);
       if (trace==1){
       		if(sumoOk && Sumo)
       			Sumo->move(0,0);
       		if(deltaT2>=1){
       			OurCircle c2;
			c2 = findPoint(greenFilter);
				if (c2.exist){
					listPointDraw.push_back(c2.center);
				}
				deltaT2 = 0;

       		} else {
       			deltaT2+=1/fps;
       		}
       		for (unsigned int i=0; i<listPointDraw.size(); i++){
				circle(cameraFrame, listPointDraw[i], 7, Scalar(255, 0, 0), CV_FILLED);
			}

       }
       if (trace==0){
			if(redPoint.exist){
				cv::Scalar blue(0,255,0);
				cv::circle(cameraFrame, redPoint.center, redPoint.radius, blue, 3);
        if(deltaT>=0.1){

          if(!oldRedPoint.exist){
            oldRedPoint = redPoint;

          } else {
            cout << (-oldRedPoint.center.y+redPoint.center.y)<< endl;
              if((-oldRedPoint.center.y+redPoint.center.y)<-60){
                std::cout << " JUMP! " << endl;
								if(sumoOk && Sumo){
                					Sumo->highJump();
								}
              }
              if((-oldRedPoint.center.x+redPoint.center.x)>60){
                std::cout << " slalom! " << endl;
                if(sumoOk && Sumo){
                  Sumo->slalom();
                }
              }
          }
          deltaT = 0;
        }

        oldRedPoint = redPoint;

			} else {
        oldRedPoint.exist = false;
				OurCircle c;
				c = findPoint(greenFilter);
				double vitesse, angle;
				if(c.exist){
					Point2i smallWord = c.center;
	        smallWord.x -= 40;
	        smallWord.y += 10;

	        //cout << "center[" << maxi << "] : " << c.center.x <<   endl;
					cv::Scalar red(255,0,0);
	        cv::circle(cameraFrame, c.center, c.radius, red, 3);

	        if(c.center.x > 2*cameraFrame.cols/5 && c.center.x < 3*cameraFrame.cols/5 ){
	          angle = 0;
	          if(vitesse>0){
	            putText(cameraFrame, "Avancer", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	          }else if(vitesse<0){
	            putText(cameraFrame, "Reculer", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	          } else if (vitesse==0){
	            putText(cameraFrame, "Arret", smallWord, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
	          }
	        } else {
	          angle = (double)(-50/(double)(-cameraFrame.cols/2))*(-cameraFrame.cols/2+c.center.x);
	          if (vitesse>0){
	            if(angle>0){
	              putText(cameraFrame, "Avancer droite", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }else if(angle<0){
	              putText(cameraFrame, "Avancer gauche", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }
	          } else if (vitesse<0){
	            if(angle>0){
	              putText(cameraFrame, "Reculer droite", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }else if(angle<0){
	              putText(cameraFrame, "Reculer gauche", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }
	          } else if (vitesse ==0){
	             if(angle>0){
	              putText(cameraFrame, "Droite", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }else if(angle<0){
	              putText(cameraFrame, "Gauche", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	          }
	        } }

	        if(c.center.y > 2*cameraFrame.rows/5 && c.center.y < 3*cameraFrame.rows/5) {
	          vitesse = 0;
	          if(angle>0){
	            putText(cameraFrame, "Droite", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	          }else if(angle<0){
	            putText(cameraFrame, "Gauche", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	          } else if (angle==0){
	            putText(cameraFrame, "Arret", smallWord, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
	          }
	        } else {

	          vitesse =  (double)(75/(double)(cameraFrame.rows/2))*(-c.center.y+cameraFrame.rows/2);
	          if (vitesse>0){
	            if(angle>0){
	              putText(cameraFrame, "Avancer droite", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }else if(angle<0){
	              putText(cameraFrame, "Avancer gauche", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }
	          } else if (vitesse<0){
	            if(angle>0){
	              putText(cameraFrame, "Reculer droite", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }else if(angle<0){
	              putText(cameraFrame, "Reculer gauche", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }
	          } else if (vitesse== 0){
	             if(angle>0){
	              putText(cameraFrame, "Droite", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	            }else if(angle<0){
	              putText(cameraFrame, "Gauche", smallWord, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
	          }
	        }
	      }
	      } else {
	        vitesse = 0;
	        angle = 0;
	      } 				cout << "vitesse: " <<  vitesse << " angle: " << angle << endl;
        rectangle(cameraFrame, Point(10,50), Point(10+1.2*fabs(vitesse),70),Scalar(0, 0, 255), CV_FILLED);
				if(Sumo && sumoOk && trace == 0)
					Sumo->move(vitesse*1,angle);
			}


			}




    imshow("Camera", cameraFrame);
		//imshow("camera2", cameraFrameOrigin);
		imshow("Red", redFilter);
		imshow("Green", greenFilter);


      //imshow("result", dest);
    //if(waitKey(30)>=0)
      //break;
    }

    return 0;
}
