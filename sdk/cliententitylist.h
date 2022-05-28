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

class IVEngineClient {
private:
    virtual void _unused0() = 0;
    virtual void _unused1() = 0;
    virtual void _unused2() = 0;
    virtual void _unused3() = 0;
    virtual void _unused4() = 0;
    virtual void _unused5() = 0;
    virtual void _unused6() = 0;
public:
    virtual void ClientCmd( const char *szCmdString ) = 0;
};

class IInputSystem {
private:
    virtual void _unused0() = 0;
    virtual void _unused1() = 0;
    virtual void _unused2() = 0;
    virtual void _unused3() = 0;
    virtual void _unused4() = 0;
    virtual void _unused5() = 0;
    virtual void _unused6() = 0;
    virtual void _unused7() = 0;
    virtual void _unused8() = 0;
    virtual void _unused9() = 0;
    virtual void _unused10() = 0;
public:
    /// Enables/disables input. PollInputState will not update current
    /// button/analog states when it is called if the system is disabled.
    virtual void EnableInput( bool bEnable ) = 0;
};