#include "help.cpp"
#include "modules/counter.hpp"
#include "modules/view.hpp"

int main(int argc, char* argv[]) {
    //if (argc != 2) return help();

    //VideoCapturePeopleCounter* counter = new VideoCapturePeopleCounter(argv[1]);
	VideoCapturePeopleCounter* counter = new VideoCapturePeopleCounter();
    counter->delegate = new WindowController(counter);
    counter->setRefLineY(120);
    counter->start();
}