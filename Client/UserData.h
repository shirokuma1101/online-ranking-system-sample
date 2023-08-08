#pragma once

#include "OrsApiClient.h"
#pragma comment(lib, "Rpcrt4.lib")

class UserData
{
public:

    static constexpr char URL[] = "192.168.1.15:5000";

    UserData(std::string_view user_name, int score) {
        uuid        = GetUuid();
        userName    = user_name;
        this->score = score;
    }

    void UpdateScore(int score) {
        this->score = score;
    }

    void UploadScore() {
        json params;
        params["uuid"]      = uuid;
        params["user_name"] = userName;
        params["score"]     = score;
        ors_api_client::Request(URL, ors_api_client::Method::POST, params);
    }

    json GetMyRanking() {
        json params;
        params["uuid"] = uuid;
        return ors_api_client::Request(URL, ors_api_client::Method::GET, params);
    }

    json GetTopRanking(int limit = 3) {
        json params;
        params["limit"] = std::to_string(limit);
        return ors_api_client::Request(URL, ors_api_client::Method::GET, params);
    }

private:

    std::string GetUuid() {
        GUID guid = GUID_NULL;
        if (FAILED(CoCreateGuid(&guid))) {
            return "";
        }

        RPC_WSTR str;
        if (RPC_S_OK == UuidToString(&guid, &str)) {
            return wide_to_sjis((PWCHAR)str);
        }
        return "";
    }

    std::string uuid;
    std::string userName;
    int         score;

};
