#define _CRT_DISABLE_PERFCRIT_LOCKS
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
using namespace std;

class ReadingManager {
public:
    void Read(int user_id, int page_count) {
        if (pages_by_users.count(user_id)) {
            auto old_count = pages_by_users[user_id];
            users_by_pages[old_count]--;
        }
        pages_by_users[user_id] = page_count;
        users_by_pages[page_count]++;
    }

    double Cheer(int user_id) const {
        if (!pages_by_users.count(user_id)) {
            return 0;
        }
        if (pages_by_users.size() == 1) {
            return 1;
        }

        auto read_by_user = pages_by_users.at(user_id);
        int total_users_less = 0;
        for (auto it : users_by_pages) {
            if (it.first >= read_by_user) {
                break;
            }
            total_users_less += it.second;
        }

        return double(total_users_less) / (pages_by_users.size() - 1);
    }

private:
    map<int, int> pages_by_users;    
    map<int, int> users_by_pages;   
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ReadingManager manager;
    int query_count;
    cin >> query_count;
    int query_id = 0;
    while (query_id < query_count) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;
        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        }
        else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
        ++query_id;
    }
    return 0;
}