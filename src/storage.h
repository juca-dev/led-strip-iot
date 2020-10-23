#ifndef STORAGE_H
#define STORAGE_H

#include <FS.h>

class Storage
{
public:
  static void setup()
  {
    if (!SPIFFS.begin())
    {
      Serial.println("### ERR: Storage - SPIFFS not configured!");
    }
  }
  static void put(String key, String value)
  {
    String path = String("/" + key);
    Serial.println(path);
    File file = SPIFFS.open(path, "w");
    if (!file)
    {
      Serial.println("### ERR: Storage - opening file for writing");
      return;
    }
    int bytesWritten = file.print(value);
    if (bytesWritten <= 0)
    {
      Serial.println("### ERR: Storage - File write failed");
    }
    file.close();
  }
  static String get(String key)
  {
    String path = String("/" + key);
    Serial.println(path);
    //check for stored credentials
    if (!SPIFFS.exists(path))
    {
      return "";
    }

    File file = SPIFFS.open(path, "r");
    if (!file)
    {
      Serial.println("### ERR: Storage - opening file for reading");
      return "";
    }
    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);
    file.close();

    return buf.get();
  }
};

#endif