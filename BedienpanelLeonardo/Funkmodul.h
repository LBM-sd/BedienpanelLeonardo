/*
  FunkmodulClass.h

  Copyright (c) 2024, SD

*/

#ifndef _FUNKMODULCLASS_h
#define _FUNKMODULCLASS_h


class FunkmodulClass {
public:
    FunkmodulClass();
    FunkmodulClass(int ce_pin, int csn_pin);
    ~FunkmodulClass();

    void init(bool read, bool write, int receiverNr);
    bool sendInteger(int zahl);
    int  readInteger();
    bool available();
    void loop();
    bool isConnected();

private:
    bool fm_connection_ok;
    void updateReplyData();
};

#endif // ifndef _FUNKMODULCLASS_h
