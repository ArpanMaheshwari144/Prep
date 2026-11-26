// JAVA SYSTEM-WRITE — AUTODESK WORKING HOURS  (26-Nov Autodesk retake ka sawaal, CodeSignal; OA me C++ me diya, yahan Java)
// Neeche WorkingHoursRegister ke 3 khaali method likho, main chala ke dekho kitne PASS.
// Atke aur galti na dikhe to har return se pehle System.out.println me RAASTE ka naam ("INVALID nahi mila", "ENTER", "LEAVE").
//
// ★ OA ME KYA GADBAD HUI (26-Nov, score 200/600): COMPILE ERROR aa gaya aur pakad nahi paaya, isliye pata hi nahi chala kitne test
//   pass hue, kitne nahi. Compile error me sabse PEHLI "error:" wali line padho: usme line number + kya galat, baaki
//   neeche ki lines aksar usi ek galti ki wajah se aati hain. Ek theek karo, phir dobara compile.
//
// ================= SAWAAL (Level 1) =================
// Office me contract workers aate-jaate hain, unka time register karna hai.
//
//   boolean addWorker(String workerId, String position, int compensation)
//        naya worker jodo (position = sirf naam jaise "Engineer"; compensation = har time unit ka paisa) -> true.
//        workerId pehle se hai -> kuch mat badlo, false.
//        (L1 me position / compensation kahin use nahi hote, par save rakho: L3 salary me compensation chahiye)
//
//   String register(String workerId, int timestamp)
//        worker nahi -> "invalid_request" (aur kuch nahi hota, worker bante bhi nahi).
//        worker office ke BAHAR hai -> ye ANDAR aane ka time.  ANDAR hai -> ye BAHAR jaane ka time.
//        dono me "registered" lautao.
//        Ek hi timestamp pe do baar register = 0 lambi session, wo bhi poori session maani jaati (0 jodti, skip nahi).
//
//   Optional<Integer> get(String workerId)
//        sirf POORI hui sessions ka total time. Andar gaya par nikla nahi = wo session nahi ginni.
//        worker nahi -> Optional.empty().  worker hai par koi session nahi -> 0.
//
// Dhyan: alag-alag worker ke timestamp aage-peeche aa sakte (bob 60, phir alice 10). Ek hi worker ke hamesha badhte.
//
// Aage ke level (OA me jo likha tha, detail tab khulti):
//   L2 time ke statistics · L3 per-worker overtime rule (ek session threshold se zyada to extra time double paisa) +
//   ek period ki salary · L4 do time-tracking system ke worker records MERGE (sessions jod ke salary).
//
// IntelliJ: is file me main pe Run.   Terminal:  javac Autodesk_WorkingHours.java && java Autodesk_WorkingHours

import java.util.*;

public class Autodesk_WorkingHours {

  // ================= TERA CODE =================
  static class WorkingHoursRegister {

    public boolean addWorker(String workerId, String position, int compensation) {
      // TERA CODE
      return false;
    }

    public String register(String workerId, int timestamp) {
      // TERA CODE
      return "";
    }

    public Optional<Integer> get(String workerId) {
      // TERA CODE
      return Optional.empty();
    }
  }

  // ================= TESTS (inhe mat chhedna) =================
  static int pass = 0, total = 0;

  static void check(String name, Object got, Object want) {
    total++;
    if (Objects.equals(got, want)) { pass++; System.out.println("PASS  " + name); }
    else System.out.println("FAIL  " + name + "   (aaya: " + got + ", chahiye: " + want + ")");
  }

  public static void main(String[] args) {
    Optional<Integer> none = Optional.empty();
    {   // sawaal wali misaal
      WorkingHoursRegister r = new WorkingHoursRegister();
      check("ex add alice", r.addWorker("alice", "Engineer", 100), true);
      check("ex add bob", r.addWorker("bob", "Manager", 200), true);
      check("ex add alice dobara", r.addWorker("alice", "Sales", 150), false);
      check("ex alice in 10", r.register("alice", 10), "registered");
      check("ex alice out 40", r.register("alice", 40), "registered");
      check("ex alice in 50", r.register("alice", 50), "registered");
      check("ex alice out 80", r.register("alice", 80), "registered");
      check("ex bob in 60", r.register("bob", 60), "registered");
      check("ex bob out 90", r.register("bob", 90), "registered");
      check("ex bob in 95 (khuli)", r.register("bob", 95), "registered");
      check("ex carol register", r.register("carol", 100), "invalid_request");
      check("ex get alice = 60", r.get("alice"), Optional.of(60));
      check("ex get bob = 30", r.get("bob"), Optional.of(30));
      check("ex get carol = empty", r.get("carol"), none);
    }
    {   // naya worker, koi session nahi -> 0 (empty nahi)
      WorkingHoursRegister r = new WorkingHoursRegister();
      r.addWorker("dev", "Intern", 10);
      check("koi session nahi = 0", r.get("dev"), Optional.of(0));
    }
    {   // sirf andar gaya, nikla nahi -> 0
      WorkingHoursRegister r = new WorkingHoursRegister();
      r.addWorker("dev", "Intern", 10);
      r.register("dev", 5);
      check("sirf khuli session = 0", r.get("dev"), Optional.of(0));
    }
    {   // same timestamp pe in + out = 0 wali poori session, phir agli session normal
      WorkingHoursRegister r = new WorkingHoursRegister();
      r.addWorker("eve", "QA", 50);
      check("eve in 5", r.register("eve", 5), "registered");
      check("eve out 5 (same time)", r.register("eve", 5), "registered");
      check("same time session = 0", r.get("eve"), Optional.of(0));
      r.register("eve", 7);   // ye ANDAR hona chahiye (pichhli session band ho chuki)
      r.register("eve", 10);  // BAHAR
      check("phir 7-10 = 3", r.get("eve"), Optional.of(3));
    }
    {   // invalid register se worker nahi banta
      WorkingHoursRegister r = new WorkingHoursRegister();
      check("zed register = invalid", r.register("zed", 1), "invalid_request");
      check("zed get = empty", r.get("zed"), none);
      check("zed ab add = true", r.addWorker("zed", "Ops", 20), true);
      check("zed ab get = 0", r.get("zed"), Optional.of(0));
    }
    {   // alag worker ke timestamp aage-peeche
      WorkingHoursRegister r = new WorkingHoursRegister();
      r.addWorker("bob", "Manager", 200);
      r.addWorker("alice", "Engineer", 100);
      r.register("bob", 100);
      r.register("alice", 10);
      r.register("alice", 30);
      r.register("bob", 150);
      check("alice 10-30 = 20", r.get("alice"), Optional.of(20));
      check("bob 100-150 = 50", r.get("bob"), Optional.of(50));
    }
    {   // naam me space (position / id me space aa sakta)
      WorkingHoursRegister r = new WorkingHoursRegister();
      check("space wala naam add", r.addWorker("john smith", "Sales Lead", 70), true);
      r.register("john smith", 1);
      r.register("john smith", 4);
      check("space wala naam get = 3", r.get("john smith"), Optional.of(3));
      check("john (aadha naam) = empty", r.get("john"), none);
    }
    System.out.println("\n" + pass + "/" + total + " pass");
  }
}
