// Copyright (c) 2023 @dheijl (danny.heijl@telenet.be)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <cctype>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include <WiFi.h>
#include <WifiClient.h>

#include "tftfunctions.h"

using std::string;
using std::vector;

static const constexpr char* MPD_CURRENTSONG = "currentsong\n";
static const constexpr char* MPD_STATUS = "status\n";
static const constexpr char* MPD_START = "play\n";
static const constexpr char* MPD_STOP = "stop\n";
static const constexpr char* MPD_CLEAR = "clear\n";
static const constexpr char* MPD_ADD = "add {}\n";

enum MpdResponseType {
    MpdOKType,
    MpdACKType,
    MpdErrorType,
};

enum MpdResponseKind {
    MpdUninitializedKind,
    MpdConnectKind,
    MpdCurrentSongKind,
    MpdStatusKind,
    MpdCommandType,
    MpdFailureKind,
};

class MpdResponse {
private:
    MpdResponseType ResponseType;
    string Response;

protected:
    MpdResponseKind ResponseKind;
    std::map<string, string> ResponseData;
    string getItem(const string& item)
    {
        auto it = this->ResponseData.find(item);
        if (it != this->ResponseData.end()) {
            return it->second;
        } else {
            return "";
        }
    }
    virtual void abstract() = 0;

public:
    MpdResponse(const string& response)
    {
        this->ResponseKind = MpdUninitializedKind;
        this->Response = response;
        auto p = response.find("ACK");
        if (p != string::npos) {
            if (p == 0) {
                this->ResponseType = MpdACKType;
            }
        } else {
            p = response.find("OK");
            if (p != string::npos) {
                if ((p == (response.length() - 3)) || (p == 0)) {
                    this->ResponseType = MpdOKType;
                }
            } else {
                this->ResponseType = MpdErrorType;
            }
        }
        // parse response into vector and then into map
        if (this->ResponseType == MpdOKType) {
            // build a vector of the response lines
            static const string delimiter = "\n";
            size_t pos_start = 0, pos_end, delim_len = delimiter.length();
            string token;
            vector<string> lines;
            while ((pos_end = this->Response.find(delimiter, pos_start)) != string::npos) {
                token = this->Response.substr(pos_start, pos_end - pos_start);
                pos_start = pos_end + delim_len;
                lines.push_back(token);
            }
            lines.push_back(this->Response.substr(pos_start));
            // now extract key-value pairs into the map
            for (auto line : lines) {
                if ((pos_start = line.find(": ")) != string::npos) {
                    auto key = line.substr(0, pos_start);
                    for (int i = 0; i < key.length(); ++i) {
                        key[i] = std::toupper(key[i]);
                    }
                    auto value = line.substr(pos_start + 2);
                    this->ResponseData[key] = value;
                }
            }
        }
    }
    string getResponseString()
    {
        return this->Response;
    }
    MpdResponseType getResponseType()
    {
        return this->ResponseType;
    }
    MpdResponseKind getResponseKind()
    {
        return this->ResponseKind;
    }
};

class MpdConnect : public MpdResponse {
private:
    string Version;
    void abstract() override { }

public:
    MpdConnect(const string& response)
        : MpdResponse(response)
    {
        this->ResponseKind = MpdConnectKind;
        this->Version = this->getResponseString();
        int p = this->Version.find('\n');
        if (p > 0) {
            this->Version.erase(p, 1);
        }
    }
    string getVersion()
    {
        return this->Version;
    }
};

class MpdCurrentSong : public MpdResponse {
private:
    void abstract() override { }

public:
    MpdCurrentSong(const string& response)
        : MpdResponse(response)
    {
        if (this->getResponseType() == MpdOKType) {
            this->ResponseKind = MpdCurrentSongKind;
        } else {
            this->ResponseKind = MpdFailureKind;
        }
    }
    string getFile()
    {
        return this->getItem("FILE");
    }
    string getName()
    {
        return this->getItem("NAME");
    }
    string getTitle()
    {
        return this->getItem("TITLE");
    }
    string getArtist()
    {
        string artist = this->getItem("ARTIST");
        if (artist.empty()) {
            artist = this->getItem("ALBUMARTIST");
        }
        return artist;
    }
    string getAlbum()
    {
        return this->getItem("ALBUM");
    }
};

class MpdStatus : public MpdResponse {
private:
    void abstract() override { }

public:
    MpdStatus(const string& response)
        : MpdResponse(response)
    {
        if (this->getResponseType() == MpdOKType) {
            this->ResponseKind = MpdStatusKind;
        } else {
            this->ResponseKind = MpdFailureKind;
        }
    }
    string getState()
    {
        return this->getItem("STATE");
    }
};

class MpdSimpleCommand : public MpdResponse {
private:
    void abstract() override { }

public:
    MpdSimpleCommand(const string& response)
        : MpdResponse(response)
    {
        if (this->getResponseType() == MpdOKType) {
            this->ResponseKind = MpdCommandType;
        } else {
            this->ResponseKind = MpdFailureKind;
        }
    }
    string GetResult()
    {
        return this->getResponseType() == MpdOKType ? "OK" : "ERROR ";
    }
};

class MpdConnection {
private:
    WiFiClient Client;
    string read_data()
    {
        int n = 5000;
        while (n-- > 0) {
            vTaskDelay(1);
            if (Client.available()) {
                break;
            }
        }
        if (n <= 0) {
            tft_println_error("no response");
            return string();
        }
        uint8_t buf[4096];
        n = Client.read(buf, sizeof(buf));
        if (n > 0) {
            string data((char*)&buf[0], n);
            return data;
        } else {
            return string("");
        }
    }

protected:
public:
    bool Connect(const char* host, int port)
    {
        if (Client.connect(host, port)) {
            tft_println("CON MPD @" + String(host) + ":" + String(port));
            string data = read_data();
            if (data.length() == 0) {
                return false;
            }
            MpdConnect con(data);
            String v = String(con.getVersion().c_str());
            if (v.indexOf("OK") < 0) {
                tft_println_error(v);
                return false;
            }
            return true;
        } else {
            tft_println_error("MPD Connection failed");
            return false;
        }
    }
    void Disconnect()
    {
        tft_println("Disconnect MPD");
        Client.stop();
    }

    bool GetStatus()
    {
        Client.write(MPD_STATUS);
        string data = read_data();
        if (data.length() == 0) {
            return false;
        }
        MpdStatus mpd_status(data);
        tft_print("MPD status: ");
        tft_println_highlight(String(mpd_status.getState().c_str()));
        return true;
    }

    bool IsPlaying()
    {
        Client.write(MPD_STATUS);
        string data = read_data();
        if (data.length() == 0) {
            return false;
        }
        MpdStatus mpd_status(data);
        auto status = mpd_status.getState();
        tft_print("MPD status: ");
        tft_println_highlight(String(status.c_str()));
        return status.compare("play") == 0;
    }

    bool GetCurrentSong()
    {
        Client.write(MPD_CURRENTSONG);
        string data = read_data();
        if (data.length() == 0) {
            return false;
        }
        MpdCurrentSong mpd_cs(data);
        String file = String(mpd_cs.getFile().c_str());
        int p = file.lastIndexOf('/');
        file = file.substring(p + 1);
        tft_println(file);
        string name = mpd_cs.getName();
        if (!name.empty()) {
            tft_println_highlight(name.c_str());
        }
        string title = mpd_cs.getTitle();
        if (!title.empty()) {
            tft_println_highlight(title.c_str());
        }
        string artist = mpd_cs.getArtist();
        if (!artist.empty()) {
            tft_println_highlight(artist.c_str());
        }
        return true;
    }

    bool Stop()
    {
        Client.write(MPD_STOP);
        string data = read_data();
        if (data.length() == 0) {
            return false;
        }
        MpdSimpleCommand mpd_command(data);
        tft_println(mpd_command.GetResult().c_str());
        return true;
    }

    bool Play()
    {
        Client.write(MPD_START);
        string data = read_data();
        if (data.length() == 0) {
            return false;
        }
        MpdSimpleCommand mpd_command(data);
        tft_println(mpd_command.GetResult().c_str());
        return true;
    }

    bool Clear()
    {
        Client.write(MPD_CLEAR);
        string data = read_data();
        if (data.length() == 0) {
            return false;
        }
        MpdSimpleCommand mpd_command(data);
        tft_println(mpd_command.GetResult().c_str());
        return true;
    }

    bool Add_Url(const char* url)
    {
        string add_cmd(MPD_ADD);
        int pos = add_cmd.find("{}");
        add_cmd.replace(pos, 2, url);
        tft_println(add_cmd.c_str());
        Client.write(add_cmd.c_str(), add_cmd.length());
        string data = read_data();
        if (data.length() == 0) {
            return false;
        }
        MpdSimpleCommand mpd_command(data);
        tft_println(mpd_command.GetResult().c_str());
        return true;
    }
};

class MPD_Client {
private:
    MpdConnection con;
    void show_player(MPD_PLAYER& player);

public:
    void show_mpd_status();
    void toggle_mpd_status();
    void play_favourite(const FAVOURITE& fav);
};

extern MPD_Client& mpd;
