// ============================================================
// RATE LIMITER + REDIS — C++ version (same kaam jo RateLimiterDemo.java karta)
// ============================================================
// Niyam: ek user (user:1) ko 60 sec me max 5 request. 6th se BLOCK.
// Redis se baat: "docker exec demo-redis redis-cli ..." command chala ke uska jawab padhte hain.
//
// CHALANE KA TAREEKA
//   Terminal 1 (Redis chalao, persistence BAND):
//     docker run -d --name demo-redis -p 6390:6379 redis:7 redis-server --save "" --appendonly no
//
//   Terminal 2 (ye program):
//     g++ RateLimiterDemo.cpp -o rl
//     ./rl
//     -> har ENTER = ek request   (q + ENTER = band)
//
//   Beech me Terminal 1 se khud maaro / wapas lao:
//     docker stop demo-redis
//     docker start demo-redis
//
//   Khatam hone pe:
//     docker rm -f demo-redis
// ============================================================

#include <bits/stdc++.h>
using namespace std;

const string KEY = "rl:user:1";
const int LIMIT = 5;
const int WINDOW_SEC = 60;

// Redis na mile to kya karein?  true = ALLOW (fail-open)  |  false = BLOCK (fail-closed)
const bool FAIL_OPEN = true;

// Redis ko command bhejo, jawab ka number lautao.
// Redis se baat na ho to "ok = false".
long long redis(string cmd, bool &ok)
{
    string full = "docker exec demo-redis redis-cli " + cmd + " 2>&1";
    FILE *pipe = popen(full.c_str(), "r");
    char buf[512];
    string reply = "";
    while (fgets(buf, sizeof(buf), pipe))
        reply += buf;
    pclose(pipe);

    // jawab number hai (jaise "6") to theek, warna Redis se baat nahi hui
    if (reply.empty() || !(isdigit(reply[0]) || reply[0] == '-'))
    {
        ok = false;
        cout << "  redis ka jawab: " << reply;
        return 0;
    }
    ok = true;
    return stoll(reply);
}

int main()
{
    int reqNo = 0;
    string line;
    cout << "ENTER dabao = ek request   (q + ENTER = band)\n";

    while (getline(cin, line))
    {
        if (line == "q")
            break;
        reqNo++;
        cout << "\n--- request " << reqNo << " ---\n";

        bool ok;
        long long count = redis("INCR " + KEY, ok);

        if (!ok)
        {
            cout << "REDIS SE BAAT NAHI HUI\n";
            if (FAIL_OPEN)
                cout << "FAIL_OPEN = true -> ALLOW (bina limit ke)\n";
            else
                cout << "FAIL_OPEN = false -> BLOCK (429)\n";
            continue;
        }

        cout << "INCR " << KEY << " -> count = " << count << "\n";

        if (count == 1)
        {
            redis("EXPIRE " + KEY + " " + to_string(WINDOW_SEC), ok);
            cout << "pehli request -> EXPIRE " << WINDOW_SEC << " sec laga\n";
        }

        long long ttl = redis("TTL " + KEY, ok);
        cout << "TTL baaki = " << ttl << " sec\n";

        if (count <= LIMIT)
            cout << "count " << count << " <= " << LIMIT << " -> ALLOW\n";
        else
            cout << "count " << count << " > " << LIMIT << " -> BLOCK (429)\n";
    }
    cout << "band.\n";
    return 0;
}
