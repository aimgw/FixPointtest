#include<iostream>
#include<string>
#include<map>
#include<vector>
using namespace std;

pair<int, int> findComma(string s) {
    int a = s.find_first_of(",");
    int b = s.find_last_of(",");
    return make_pair(a, b);
} 

int main(void){
    int q, N;
    cin >> q >> N;
    vector<string> logtime(q), ip(q), ping(q);
    for(int i=0; i<q; ++i) {
        string str;
        cin >> str;
        pair<int, int> p = findComma(str);
        logtime[i] = str.substr(0, p.first);
        ip[i] = str.substr(p.first+1, p.second-p.first-1);
        ping[i] = str.substr(p.second+1);
    }
    
    map<string, int> cnt;
    map<string, string> section;

    for(int i=0; i<q; ++i) {
        if(ping[i] == "-") {
            //故障(pingがタイムアウト)
            if(cnt[ip[i]] == 0) {
                //新規故障
                section[ip[i]] = logtime[i];
                cnt[ip[i]]++;
            } else {
                //故障継続
                cnt[ip[i]]++;
            }
        }
        else {
            //pingが正常に飛んでいる
            if(cnt[ip[i]] >= N) {
                //復旧(故障期間出力)
                cout << "サーバアドレス : 故障開始時間 : 復旧時間 -> ";
                cout << ip[i] << " : " << section[ip[i]] << " : " << logtime[i] << endl;
                cnt[ip[i]] = 0;
            } else {
                //N回未満で復旧
                cnt[ip[i]] = 0;
            }
        }
    }

    //復旧していないサーバーがあったらそれも出力
    for(auto [key, value] : cnt) {
        if(value >= N) {
            cout << "サーバー : " << key << " は " << section[key] << " から復旧していません!" << endl;
        }
    }

    return 0;
}