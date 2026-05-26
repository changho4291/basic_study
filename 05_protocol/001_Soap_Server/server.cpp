#include <cstdio>
#include "soapH.h"
#include "calc.nsmap"

int ns__Add(struct soap* soap, int a, int b, int& result) {
    printf("Add request: %d + %d\n",a,b);

    result = a + b;

    return SOAP_OK;
}

int main() {
    struct soap soap;

    soap_init(&soap);

    int master =
        soap_bind(&soap,nullptr, 8080, 100);

    if(master < 0) {
        soap_print_fault(&soap,stderr);
        return 1;
    }

    printf("Listening on 8080\n");

    while(true) {
        int slave =soap_accept(&soap);

        if(slave < 0) { continue; }

        soap_serve(&soap);
        soap_end(&soap);
    }

    soap_done(&soap);
}