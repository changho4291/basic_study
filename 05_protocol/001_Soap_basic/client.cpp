#include <cstdio>
#include "soapH.h"
#include "calc.nsmap"

int main() {
    struct soap soap;

    soap_init(&soap);

    soap_set_recv_logfile(&soap, "recv.xml");

    soap_set_sent_logfile(&soap, "sent.xml");

    int result;

    if(soap_call_ns__Add(&soap,"http://localhost:8080"
        , nullptr , 10, 20,result ) == SOAP_OK) {
        printf("Result = %d\n", result);
    } else {
        soap_print_fault( &soap, stderr);
    }

    soap_done(&soap);
}