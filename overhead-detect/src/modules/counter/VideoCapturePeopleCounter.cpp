#include <chrono>
#include <set>

using namespace std::chrono;

#define PERSON_MIN_CONTOUR_AREA 2000

#define S_W 1280
#define S_H 720
//#define S_W 640
//#define S_H 480

class IAlarm{
public:
	virtual void alarm() = 0;
};

class VideoCapturePeopleCounter {

public:

    Line refLine;
    Line refLine2;
	IAlarm * m_pAlarm;
	int m_nDeviceIdx;
	int m_type;

    int peopleWhoEnteredCount = 0;
    int peopleWhoExitedCount = 0;
    
    VideoCapturePeopleCounterDelegate* delegate;
    
    VideoCapturePeopleCounter(const string& videoCapturePath,IAlarm*alarm) {
        this->backgroundSubstractor = createBackgroundSubtractorMOG2();
        this->videoCapturePath = videoCapturePath;
		m_type = 1;
		m_pAlarm = alarm;
		m_nDeviceIdx = 0;
    }

	VideoCapturePeopleCounter(IAlarm*alarm,int deviceIdx) {
		this->backgroundSubstractor = createBackgroundSubtractorMOG2();
		m_pAlarm = alarm;
		m_nDeviceIdx = deviceIdx;
		m_type = 0;
	}

    ~VideoCapturePeopleCounter() {
    }

    // setters

    void setRefLineY(int y,int y2) {
        refLineY = y;
		refLineY2 = y2;

		//refLineY = S_H / 4;
  //      refLineY2= S_H *3 / 4;
    }

    // member methods
	void GetGammaTransLUT(uchar *pLUT, float Gamma, int iLUTLen)
	{
		for (int i = 0; i < iLUTLen; i++)
		{
			pLUT[i] = (uchar)(pow((float)i / 255.0, Gamma) * 255);
		}
	}
	void GammaTrans(uchar *pSrc, uchar *pDst, const int iHeight,
		const int iWidth, float Gamma)
	{
		uchar *pLUT = new uchar[256];
		GetGammaTransLUT(pLUT, Gamma, 256);
		for (int i = 0; i < iHeight*iWidth; i++)
		{
			pDst[i] = (uchar)pLUT[pSrc[i]];
		}
		delete[]pLUT;
	}



    void start() {
        Mat frame;
		if (m_type == 1) {
			VideoCapture videoCapture(videoCapturePath);
			//videoCapture.set(CV_CAP_PROP_FPS, 30);//帧数
			//videoCapture.set(CV_CAP_PROP_BRIGHTNESS, 1);//亮度 1
			//videoCapture.set(CV_CAP_PROP_CONTRAST, 10);//对比度 40
			//videoCapture.set(CV_CAP_PROP_SATURATION, 50);//饱和度 50
			//videoCapture.set(CV_CAP_PROP_HUE, 50);//色调 50
			//videoCapture.set(CV_CAP_PROP_EXPOSURE, 0);//曝光 50

			//滤波器的核
			int kernel_size = 3;
			Mat kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);
		
			while (videoCapture.isOpened()) {
				if (!videoCapture.read(frame)) break;

				//-------------------------------------------------------------
				//灰度
				cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

				//gama
				//Mat frame2(frame.size(), frame.type());				
				uchar* pSrc = frame.data;
				uchar* pDst = frame.data;
				const int iHeight = frame.rows;
				const int iWidth = frame.cols;
				GammaTrans(pSrc, pDst, iHeight, iWidth, 0.3);

				//滤波
				filter2D(frame, frame, -1, kernel);
				GaussianBlur(frame, frame, Size(3, 3), 0, 0);
				//-------------------------------------------------------------

				if (++frameNumber == 1) {
					refLine = Line(0, refLineY, frame.cols, refLineY);
					refLine2 = Line(0, refLineY2, frame.cols, refLineY2);
				}
				// erase old contours (seen 16 frames ago)
				unregisterPersonIf([&](const Person* p) {
					return frameNumber - lastFrameWherePersonWasSeen[p] > 16;
				});

				// and then process the current frame
				processFrame(frame);
			}
			return;
		}
        //VideoCapture videoCapture(videoCapturePath);
		VideoCapture videoCapture;
		videoCapture.open(m_nDeviceIdx);
		videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, S_W);//宽度
		videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, S_H);//高度
		videoCapture.set(CV_CAP_PROP_FPS, 30);//帧数
		videoCapture.set(CV_CAP_PROP_BRIGHTNESS, 1);//亮度 1
		videoCapture.set(CV_CAP_PROP_CONTRAST, 40);//对比度 40
		videoCapture.set(CV_CAP_PROP_SATURATION, 50);//饱和度 50
		videoCapture.set(CV_CAP_PROP_HUE, 50);//色调 50
		videoCapture.set(CV_CAP_PROP_EXPOSURE, 50);//曝光 50

        while (videoCapture.isOpened()) {
            if (!videoCapture.read(frame)) break;
            if (++frameNumber == 1) {
                refLine = Line(0, refLineY, frame.cols, refLineY);
                refLine2 = Line(0, refLineY2, frame.cols, refLineY2);
            }
            // erase old contours (seen 16 frames ago)
            unregisterPersonIf([&](const Person* p) {
                return frameNumber - lastFrameWherePersonWasSeen[p] > 16;
            });
            
            // and then process the current frame
            processFrame(frame);
        }
    }

protected:

    set<Person*> people;

    Person* registerPerson(const vector<Point>& contour) {
        time_p now = high_resolution_clock::now();
        Person* person = NULL;
    
        for (set<Person*>::iterator it = people.begin(); it != people.end(); ++it) {
            if ((*it)->hasSimilarContour(contour)) {
                person = *it; 
                person->lastSeen = now;
                person->update(contour);
                countIfPersonIsCrossingTheRefLine(person);
                break;
            }
        }

        if (person == NULL) {
            person = new Person(contour);
            person->firstSeen = now;
            person->lastSeen = now;
            people.insert(person);
        }

        lastFrameWherePersonWasSeen[person] = frameNumber;

        return person;
    }

    template<typename F>
    void unregisterPersonIf(F predicate) {
        for (set<Person*>::iterator it = people.begin(); it != people.end();) {
            Person* person = *it;
            if (predicate(person)) {
                lastFrameWherePersonWasSeen.erase(person);
                linesCrossedByPerson.erase(person);
                it = people.erase(it);
            }
            else {
                ++it;
            }
        }
    }

private:
    
    Ptr<BackgroundSubtractor> backgroundSubstractor;
    string videoCapturePath;

    int refLineY,refLineY2;

    int frameNumber = 0;
    map<const Person*, int> lastFrameWherePersonWasSeen;
    map<const Person*, vector<Line> > linesCrossedByPerson;

    void countIfPersonIsCrossingTheRefLine(const Person* person) {
        int direction = 0;
        
        //if (isPersonCrossingTheRefLine(person, refLine, &direction)) {
        //    if (direction == LINE_DIRECTION_DOWN) peopleWhoEnteredCount++;
        //    else if (direction == LINE_DIRECTION_UP) peopleWhoExitedCount++;
        //}

        if (isPersonCrossingTheRefLine2(person, refLine,refLine2, &direction)) {
            if (direction == LINE_DIRECTION_UP) peopleWhoEnteredCount++;
            else if (direction == LINE_DIRECTION_DOWN) peopleWhoExitedCount++;
        }

		if (m_pAlarm && direction!=0 && peopleWhoEnteredCount > 0) {
			m_pAlarm->alarm();
		}
    }
    
    bool isPersonCrossingTheRefLine(const Person* person, Line line, int* direction = NULL) {
        for (int i = 0; i < linesCrossedByPerson[person].size(); i++) {
            if (line == linesCrossedByPerson[person][i]) {
                return false;
            }
        }

        if (person->trace.size() > 2) {
            for (int i = 0; i < person->trace.size() - 2; i++) {
                if (intersect(person->trace[i], person->trace[i + 1], line.start, line.end)) {
                    if (direction != NULL) {
                        *direction = person->trace[i].y > line.start.y ? LINE_DIRECTION_UP : LINE_DIRECTION_DOWN;
                    }
                    
                    linesCrossedByPerson[person].push_back(line);
                    return true;
                }
            }
        }

        return false;
    }

	bool isPersonCrossingTheRefLine2(const Person* person, Line line, Line line2, int* direction = NULL) {
		for (int i = 0; i < linesCrossedByPerson[person].size(); i++) {
			if (line == linesCrossedByPerson[person][i] || line2 == linesCrossedByPerson[person][i]) {
				return false;
			}
		}

		if (person->trace.size() > 2) {
			for (int i = 0; i < person->trace.size() - 2; i++) {
				if (intersect(person->trace[i], person->trace[i + 1], line.start, line.end)) {
					if (direction != NULL) {
						*direction = person->trace[i].y > line.start.y && person->trace[i].y < line2.start.y ? LINE_DIRECTION_UP : LINE_DIRECTION_DOWN;
					}

					linesCrossedByPerson[person].push_back(line);
					return true;
				}
				if (intersect(person->trace[i], person->trace[i + 1], line2.start, line2.end)) {
					if (direction != NULL) {
						*direction = person->trace[i].y > line.start.y && person->trace[i].y < line2.start.y ? LINE_DIRECTION_UP : LINE_DIRECTION_DOWN;
					}

					linesCrossedByPerson[person].push_back(line);
					return true;
				}

			}
		}

		return false;
	}
    
    void processFrame(const Mat& frame) {
        Mat tempFrame;

        // substract background from frame
        backgroundSubstractor->apply(frame, tempFrame);

        // binarize frame
        threshold(tempFrame, tempFrame, 128, 255, THRESH_BINARY);

        // morph ops
        morphologyEx(tempFrame, tempFrame, MORPH_OPEN, Mat(8, 8, CV_8UC1, Scalar(1)));
        morphologyEx(tempFrame, tempFrame, MORPH_CLOSE, Mat(8, 8, CV_8UC1, cv::Scalar(1)));

        // find contours
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(tempFrame, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // foreach identified person contour
        for (int i = 0; i < contours.size(); ++i) {
            if (contourArea(contours[i]) > PERSON_MIN_CONTOUR_AREA) {
                Person* person = registerPerson(contours[i]);

                if (delegate != NULL) {
                    delegate->onFrameWithPersonDetected(frame, tempFrame, person);
                }
            }
        }

        // notify delegate of frame process
        if (delegate != NULL) {
            delegate->onFrameProcess(frame, tempFrame);
        }
    }

};