//============================================================================
// Name        : helloworld.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include "SimpleOpt.h"
#include <iomanip>
#include <fstream>
#include <vector>
#include "helloworld.h"
//#include "CyAPI.h"

using namespace std;

// for testing macro
#define C0C_COPYRIGHT_YEARS "2004-2012"

#define C0C_V1 3
#define C0C_V2 0
#define C0C_V3 0
#define C0C_V4 0

#define MK_VERSION_STR1(V1, V2, V3, V4) #V1 "." #V2 "." #V3 "." #V4
#define MK_VERSION_STR(V1, V2, V3, V4) MK_VERSION_STR1(V1, V2, V3, V4)

#define C0C_VERSION_STR MK_VERSION_STR(C0C_V1, C0C_V2, C0C_V3, C0C_V4)

// for testing the strtol function
const string str = "2007forum.falinux.com";
char *ptr;
long value;

// for tesing ofstream
const string fileName = "output.bin";
string dumpFileName;

// for testing SimpleOpt.h
enum Options
{
	VENDOR_ID = 0,
	PRODUCT_ID = 1,
	SERIAL_NUMBER = 2,
	DUMP_FILE = 3,
	NUM_OF_FRAMES = 4,
	CAPTURE_TYPE = 5,
	HELP_ID = 6
};

vector<CSimpleOpt::SOption> g_rgOptions =
{
	{ VENDOR_ID,		"-v", SO_REQ_SEP, "Vendor ID of the USB device (hexadecimal)"},
	{ PRODUCT_ID,		"-p", SO_REQ_SEP, "Comma separated list of Product IDs of the USB devices (hexadecimal)"},
	{ SERIAL_NUMBER,	"-s", SO_REQ_SEP, "Serial number of the USB device (string)"},
	{ DUMP_FILE,		"-f", SO_REQ_SEP, "Name of the file to dump extracted frames"},
	{ NUM_OF_FRAMES,	"-n", SO_REQ_SEP, "Number of frames to dump [default = 1]"},
	{ CAPTURE_TYPE,		"-t", SO_REQ_SEP, "Type of capture (raw/raw_processed/depth/pointcloud) [default = raw]"},
	{ HELP_ID,			"-h", SO_NONE, "Help message"},
	SO_END_OF_OPTIONS
};

void help(void);
void cyapi_test(void);

// for testing template & lambda
template <typename T>
void Sort(int *x, int n, T cmp)
{
	for (int i =0; i < n; i++) {
		for (int j = i; j < n; j++) {
			if (cmp(x[i], x[j])) {
				x[i] ^= x[j] ^= x[i] ^= x[j];
			}
		}
	}
}

void swap(int *x, int *y)
{
	*x ^= *y ^= *x ^= *y;
//	*x = *x ^ *y;
//	*y = *y ^ *x;
//	*x = *x ^ *y;
}

int main(int argc, char *argv[]) {
	// for testing macro
	cout << "VERSION " C0C_VERSION_STR " (" __DATE__ " " __TIME__ ")" << endl; // prints !!!Hello World!!!

	// for testing the strtol function
	value = strtol(str.c_str(), &ptr, 10);

	cout << str << " strtol =" << value << "   str=" << ptr << endl;

	cout << hex;
	cout << str << " strtol =" << value << "   str=" << ptr << endl;

	// for testing ofstream
	cout << dec;

	dumpFileName = fileName;
	ofstream f(dumpFileName, ios::binary | ios::out);

	if (!f.good())
	{
		cerr << "Failed to open '" << dumpFileName << "'" << endl;
	}

	f.write(str.c_str(), str.length());

	f.close();

	// for testing SimpleOpt.h
	CSimpleOpt args(argc, argv, g_rgOptions);

	while (args.Next())
	{
		if (args.LastError() == SO_SUCCESS)	{
			switch (args.OptionId()) {
			default:
				help();
				break;
			}
		}
		else {
			cout << args.GetLastErrorText(args.LastError()) << ": '" << args.OptionText() << "' (use -h to get command line help)" << endl;
			help();
		}
	}

	// for testing template & lambda
	int data[10] = {0,1,2,};
	Sort(data, 10, [&](int a, int b){return (a<b?true:false);});

	for (int i = 0; i < 10; i++) {
		cout << data[i] << endl;
	}

	// for testing swap
	int x1 = 2345;
	int y1 = 12372;
	swap(&x1, &y1);
	cout << "x1 = " << x1 << " y1 = " << y1 << endl;

	// for testing CyAPI.lib
	//cyapi_test();

	cout << "sizeof(float) = " << sizeof(float) << endl;

	return 0;
}

void help(void)
{
	CSimpleOpt::SOption *option = g_rgOptions.data();

	while (option->nId >= 0)
	{
		cout << option->pszArg << " " << option->helpInfo << endl;
		option++;
	}
}

#if 0
void cyapi_test(void)
{
	CCyUSBDevice *USBDevice;
	USB_DEVICE_DESCRIPTOR descr;

	USBDevice = new CCyUSBDevice(NULL);

	cout << "device count = " << USBDevice->DeviceCount() << "\n";
}
#endif
