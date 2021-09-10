/*
 * Usage:
#include "myUtils.h"
    Arduino was dependent on some STL (Standard Template Library from C++)
        -- see mydocs/_Arduino.txt
 * FUNCTIONS Included:
getNext(byte limit)
bool inArray (int *start, int *end, T value)
leftPadZero (byte digit)
max_element ( ForwardIterator first, ForwardIterator last )
min_element ( ForwardIterator first, ForwardIterator last )
posFound (T (&inArray)[rows], int needle)
printBits ( T n, bool ln = false )
 * print 64 ints
template <typename T, size_t cols> void prnArray (const T (&a)[cols])                           // 1D cols auto
template <typename T, size_t rows, size_t cols> void prnArray (const T (&a)[rows][cols])        // 2D full auto
template <typename T> void prnArray (T &a, size_t rows)                                         // 2D rows passed
prnSysTime (const char *format=TFORMAT, bool useUTC = false, time_t tsUnix = time(nullptr))
round_uint(unsigned int dividend, unsigned int divisor)
template <typename T, size_t cols> size_t rowCount (T (&a)[cols])                               // 1D
template <typename T, size_t rows, size_t cols> size_t rowCount (T (&a)[rows][cols])            // 2D
tick (uint16_t ticks)						// ticks number of 1ms delays
int vRefInUse()
 
 */


#include "Arduino.h"
#include <ArduinoSTL.h>
#pragma once


// how many chars req'd to print a variable
int charCountInclNull(int num) {
	return 1 + (snprintf (nullptr, 0, "%i", num));
}
int charCountInclNull(long num) {
	return 1 + (snprintf (nullptr, 0, "%li", num));
}
//int charCountInclNull(float num) {
int charCountInclNull(double num) {
	return 1 + (snprintf (nullptr, 0, "%g", num));
}
int charCountInclNull(unsigned int num) {
	return 1 + (snprintf (nullptr, 0, "%u", num));
}
int charCountInclNull(unsigned long num) {
	return 1 + (snprintf (nullptr, 0, "%lu", num));
}
/*template <typename T>		// can't use typeid with templated type
int charCountInclNull(T num) {
	const char * fStr;
	int tID = typeid(num);
		 if (typeid(int) == tID) { fStr = "%i";	}
	else if (typeid(long) == tID) { fStr = "%li"; }
	else if (typeid(double) == tID) { fStr = "%g"; }
	else if (typeid(unsigned int) == tID) { fStr = "%u"; }
	else if (typeid(unsigned long) == tID) { fStr = "%lu"; }
	return 1 + (snprintf (nullptr, 0, fStr, num));
}*/


byte getNext(byte limit) {
  static byte next = 0;
  if (limit == next) next = 0;
  return next++;
}


// get free stack mem
extern "C" {
	//cont_t g_cont;   //'cont_t' does not name a type
//size_t free = cont_get_free_stack(&g_cont);
//Serial.print(F("cont_get_free_stack "));  Serial.print(free);  Serial.print(F(" bytes"));
//ESP.getFreeHeap();	// returns the free heap size.
//ESP.getHeapFragmentation();	// returns the fragmentation metric (0% is clean, more than ~50% is not harmless)
//ESP.getMaxFreeBlockSize();	// returns the maximum allocatable ram block regarding heap fragmentation
}


/*  Value exists in array, given bounds of contiguous memory, https://stackoverflow.com/questions/62620002/program-to-check-any-number-exist-in-2d-array
*/
template <typename T>
bool inArray (int *start, int *end, T value)
{
   while (start != end)
   {
      if ( value == *start )
        return true;
      ++start;
   }
   return false;
}


template <class ForwardIterator>
  ForwardIterator max_element ( ForwardIterator first, ForwardIterator last ) {
	//long* maxel = max_element(uData.dTimeStamp,uData.dTimeStamp+3);
	//Serial.print(F("The largest element is "));  Serial.println(*maxel);
	// replacement for <algorithm> max_element, http://www.cplusplus.com/reference/algorithm/max_element/

	if (first == last) return last;
	ForwardIterator largest = first;

	while (++first != last)
		if (*largest < *first)    // or: if (comp(*first,*smallest)) for version (2)
			largest = first;
	return largest;
}
template <class ForwardIterator>
  ForwardIterator min_element ( ForwardIterator first, ForwardIterator last ) {
	//long* minel = min_element(uData.dTimeStamp,uData.dTimeStamp+3);
	//Serial.print(F("The smallest element is "));  Serial.println(*minel);
	// replacement for <algorithm> min_element, http://www.cplusplus.com/reference/algorithm/min_element/

	if (first == last) return last;
	ForwardIterator smallest = first;

	while (++first != last)
		if (*first < *smallest)    // or: if (comp(*first,*smallest)) for version (2)
			smallest = first;
	return smallest;
}


void leftPadZero (byte digit) {
 // utility function for digital clock display: prints colon and leading 0
 Serial.print(F(":"));
 if(digit < 10)
   Serial.print('0');       // can be F'd
 Serial.print(digit,DEC);  
}

/*	Dynamic format specifiers
	(s)printf format specifiers like %d, within if/for control structures, inappropriately generate type warnings:
	warnings format '%aaa' expects argument of type 'aaa', but ...
	char printFmtStr[64] = {0}; int idx=4;  long long array[]={-9223372036854775807LL,10751563660140LL,10751,10751563660144LL,9223372036854775807LL};
	strcpy(printFmtStr, "Confirming posFound() using find(). Array[ %");
	strcat(printFmtStr, fmtSpec(idx));
	strcat(printFmtStr, "] == %");
	strcat(printFmtStr, fmtSpec(array[idx]));
	strcat(printFmtStr, "\n");
	sprintf(printFmtStr, idx, array[idx]);
*/
const char* fmtSpec(char a)						{return "c";}
const char* fmtSpec(char* a)					{return "c";}
const char* fmtSpec(const char* a)				{return "c";}
const char* fmtSpec(long* a)					{return "ld";}
const char* fmtSpec(double a)					{return "g";}
const char* fmtSpec(float a)					{return "g";}
const char* fmtSpec(int a)						{return "i";}
const char* fmtSpec(unsigned int a)				{return "u";}
const char* fmtSpec(long a)						{return "ld";}
const char* fmtSpec(unsigned long a)			{return "lu";}
const char* fmtSpec(long long a)				{return "lld";}
const char* fmtSpec(unsigned long long a)		{return "llu";}
//const char* fmtSpec(std::string a)				{return "s";}   // error: 'string' is not a member of 'std'
const char* fmtSpec(String a)					{return "s";}


// Find index of needle in haystack (could be multi-dimensional array)
// iterating without rows is possible, but requires memory first addr and last addr to use ptr match
template <typename T, int rows>
size_t posFound (T (&inArray)[rows], T needle) {
	// type array & type needle must be same
	bool debug = false;
	size_t idx = -1;	//time_t idx = -1;
	if (debug) {
		Serial.print(F("\nIn function posFound, inArray[ "));
		for (int i=0; i<rows; i++) {
			Serial.print(inArray[i]);
			if (i < rows-1) Serial.print(F(","));    else Serial.println (F(" }"));
		}
		// F these if resurrected
        //printf if int64_t in the house, print(ln) error: overloaded 'println(long long int&)' is ambiguous
		//Serial.printf("std::begin(inArray): %ld", *std::begin(inArray));     // error: '' is not a member of 'std'
		//Serial.print("std::begin(inArray): ");  Serial.println(*std::begin(inArray));
		//Serial.printf("std::end(inArray): %ld", *std::end(inArray));       // error: '' is not a member of 'std'
		//Serial.print("std::end(inArray): ");  Serial.println(*std::end(inArray));
		//Serial.printf("needle: '%lld'", needle);						
		Serial.print(F("needle: "));  Serial.println(needle);
	}
	T * pos = std::find(std::begin(inArray), std::end(inArray), needle);   // error: '' is not a member of 'std'
	// When the element is not found, std::find returns one past the last element of the array
	//if (pos != std::end(inArray)) {   // error: '' is not a member of 'std'
    if (pos != 0) {
		idx = std::distance(inArray, pos);   // error: '' is not a member of 'std'
	} else {
		if (debug) { Serial.print(F("Needle '"));  Serial.print(needle);  Serial.print(F("' NOT found")); }
	}
	
	return idx;
}


template <typename T>
void printBits ( T n, bool ln = false )	// Prints out Leading Bit Zeros, unlike .print(x, BIN);
//void Vcc_Temp::printBits( byte n, bool ln = false )
{
  //int bits = (sizeof(n) * 8) - 1;			// byte (8 bits) smallest Duino integer type
  //unsigned int bits = (sizeof(n) * 8) - 1;		// unsigned illegal type; no errors but not entering for loop either
  //Serial.print(__PRETTY_FUNCTION__);  Serial.print(F(" bits:"));  Serial.print(bits);  Serial.print(F(" ln:"));  Serial.println(ln);
  byte rev = 1;
  for (int bits = (sizeof(n) * 8) - 1; bits > -1; bits--) {          //Serial.print(bits); Serial.print (F(" rev:"));  Serial.print (rev);  Serial.print (F(" "));
  //for (bits; bits > -1; bits--) {          //Serial.print(bits); Serial.print (F(" rev:"));  Serial.print (rev);  Serial.print (F(" "));
	if (n & (1 << bits))  Serial.print(F("1"));                             // Compare bits 7-0 in byte
	else  Serial.print(F("0"));
	if( 0 == bits ) { if(ln) Serial.println();  break; }
	else if( 5 == ++rev ) { Serial.print(F(" "));  rev = 1; }
  }
}


// All Arrays,     sz = *(&a + 1) - a;                 // pointer math (left for reference)


/*  Dynamic array, one line per row
template <typename T, typename A>
void prn2d (T &a, A arg2) {
    // Serial.print (d1);  Serial.print(F(" d1, "));
    // Serial.print (d2);  Serial.println(F(" d2"));
    for (size_t row=0; row < d1; ++row) {
        for (size_t col=0; col < d2; ++col) {
            Serial.print (a[row][col]);  Serial.print (F(", "));
        }
        Serial.print(F(" 2d["));  Serial.print(row); Serial.println (F("][...]"));
    }
}
*/
/*  1D print fixed array, without args, one line
*/
void prn1d (size_t* a, size_t d1) {
    for (size_t col=0; col < d1; ++col) {
        Serial.print (a[col]);  Serial.print (F(", "));
    }
    Serial.print(F(" 1d["));  Serial.print(d1); Serial.println (F("]"));
}


/*  prnArray (fixed not dynamic), without args, one line per row
*/
template <typename T, size_t d1>        // D1
void prnArray (const T (&a)[d1]) {
    Serial.print(F("prnArray: "));
    for (size_t col=0; col < d1; ++col) {
        Serial.print (a[col]);  Serial.print (F(", "));
    }
    Serial.print(F("1d["));  Serial.print(d1); Serial.println (F("]"));
}
template <typename T, size_t d1, size_t d2>     // D2
void prnArray (const T (&a)[d1][d2]) {
    Serial.print(F("prnArray: "));
    for (size_t row=0; row < d1; ++row) {
        for (size_t col=0; col < d2; ++col) {
            Serial.print (a[row][col]);  Serial.print (F(", "));
        }
        Serial.print(F("2d["));  Serial.print(row); Serial.println (F("][...]"));
    }
}


#ifdef __XTENSA__
//void prnSysTime (const char *format="%a %F %r %Z", bool useUTC = false, time_t tsUnix = time(nullptr))      //error: 'time_t' has not been declared
void prnSysTime (const char *format="%a %F %r %Z", bool useUTC = false, size_t tsUnix = time(nullptr))      //error: 'time_t' has not been declared
{
	// Print a unix time stamp in UTC or local time), http://strftime.net/
	//		format has default (not req'd in caller), see myPrintTime for version without default format
	// %FT%T						// 2019-06-22T11:36:11
	//"%a %Y-%m-%d %H:%M:%S %Z";	// Sun 2019-05-19 13:40:21 MDT
	//"%a %F %r %Z";               // Sun 2019-05-19 01:55:23 PM MDT (default)
    //struct tm* tParts = (useUTC) ? gmtime (&tsUnix) : localtime (&tsUnix);      // error: 'gmtime' was not declared in this scope
    // strftime offers most format options than ctime, like 4 digit yr, timezone
    char buf[32];
    //strftime(buf, sizeof(buf), format, tParts);      // error: 'strftime' was not declared in this scope
    Serial.printf("%s", buf);
}
#endif


// https://forum.arduino.cc/t/printing-uint64_t/364646/8#msg2610248 (code to add Print:: funcs)
// Need whole Print.h/.cpp replacement files each IDE update? or just extra code to import, prototypes/functions, when needed until support added by Arduino??
// dwb added protos to esp ... Print.h, but overwritten by update
/* similar in custom Print.h/.cpp
size_t Print::println(int64_t number, int base) {
    size_t n = 0;
    n += print(number, base);
    n += println();
    return n;
}

size_t Print::print(int64_t number, int base) {
    size_t n = 0;
    if (number < 0) {
        write('-');
        number = -number;
        n++;
    }
    n += print((uint64_t)number, base);
    return n;
}

size_t Print::println(uint64_t number, int base) {
    size_t n = 0;
    n += print((uint64_t)number, base);
    n += println();
    return n;
}

size_t Print::print(uint64_t number, int base) {
    size_t n = 0;
    unsigned char buf[64];
    uint8_t i = 0;

    if (number == 0) {
        n += print((char)'0');
        return n;
    }

    if (base < 2) base = 2;
    else if (base > 16) base = 16;

    while (number > 0) {
        uint64_t q = number/base;
        buf[i++] = number - q*base;
        number = q;
    }

    for (; i > 0; i--)
		n += write((char) (buf[i - 1] < 10 ?
		'0' + buf[i - 1] :
		'A' + buf[i - 1] - 10));

    return n;
}
*/


unsigned int round_uint(unsigned int dividend, unsigned int divisor)
{
    return (dividend + (divisor / 2)) / divisor;
}


/*  Auto-detect number of 1D or 2D rows, overloaded template functions
       use rowCount (array_name);
*/
template <typename T, size_t rows, size_t cols>
size_t rowCount (T (&a)[rows][cols]) {         // use rowCount (array_name);
    return rows;
}
template <typename T, size_t cols>
size_t rowCount (T (&a)[cols]) {         // use rowCount (array_name);
    return 1;
}


void tick (uint16_t ticks) {
	for (uint16_t i=0; i<ticks; i++) delay(1);
}


// typeid(var).name(); is disabled in Arduino IDE, so using polymorphism, also use auto over decltype
const char* varType(char a)					{return "char";}
const char* varType(char* a)				{return "char*";}
const char* varType(const char* a)			{return "const char*";}
const char* varType(long* a)				{return "long*";}					// time_t
const char* varType(double a)				{return "double";}
const char* varType(float a)				{return "float";}
const char* varType(int a)					{return "int";}
const char* varType(unsigned int a)			{return "unsigned int";}
const char* varType(long a)					{return "long";}					// ld
const char* varType(unsigned long a)		{return "unsigned long";}			// lu
const char* varType(long long a)			{return "long long";}				// lld
//const char* varType(unsigned long long a)	{return "unsigned long long";}		// llu, note: previous declaration 'const char* varType(long long unsigned int)'
//const char* varType(std::string a)			{return "std::string";}     //error: 'const char* varType' redeclared as different kind of symbol
const char* varType(String a)				{return "String";}
//const char* varType(time_t a)				{return "time_t";}


/* Where did Bitwork go?
#ifdef ARDUINO_ARCH_AVR             // AVR
// Reads 2 high bits of ADMUX, which voltage reference are we using
//int Bitwork::vRefInUse( bool debug = false )
int vRefInUse()
{
  int vRef = ADMUX >> 6;							// keep 2 high bits of ADMUX = Voltage Reference (Arduino Uno)
  Serial.print(F("_dmm.getDebug() "));  Serial.println( _dmm.getDebug() );
  //if( _dmm.getDebug() ) {  Serial.print(F("vRef "));  Serial.println( vRef ); }  //Serial.print(F(", ln "));  Serial.println(ln);   //error: '_dmm' was not declared in this scope
  return vRef;
}
#endif
*/