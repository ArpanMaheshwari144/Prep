// JAVA SYSTEM-WRITE — L1 WALLET  (CodeSignal / Airbnb jaisa: class ke method likho, tests pass karo)
// Atke aur galti na dikhe to System.out.println se value dekh.
//
// Ek in-memory wallet system. Account = id (String) + balance (int, shuru 0).
//
//   boolean createAccount(String id)
//        naya account, balance 0 -> true.  id pehle se hai -> false.
//
//   Optional<Integer> deposit(String id, int amount)
//        balance += amount, NAYA balance lautao.
//        account nahi  YA  amount <= 0  -> Optional.empty()
//
//   Optional<Integer> transfer(String fromId, String toId, int amount)
//        from se ghatao, to me jodo, FROM ka naya balance lautao.
//        Optional.empty() jab: koi bhi account nahi · fromId == toId · amount <= 0 · from me paisa kam
//        (empty wale case me kisi ka balance NAHI badalna chahiye)
//
//   Optional<Integer> getBalance(String id)
//        account hai -> balance.  nahi -> Optional.empty()
//
// compile+run:  javac L1_Wallet.java && java L1_Wallet

import java.util.*;

public class L1_Wallet {

    static class Wallet {
        // TODO: tu likh (field + 4 method)

        boolean createAccount(String id) {
            return false;
        }

        Optional<Integer> deposit(String id, int amount) {
            return Optional.empty();
        }

        Optional<Integer> transfer(String fromId, String toId, int amount) {
            return Optional.empty();
        }

        Optional<Integer> getBalance(String id) {
            return Optional.empty();
        }
    }

    // ---- test helper (ise mat chhed) ----
    static int pass = 0, total = 0;
    static void check(Object got, Object exp, String label) {
        total++;
        boolean ok = Objects.equals(got, exp);
        if (ok) pass++;
        System.out.println(label + " -> got " + got + " | exp " + exp + (ok ? "   PASS" : "   *** FAIL ***"));
    }

    public static void main(String[] args) {
        Wallet w = new Wallet();
        check(w.createAccount("A"), true, "T1  create A");
        check(w.createAccount("B"), true, "T2  create B");
        check(w.createAccount("A"), false, "T3  create A dobara");
        check(w.getBalance("A"), Optional.of(0), "T4  naya account balance 0");
        check(w.deposit("A", 500), Optional.of(500), "T5  deposit A 500");
        check(w.deposit("A", 200), Optional.of(700), "T6  deposit A 200 aur");
        check(w.deposit("Z", 100), Optional.empty(), "T7  deposit, account nahi");
        check(w.deposit("A", 0), Optional.empty(), "T8  deposit 0");
        check(w.deposit("A", -50), Optional.empty(), "T9  deposit negative");
        check(w.transfer("A", "B", 300), Optional.of(400), "T10 transfer A->B 300");
        check(w.getBalance("B"), Optional.of(300), "T11 B ko mila");
        check(w.transfer("A", "B", 1000), Optional.empty(), "T12 paisa kam");
        check(w.getBalance("A"), Optional.of(400), "T13 fail ke baad A wahi");
        check(w.transfer("A", "A", 100), Optional.empty(), "T14 khud ko transfer");
        check(w.transfer("A", "Z", 100), Optional.empty(), "T15 to account nahi");
        check(w.getBalance("A"), Optional.of(400), "T16 to-nahi ke baad A wahi");
        check(w.transfer("Z", "A", 100), Optional.empty(), "T17 from account nahi");
        check(w.transfer("B", "A", 0), Optional.empty(), "T18 transfer 0");
        check(w.transfer("B", "A", 300), Optional.of(0), "T19 B poora khaali");
        check(w.getBalance("A"), Optional.of(700), "T20 A ko wapas mila");
        check(w.getBalance("Z"), Optional.empty(), "T21 balance, account nahi");
        System.out.println("\n" + pass + "/" + total + " PASS");
    }
}
