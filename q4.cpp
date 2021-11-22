#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<bitset>
using namespace std;

pair<int, int> findComma(string s) {
    int a = s.find_first_of(",");
    int b = s.find_last_of(",");
    return make_pair(a, b);
}

string to_binary(string ip) {
    //文字列操作してアドレスを32bitの2進数に変換する関数
    //input ip ipアドレス(ネットワークプレフィックス長なし)　例:192.168.1.1
    //output ipアドレスを2進数32bitで表した文字列(.区切りなし)
    vector<string> level;
    int st = 0;
    int now = ip.find_first_of(".");

    while(st < ip.size()) {
        string sub(ip, st, now-st);
        level.push_back(sub);
        st = now+1;
        now = ip.find_first_of(".", st);

        if(now == string::npos) {
            now = ip.size();
        }
    }

    string res = "";
    for(string s : level) {
        bitset<8> bs(stoi(s));
        res += bs.to_string();
    }
    return res;
}

string to_decimal(string bs) {
    //10進数のアドレス表記に戻す関数
    string res = "";
    for(int i=0; i<4; ++i) {
        if(i) res += ".";
        string s = bs.substr(i*8, 8);
        bitset<8> b(s);
        unsigned long x = b.to_ulong();
        res += to_string((int)x);
    }
    return res;
}

string makeSubnet(string ip) {
    //サブネットマスクの長さを取得
    int x = ip.find_first_of("/");
    string address = ip.substr(0, x);
    int len = stoi(ip.substr(x+1));
    string binaryaddress = to_binary(address);

    bitset<32> mask;
    mask.set(); //全ビット1にする
    bitset<32> bs(binaryaddress);

    bitset<32> subnet = bs & (mask << (32-len));

    string res = to_decimal(subnet.to_string());

    return res;
}

int main(void){
    int q, N;
    cin >> q >> N;
    vector<string> logtime(q), ip(q), ping(q), subnet(q);
    for(int i=0; i<q; ++i) {
        string str;
        cin >> str;
        pair<int, int> p = findComma(str);
        logtime[i] = str.substr(0, p.first);
        ip[i] = str.substr(p.first+1, p.second-p.first-1);
        ping[i] = str.substr(p.second+1);
        //サブネットアドレスの導出
        subnet[i] = makeSubnet(ip[i]);
    }


    map<string, int> cnt;
    map<string, string> sttime;

    for(int i=0; i<q; ++i) {
        if(ping[i] == "-") {
            //故障(pingがタイムアウト)
            if(cnt[subnet[i]] == 0) {
                //新規故障
                sttime[subnet[i]] = logtime[i];
                cnt[subnet[i]]++;
            } else {
                //故障継続
                cnt[subnet[i]]++;
            }
        }
        else {
            //pingが正常に飛んでいる
            if(cnt[subnet[i]] >= N) {
                //復旧(故障期間出力)
                cout << "サブネットアドレス : 故障開始時間 : 復旧時間 -> ";
                cout << subnet[i] << " : " << sttime[subnet[i]] << " : " << logtime[i] << endl;
                cnt[subnet[i]] = 0;
            } else {
                //N回未満で復旧
                cnt[subnet[i]] = 0;
            }
        }
    }

    //復旧していないサーバーがあったらそれも出力
    for(auto [key, value] : cnt) {
        if(value >= N) {
            cout << "サブネット : " << key << " は " << sttime[key] << " から復旧していません!" << endl;
        }
    }

    return 0;
}