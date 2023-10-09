/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "spiffs.h"

#include "config.h"
#include "debug.h"


namespace spiffs {
    File streamFile;

    // ===== PRIVATE ===== //
    void fixPath(String& path) {
        if (!path.startsWith("/")) {
            path = "/" + path;
        }
    }

    // ===== PUBLIC ====== //
    void begin() {
        debug("Initializing SPIFFS...");
        LittleFS.begin(true);
        debugln("OK");

        String FILE_NAME = "/startup_spiffs_test";

        remove(FILE_NAME);
        create(FILE_NAME);
        File f = open(FILE_NAME);
        if (!f) {
            ESP_LOGE("", "test fs fail!");
            format();
        } else {
            f.close();
            ESP_LOGE("", "test fs done!");
            remove(FILE_NAME);
        }
    }

    void format() {
        debug("Formatting SPIFFS...");
        LittleFS.format();
        debugln("OK");
    }

    size_t size() {
        return LittleFS.totalBytes();
    }

    size_t usedBytes() {
        return LittleFS.usedBytes();
    }

    size_t freeBytes() {
        return LittleFS.totalBytes() - LittleFS.usedBytes();
    }

    size_t size(String fileName) {
        fixPath(fileName);

        File f = LittleFS.open(fileName, "r");

        return f.size();
    }

    bool exists(String fileName) {
        return LittleFS.exists(fileName);
    }

    File open(String fileName) {
        fixPath(fileName);

        ESP_LOGI("", "File name %s", fileName);

        File f = LittleFS.open(fileName, "a+");

        f.seek(0);

        return f;
    }

    void create(String fileName) {
        fixPath(fileName);

        File f = LittleFS.open(fileName, "a+");

        f.close();
    }

    void remove(String fileName) {
        fixPath(fileName);

        File f = LittleFS.open(fileName, "a+");
        f.close();

        LittleFS.remove(fileName);
    }

    void rename(String oldName, String newName) {
        fixPath(oldName);
        fixPath(newName);

        LittleFS.rename(oldName, newName);
    }

    void write(String fileName, const char* str) {
        ESP_LOGE("", "write!!!!!!");
        File f = open(fileName);

        if (f) {
            f.println(str);
            f.close();
            debugln("Wrote file");
        } else {
            debugln("File error");
        }
    }

    void write(String fileName, const uint8_t* buf, size_t len) {
        ESP_LOGE("", "write!!!!!! utf8");
        File f = open(fileName);

        if (f) {
            f.write(buf, len);
            f.close();
            debugln("Wrote file");
        } else {
            debugln("File error");
        }
    }

    String listDir(String dirName) {

        String res;

        fixPath(dirName);

        File root = open(dirName);

        File file = root.openNextFile();

        while (file) {
            res += file.name();
            res += ' ';
            res += size(file.name());
            res += '\n';
            file = root.openNextFile();
        }

        if (res.length() == 0) {
            res += "\n";
        }

        return res;
    }

    void streamOpen(String fileName) {
        streamClose();
        streamFile = open(fileName);
        if (!streamFile) debugln("ERROR: No stream file open");
        else ESP_LOGI("", "File opened!");
    }

    void streamWrite(const char* buf, size_t len) {
        if (streamFile) streamFile.write((uint8_t*)buf, len);
        else debugln("ERROR: No stream file open");
    }

    size_t streamRead(char* buf, size_t len) {
        if (streamFile) {
            size_t i;

            for (i = 0; i<len; ++i) {
                if (!streamFile.available() || (i == len-1)) {
                    buf[i] = '\0';
                    break;
                } else {
                    buf[i] = streamFile.read();
                }
            }

            ESP_LOGI("", "File buf %s", buf);

            return i;
        } else {
            ESP_LOGI("", "ERROR: No stream file open");
            return 0;
        }
    }

    size_t streamReadUntil(char* buf, char delimiter, size_t max_len) {
        if (streamFile) {
            size_t i;
            char   c = 'x';

            for (i = 0; i<max_len; ++i) {
                if ((c == delimiter) || !streamFile.available() || (i == max_len-1)) {
                    buf[i] = '\0';
                    break;
                } else {
                    c      = streamFile.read();
                    buf[i] = c;
                }
            }

            return i;
        } else {
            debugln("ERROR: No stream file open");
            return 0;
        }
    }

    void streamClose() {
        streamFile.close();
    }

    bool streaming() {
        return streamFile;
    }

    size_t streamAvailable() {
        if (!streamFile) return 0;
        return streamFile.available();
    }
}