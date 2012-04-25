/*
 * GString.h
 *
 *  Created on: 08/01/2012
 *      Author: andre
 */

#ifndef GSTRING_H_
#define GSTRING_H_

#include <Arduino.h>

/**
 * GorillaBuilderz utility class, created primarily to expose the underlying Arduino char array backing
 * the String object
 */
class GString :  public String {
	public:
		GString( const char *value = "" ) : String(value) {};
	    GString( const String &value ) : String(value) {};
	    GString( const char value) : String(value) {};
	    GString( const unsigned char value) : String(value) {};
	    GString( const int value, const int base=10) : String(value, base) {};
	    GString( const unsigned int value, const int base=10 ) : String(value, base) {};
	    GString( const long value, const int base=10 ) : String(value, base) {};
	    GString( const unsigned long value, const int base=10 ) : String(value, base) {};

		const GString & operator = ( const GString &rhs ) { return String::operator=(rhs); };
    	const GString & operator +=( const GString &rhs ) { return String::operator+=(rhs); };
    	const GString & operator +=( const char rhs) { return String::operator+=(rhs); };

		char *getBuffer() { return buffer; }
};

#endif /* GSTRING_H_ */
