#include <arm_book_lib.h>
#include <mbed.h>

char wifiComCharRead();
void wifiComStringWrite( const char* str );
void wifiComCharWrite( char chr );
char pcSerialComCharRead();
void pcSerialComStringWrite( const char* str );
void pcSerialComCharWrite( char chr );

BufferedSerial uartWifi( PE_8, PE_7, 115200 );
UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

int main()
{
    char receivedCharUsb;
    char receivedCharEsp;
    bool prepareWeb = false;

    pcSerialComStringWrite("AT COMMANDS test\r\n");

    while( true ) {

        if( uartUsb.readable() ) {
            receivedCharUsb = pcSerialComCharRead();
            if (prepareWeb) {
                if ( ( receivedCharUsb == 'h' ) || ( receivedCharUsb == 'H' ) )  {
                    wifiComStringWrite("<!doctype html> <html> <body> Hello! </body> </html>");
                    prepareWeb = false;
                }
            } else if ( receivedCharUsb == '\r' ) {
                wifiComStringWrite("\r\n");
            } else {
                wifiComCharWrite( receivedCharUsb );
            }
        }

        if ( uartWifi.readable() ) {
            receivedCharEsp = wifiComCharRead();
            pcSerialComCharWrite( receivedCharEsp );
            if (receivedCharEsp == '>') {
                prepareWeb = true;
            }
        }
    }
}

char wifiComCharRead()
{
    char receivedChar = '\0';
    if( uartWifi.readable() ) {
        uartWifi.read(&receivedChar,1);
    }
    return receivedChar;
}

void wifiComStringWrite( const char* str )
{
    uartWifi.write( str, strlen(str) );
}

void wifiComCharWrite( char chr )
{
    char str[2] = "";
    sprintf (str, "%c", chr);
    uartWifi.write( str, strlen(str) );
}


char pcSerialComCharRead()
{
    char receivedChar = '\0';
    if( uartUsb.readable() ) {
        uartUsb.read( &receivedChar, 1 );
    }
    return receivedChar;
}

void pcSerialComStringWrite( const char* str )
{
    uartUsb.write( str, strlen(str) );
}

void pcSerialComCharWrite( char chr )
{
    char str[2] = "";
    sprintf (str, "%c", chr);
    uartUsb.write( str, strlen(str) );
}
