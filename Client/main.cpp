#include "UserData.h"
#include "OrsApiClient.h"

void ShowRanking(const json& j)
{
    std::cout << "========== Ranking ==========" << std::endl;
    for (const auto& [key, value] : j.items()) {
        std::cout << std::format("{}st) {} / {}", key, value["user_name"].get<std::string>(), value["score"].get<int>()) << std::endl;
    };
    std::cout << "=============================" << std::endl;
}

int main()
{
    // UserDataを新規作成
    UserData userData = UserData("myname", 250);
    UserData userData1 = UserData("test1", 100);
    UserData userData2 = UserData("test2", 200);
    UserData userData3 = UserData("test3", 300);
    UserData userData4 = UserData("test4", 400);
    // UserDataをアップロード
    userData.UploadScore();
    userData1.UploadScore();
    userData2.UploadScore();
    userData3.UploadScore();
    userData4.UploadScore();

    // 自分の順位を取得
    ShowRanking(userData.GetMyRanking());

    // トップ3のランキングを取得
    ShowRanking(userData.GetTopRanking(3));

    // スコアを更新
    userData.UpdateScore(450);
    userData.UploadScore();

    // トップ3のランキングを取得
    ShowRanking(userData.GetTopRanking(3));
}
