#include "help.cpp"
#include "modules/counter.hpp"
#include "modules/view.hpp"

class CAlarm :public IAlarm {
public:
	virtual void alarm() {
		printf("%d,some one in...\n", time(0));
	}
};

int main(int argc, char* argv[]) {
    if (argc != 3) return help();
	int y1 = atoi(argv[1]);
	int y2 = atoi(argv[2]);
	CAlarm * pAlarm = new CAlarm();
    //VideoCapturePeopleCounter* counter = new VideoCapturePeopleCounter(argv[1]);
	VideoCapturePeopleCounter* counter = new VideoCapturePeopleCounter(pAlarm);
    counter->delegate = new WindowController(counter);
    //counter->setRefLineY(180,540);
	counter->setRefLineY(y1, y2);
    counter->start();
}