#ifndef CREDENTIALS_H
#define CREDENTIALS_H

class Credentials
{
private:
    const char *_wifi_ssid = "BLACKNET";
    const char *_wifi_password = "SarahKonrad2018!";
    const char *_broker_ip = "192.168.10.34";
    const int _broker_port = 1883;
    const char *_broker_user = "arox86";
    const char *_broker_password = "Seehund43er!";

public:
    Credentials();

    const char *getWifiSSID() { return _wifi_ssid; }
    const char *getWifiPassword() { return _wifi_password; }
    const char *getBrokerIP() { return _broker_ip; }
    int getBrokerPort() { return _broker_port; }
    const char *getBrokerUsername() { return _broker_user; }
    const char *getBrokerPassword() { return _broker_password; }

    ~Credentials();
};

#endif /* CREDENTIALS_H */