#pragma once

#include <exception>
#include <cstring>

class BaseException : public std::exception{
protected:
    static const size_t sizebuff = 128;
    char errormsg[sizebuff]{};

public:
    BaseException() noexcept = default;
    BaseException(const char* msg) noexcept{strncpy(errormsg, msg, sizebuff);}
    ~BaseException() override {}

    const char* what() const noexcept override { return errormsg; }
};


class FileException : public BaseException{
private:
    const char* errIndexMsg = "LoadError";

public:
    FileException() noexcept {snprintf(errormsg, sizebuff, "%s", errIndexMsg);}
    FileException(const char* msg) noexcept
        : BaseException(msg) {}
    ~FileException() override = default;

    const char* what() const noexcept override { return errormsg; }
};

class HttpServerError : public BaseException{
private:
    const char* errIndexMsg = "HttpServerError";

public:
    HttpServerError() noexcept {snprintf(errormsg, sizebuff, "%s", errIndexMsg);}
    HttpServerError(const char* msg) noexcept
        : BaseException(msg) {}
    ~HttpServerError() override = default;

    const char* what() const noexcept override { return errormsg; }
};

class SqlDataBaseError : public BaseException{
private:
    const char* errIndexMsg = "SqlDataBaseError";

public:
    SqlDataBaseError() noexcept {snprintf(errormsg, sizebuff, "%s", errIndexMsg);}
    SqlDataBaseError(const char* msg) noexcept
        : BaseException(msg) {}
    ~SqlDataBaseError() override = default;

    const char* what() const noexcept override { return errormsg; }
};

class ResolveInfoError : public BaseException{
private:
    const char* errIndexMsg = "ResolveInfoError";

public:
    ResolveInfoError() noexcept {snprintf(errormsg, sizebuff, "%s", errIndexMsg);}
    ResolveInfoError(const char* msg) noexcept
        : BaseException(msg) {}
    ~ResolveInfoError() override = default;

    const char* what() const noexcept override { return errormsg; }
};

class UnknownFsError : public BaseException{
private:
    const char* errIndexMsg = "UnknownFsError";

public:
    UnknownFsError() noexcept {snprintf(errormsg, sizebuff, "%s", errIndexMsg);}
    UnknownFsError(const char* msg) noexcept
        : BaseException(msg) {}
    ~UnknownFsError() override = default;

    const char* what() const noexcept override { return errormsg; }
};

class MountError : public BaseException{
private:
    const char* errIndexMsg = "MountError";

public:
    MountError() noexcept {snprintf(errormsg, sizebuff, "%s", errIndexMsg);}
    MountError(const char* msg) noexcept
        : BaseException(msg) {}
    ~MountError() override = default;

    const char* what() const noexcept override { return errormsg; }
};

class UnMountError : public BaseException{
private:
    const char* errIndexMsg = "UnMountError";

public:
    UnMountError() noexcept {snprintf(errormsg, sizebuff, "%s", errIndexMsg);}
    UnMountError(const char* msg) noexcept
        : BaseException(msg) {}
    ~UnMountError() override = default;

    const char* what() const noexcept override { return errormsg; }
};