#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<queue>
using namespace std;

pair<int, int> findComma(string s) {
    int a = s.find_first_of(",");
    int b = s.find_last_of(",");
    return make_pair(a, b);
}

bool check(int tot, int log_size, int m, int t) {
    //過負荷状態か判定する関数
    m = min(m, log_size);
    return tot >= t * m; //もしかしたらオーバーフローがこわいかも...
}

int main(void){
    int q, m, t;
    cin >> q >> m >> t;
    vector<string> logtime(q), ip(q), ping(q);
    for(int i=0; i<q; ++i) {
        string str;
        cin >> str;
        pair<int, int> p = findComma(str);
        logtime[i] = str.substr(0, p.first);
        ip[i] = str.substr(p.first+1, p.second-p.first-1);
        ping[i] = str.substr(p.second+1);
    }
    
    map<string, queue<int>> pinglist;   //直近m個のping値を保存する連想配列
    map<string, queue<string>> timelist;    //過負荷状態の期間を記録する連想配列
    map<string, int> tot;   //ping値の和を保存しておく連想配列
    map<string, int> cnt;   //過負荷期間をカウントする連想配列
    map<string, string> sttime;    //過負荷期間の開始時間を記録する連想配列

    for(int i=0; i<q; ++i) {
        if(ping[i] == "-") continue;  //タイムアウトしているケースはこの問題では取り扱わない
        if(pinglist[ip[i]].size() == m) {
            //すでにm個のデータを読み込んでいる場合は先頭の要素を削除
            tot[ip[i]] -= pinglist[ip[i]].front();
            pinglist[ip[i]].pop();
            timelist[ip[i]].pop();
        }
        //要素の追加
        int nowping = stoi(ping[i]);
        pinglist[ip[i]].push(nowping);
        timelist[ip[i]].push(logtime[i]);
        tot[ip[i]] += nowping;

        //現在過負荷状態か判定
        if(check(tot[ip[i]], (int)pinglist[ip[i]].size(), m, t)) {
            //過負荷状態
            if(cnt[ip[i]] == 0) {
                //新規過負荷状態
                sttime[ip[i]] = logtime[i];
                cnt[ip[i]]++;
            } else {
                //継続過負荷状態
                cnt[ip[i]]++;
            }
        } else {
            //過負荷状態ではない
            if(cnt[ip[i]] != 0) {
                //過負荷状態->通常なのでその期間を出力
                cout << "サーバアドレス : 過負荷状態開始時間 : 復旧時間 -> ";
                cout << ip[i] << " : " << sttime[ip[i]] << " : " << logtime[i] << endl;
                cnt[ip[i]] = 0;
            }
        }
    }
    
    //復旧していないものがあれば出力
    for(auto [key, value] : cnt) {
        if(value != 0) {
            //過負荷状態が継続
            cout << "サーバ " << key << " は過負荷状態が " << sttime[key] << " から継続中!" << endl;
        }
    }

    return 0;
}