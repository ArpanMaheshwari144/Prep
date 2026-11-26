// SYSTEM-WRITE — AUTODESK WORKING HOURS  (26-Nov Autodesk retake ka sawaal, CodeSignal, C++ me diya)
// Class ke method likho, neeche ke tests pass karo. Sirf Level 1 ka poora sawaal hai (OA me yahi khula tha).
// Atke aur galti na dikhe to har return se pehle cout me RAASTE ka naam daal ("INVALID nahi mila", "ENTER", "LEAVE").
//
// ★ OA ME KYA GADBAD HUI (26-Nov): COMPILE ERROR aa gaya aur pakad nahi paaya, isliye pata hi nahi chala kitne test
//   pass hue, kitne nahi. Compile error me sabse PEHLI "error:" wali line padho: usme line number + kya galat, baaki
//   neeche ki lines aksar usi ek galti ki wajah se aati hain. Ek theek karo, phir dobara compile.
//
// ================= SAWAAL (Level 1) =================
// Office me contract workers aate-jaate hain, unka time register karna hai.
//
//   bool AddWorker(worker_id, position, compensation)
//        naya worker jodo (position + compensation = har time unit ka paisa) -> true.
//        worker_id pehle se hai -> kuch mat badlo, false.
//
//   string Register(worker_id, timestamp)
//        worker nahi -> "invalid_request" (aur kuch nahi hota, worker bante bhi nahi).
//        worker office ke BAHAR hai -> ye ANDAR aane ka time.  ANDAR hai -> ye BAHAR jaane ka time.
//        dono me "registered" lautao.
//        Ek hi timestamp pe do baar Register = 0 lambi session, wo bhi poori session maani jaati (0 jodti, skip nahi).
//
//   optional<int> Get(worker_id)
//        sirf POORI hui sessions ka total time. Andar gaya par nikla nahi = wo session nahi ginni.
//        worker nahi -> nullopt.  worker hai par koi session nahi -> 0.
//
// Dhyan: alag-alag worker ke timestamp aage-peeche aa sakte (bob 60, phir alice 10). Ek hi worker ke hamesha badhte.
//
// Aage ke level (OA me jo likha tha, detail tab khulti):
//   L2 time ke statistics · L3 per-worker overtime rule (ek session threshold se zyada to extra time double paisa) +
//   ek period ki salary · L4 do time-tracking system ke worker records MERGE (sessions jod ke salary).
//
// compile+run:  g++ -std=c++17 Autodesk_WorkingHours.cpp -o wh && ./wh

#include <bits/stdc++.h>
using namespace std;

class WorkingHoursRegister {
public:
    bool AddWorker(const string& worker_id, const string& position, int compensation) {
        // TERA CODE
        return false;
    }

    string Register(const string& worker_id, int timestamp) {
        // TERA CODE
        return "";
    }

    optional<int> Get(const string& worker_id) {
        // TERA CODE
        return nullopt;
    }
};

// ================= TESTS (inhe mat chhedna) =================
int pass_cnt = 0, total = 0;
template <class A, class B>
void check(const string& name, const A& got, const B& want) {
    total++;
    if (got == want) { pass_cnt++; cout << "PASS  " << name << "\n"; }
    else cout << "FAIL  " << name << "\n";
}
optional<int> none = nullopt;

int main() {
    {   // sawaal wali misaal
        WorkingHoursRegister r;
        check("ex add alice", r.AddWorker("alice", "Engineer", 100), true);
        check("ex add bob", r.AddWorker("bob", "Manager", 200), true);
        check("ex add alice dobara", r.AddWorker("alice", "Sales", 150), false);
        check("ex alice in 10", r.Register("alice", 10), string("registered"));
        check("ex alice out 40", r.Register("alice", 40), string("registered"));
        check("ex alice in 50", r.Register("alice", 50), string("registered"));
        check("ex alice out 80", r.Register("alice", 80), string("registered"));
        check("ex bob in 60", r.Register("bob", 60), string("registered"));
        check("ex bob out 90", r.Register("bob", 90), string("registered"));
        check("ex bob in 95 (khuli)", r.Register("bob", 95), string("registered"));
        check("ex carol register", r.Register("carol", 100), string("invalid_request"));
        check("ex get alice = 60", r.Get("alice"), optional<int>(60));
        check("ex get bob = 30", r.Get("bob"), optional<int>(30));
        check("ex get carol = nullopt", r.Get("carol"), none);
    }
    {   // naya worker, koi session nahi -> 0 (nullopt nahi)
        WorkingHoursRegister r;
        r.AddWorker("dev", "Intern", 10);
        check("koi session nahi = 0", r.Get("dev"), optional<int>(0));
    }
    {   // sirf andar gaya, nikla nahi -> 0
        WorkingHoursRegister r;
        r.AddWorker("dev", "Intern", 10);
        r.Register("dev", 5);
        check("sirf khuli session = 0", r.Get("dev"), optional<int>(0));
    }
    {   // same timestamp pe in + out = 0 wali poori session, phir agli session normal
        WorkingHoursRegister r;
        r.AddWorker("eve", "QA", 50);
        check("eve in 5", r.Register("eve", 5), string("registered"));
        check("eve out 5 (same time)", r.Register("eve", 5), string("registered"));
        check("same time session = 0", r.Get("eve"), optional<int>(0));
        r.Register("eve", 7);   // ye ANDAR hona chahiye (pichhli session band ho chuki)
        r.Register("eve", 10);  // BAHAR
        check("phir 7-10 = 3", r.Get("eve"), optional<int>(3));
    }
    {   // invalid register se worker nahi banta
        WorkingHoursRegister r;
        check("zed register = invalid", r.Register("zed", 1), string("invalid_request"));
        check("zed get = nullopt", r.Get("zed"), none);
        check("zed ab add = true", r.AddWorker("zed", "Ops", 20), true);
        check("zed ab get = 0", r.Get("zed"), optional<int>(0));
    }
    {   // alag worker ke timestamp aage-peeche
        WorkingHoursRegister r;
        r.AddWorker("bob", "Manager", 200);
        r.AddWorker("alice", "Engineer", 100);
        r.Register("bob", 100);
        r.Register("alice", 10);
        r.Register("alice", 30);
        r.Register("bob", 150);
        check("alice 10-30 = 20", r.Get("alice"), optional<int>(20));
        check("bob 100-150 = 50", r.Get("bob"), optional<int>(50));
    }
    {   // naam me space (position / id me space aa sakta)
        WorkingHoursRegister r;
        check("space wala naam add", r.AddWorker("john smith", "Sales Lead", 70), true);
        r.Register("john smith", 1);
        r.Register("john smith", 4);
        check("space wala naam get = 3", r.Get("john smith"), optional<int>(3));
        check("john (aadha naam) = nullopt", r.Get("john"), none);
    }
    cout << "\n" << pass_cnt << "/" << total << " pass\n";
}
