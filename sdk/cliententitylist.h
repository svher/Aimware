#pragma once

class IClientEntityList {
private:
    virtual void _unused0() = 0;
    virtual void _unused1() = 0;
    virtual void _unused2() = 0;
public:
    virtual Entity* GetClientEntity(int index) = 0;
};

class ClientClass;

class CHLClient {
private:
    virtual void _unused0() = 0;
    virtual void _unused1() = 0;
    virtual void _unused2() = 0;
    virtual void _unused3() = 0;
    virtual void _unused4() = 0;
    virtual void _unused5() = 0;
    virtual void _unused6() = 0;
    virtual void _unused7() = 0;
public:
    virtual ClientClass *GetAllClasses(void) = 0;
};