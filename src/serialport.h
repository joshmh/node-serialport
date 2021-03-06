
#ifndef _serialport_h_
#define _serialport_h_

#include <node.h>
#include <v8.h>
#include <node_buffer.h>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum SerialPortParity {
  SERIALPORT_PARITY_NONE = 1,
  SERIALPORT_PARITY_MARK = 2,
  SERIALPORT_PARITY_EVEN = 3,
  SERIALPORT_PARITY_ODD = 4,
  SERIALPORT_PARITY_SPACE = 5
};

enum SerialPortStopBits {
  SERIALPORT_STOPBITS_ONE = 1,
  SERIALPORT_STOPBITS_ONE_FIVE = 2,
  SERIALPORT_STOPBITS_TWO = 3
};

#define ERROR_STRING_SIZE 1024

v8::Handle<v8::Value> List(const v8::Arguments& args);
void EIO_List(uv_work_t* req);
void EIO_AfterList(uv_work_t* req);

v8::Handle<v8::Value> Open(const v8::Arguments& args);
void EIO_Open(uv_work_t* req);
void EIO_AfterOpen(uv_work_t* req);
void AfterOpenSuccess(int fd, v8::Handle<v8::Value> dataCallback, v8::Handle<v8::Value> disconnectedCallback, v8::Handle<v8::Value> errorCallback);

v8::Handle<v8::Value> Write(const v8::Arguments& args);
void EIO_Write(uv_work_t* req);
void EIO_AfterWrite(uv_work_t* req);

v8::Handle<v8::Value> Close(const v8::Arguments& args);
void EIO_Close(uv_work_t* req);
void EIO_AfterClose(uv_work_t* req);

v8::Handle<v8::Value> Flush(const v8::Arguments& args);
void EIO_Flush(uv_work_t* req);
void EIO_AfterFlush(uv_work_t* req);

v8::Handle<v8::Value> Drain(const v8::Arguments& args);
void EIO_Drain(uv_work_t* req);
void EIO_AfterDrain(uv_work_t* req);

#ifndef WIN32
v8::Handle<v8::Value> GetStatus(const v8::Arguments& args);
void EIO_GetStatus(uv_work_t* req);
void EIO_AfterGetStatus(uv_work_t* req);

v8::Handle<v8::Value> SetStatus(const v8::Arguments& args);
void EIO_SetStatus(uv_work_t* req);
void EIO_AfterSetStatus(uv_work_t* req);
#endif

SerialPortParity ToParityEnum(const v8::Handle<v8::String>& str);
SerialPortStopBits ToStopBitEnum(double stopBits);

struct OpenBaton {
public:
  char path[1024];
  v8::Persistent<v8::Value> callback;
  v8::Persistent<v8::Value> dataCallback;
  v8::Persistent<v8::Value> disconnectedCallback;
  v8::Persistent<v8::Value> errorCallback;
  int result;
  int baudRate;
  int dataBits;
  int bufferSize;
  bool rtscts;
  bool xon;
  bool xoff;
  bool xany;
  bool dsrdtr;
  SerialPortParity parity;
  SerialPortStopBits stopBits;
  char errorString[ERROR_STRING_SIZE];
};

struct WriteBaton {
public:
  int fd;
  char* bufferData;
  size_t bufferLength;
  size_t offset;
  v8::Persistent<v8::Object> buffer;
  v8::Persistent<v8::Value> callback;
  int result;
  char errorString[ERROR_STRING_SIZE];
};

struct QueuedWrite {
public:
  uv_work_t req;
  ngx_queue_t queue;
  WriteBaton* baton;
};

struct CloseBaton {
public:
  int fd;
  v8::Persistent<v8::Value> callback;
  char errorString[ERROR_STRING_SIZE];
};

struct ListResultItem {
public:
  std::string comName;
  std::string manufacturer;
  std::string serialNumber;
  std::string pnpId;
  std::string locationId;
  std::string vendorId;
  std::string productId;
};

struct ListBaton {
public:
  v8::Persistent<v8::Value> callback;
  std::list<ListResultItem*> results;
  char errorString[ERROR_STRING_SIZE];
};

struct FlushBaton {
public:
  int fd;
  v8::Persistent<v8::Value> callback;
  int result;
  char errorString[ERROR_STRING_SIZE];
};

struct DrainBaton {
public:
  int fd;
  v8::Persistent<v8::Value> callback;
  int result;
  char errorString[ERROR_STRING_SIZE];
};

#ifndef WIN32
struct GetStatusBaton {
public:
  int fd;
  v8::Persistent<v8::Value> callback;
  int result;
  char errorString[ERROR_STRING_SIZE];
};

struct SetStatusBaton {
public:
  int fd;
  int status;
  v8::Persistent<v8::Value> callback;
  int result;
  char errorString[ERROR_STRING_SIZE];
};
#endif

#endif
