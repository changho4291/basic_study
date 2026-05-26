//gsoap ns service name: calc
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded
//gsoap ns service namespace: http://example.com/calc.wsdl
//gsoap ns service location: http://localhost:8080

int ns__Add(
    int a,
    int b,
    int &result
);