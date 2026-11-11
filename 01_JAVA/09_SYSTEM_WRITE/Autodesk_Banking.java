// JAVA SYSTEM-WRITE — AUTODESK BANKING  (11-Nov Autodesk OA ka sawaal, CodeSignal jaisa: class ke method likho, tests pass karo)
// Neeche BankingSystemImpl me TERA OA wala code jaisa ka taisa hai. Chala, jo FAIL aaye wahan theek kar.
// Atke aur galti na dikhe to System.out.println se value dekh.
//
//   boolean createAccount(int timestamp, String accountId)
//        naya account, balance 0 -> true.  accountId pehle se hai -> false.
//
//   Optional<Integer> deposit(int timestamp, String accountId, int amount)
//        balance += amount, NAYA balance lautao.  account nahi -> Optional.empty()
//
//   Optional<Integer> transfer(int timestamp, String sourceAccountId, String targetAccountId, int amount)
//        source se ghatao, target me jodo, SOURCE ka naya balance lautao.
//        Optional.empty() jab: source ya target nahi · source aur target same · source me paisa kam
//        (balance ke BARABAR amount = theek hai, transfer hoga)
//
// Balance dekhne ka tareeka (getBalance nahi hai): deposit(t, id, 0) -> naya balance hi lautata hai.
//
// compile+run:  javac Autodesk_Banking.java && java Autodesk_Banking

import java.util.*;

public class Autodesk_Banking {

  interface BankingSystem {
    boolean createAccount(int timestamp, String accountId);
    Optional<Integer> deposit(int timestamp, String accountId, int amount);
    Optional<Integer> transfer(int timestamp, String sourceAccountId, String targetAccountId, int amount);
  }

  // ================= TERA CODE (OA me jo likha, waisa hi) =================
  static class BankingSystemImpl implements BankingSystem {

    public BankingSystemImpl() {
      // TODO: implement
    }

    // I am assuming there are only 1 thread;

    HashMap<String, Integer> mp = new HashMap<>();

    /**
     * Should create a new account with the given identifier if it
     * does not already exist.
     * Returns `true` if the account was successfully created or
     * `false` if an account with `accountId` already exists.
     */
    public boolean createAccount(int timestamp, String accountId) {
      if (mp.containsKey(accountId)) {
        return false;
      }
      mp.put(accountId, 0);
      return true;
    }

    /**
     * Should deposit the given `amount` of money to the specified
     * `accountId`.
     * Returns the total amount of money in the account after the
     * query has been processed.
     * If the specified account does not exist, should return
     * `Optional.empty()`.
     */

    public Optional<Integer> deposit(int timestamp, String accountId, int amount) {
      if (!mp.containsKey(accountId)) {
        return Optional.empty();
      }

      int getAmmount = mp.get(accountId);
      getAmmount += amount;

      mp.put(accountId, getAmmount);
      return Optional.of(getAmmount);
    }

    /**
     * Should transfer the given amount of money from account
     * `sourceAccountId` to account `targetAccountId`.
     * Returns the balance of `sourceAccountId` if the transfer was
     * successful or `Optional.empty()` otherwise.
     * * Returns `Optional.empty()` if `sourceAccountId` or
     * `targetAccountId` doesn't exist.
     * * Returns `Optional.empty()` if `sourceAccountId` and
     * `targetAccountId` are the same.
     * * Returns `Optional.empty()` if account `sourceAccountId`
     * has insufficient funds to perform the transfer.
     * Only an amount exceeding the current balance counts as
     * insufficient funds - a transfer for an amount exactly equal
     * to the current balance succeeds.
     * * Every operation returns that value directly: `true`,
     * `false`, a number, or `Optional.empty()`.
     * Do not wrap the result in an object.
     */

    // run("T7 ", () -> check(b.transfer(7, "A", "B", 300), Optional.of(400), "T7  transfer A->B 300"));
    public Optional<Integer> transfer(int timestamp, String sourceAccountId, String targetAccountId, int amount) {
      if (!mp.containsKey(sourceAccountId) || !mp.containsKey(targetAccountId)) {
        return Optional.empty();
      }
      else if(sourceAccountId.equals(targetAccountId)){
        return Optional.empty();
      }
      else if(mp.get(sourceAccountId) < amount){
        return Optional.empty();
      }

      int sourceAmount = mp.get(sourceAccountId);
      int targetAmount = mp.get(targetAccountId);

      sourceAmount -= amount;
      targetAmount += amount;

      mp.put(sourceAccountId, sourceAmount);
      mp.put(targetAccountId, targetAmount);

      return Optional.of(sourceAmount);
    }
    // ================= TERA CODE KHATAM =================

    // ---- test helper (ise mat chhed) ----
    static int pass = 0, total = 0;

    static void check(Object got, Object exp, String label) {
      total++;
      boolean ok = Objects.equals(got, exp);
      if (ok) pass++;
      System.out.println(label + " -> got " + got + " | exp " + exp + (ok ? "   PASS" : "   *** FAIL ***"));
    }

    static void run(String label, Runnable r) {
      try {
        r.run();
      } catch (Exception e) {
        total++;
        System.out.println(label + " -> CRASH " + e + "   *** FAIL ***");
      }
    }

    public static void main(String[] args) {
      BankingSystem b = new BankingSystemImpl();
      run("T1 ", () -> check(b.createAccount(1, "A"), true, "T1  create A"));
      run("T2 ", () -> check(b.createAccount(2, "B"), true, "T2  create B"));
      run("T3 ", () -> check(b.createAccount(3, "A"), false, "T3  create A dobara"));
      run("T4 ", () -> check(b.deposit(4, "A", 500), Optional.of(500), "T4  deposit A 500"));
      run("T5 ", () -> check(b.deposit(5, "A", 200), Optional.of(700), "T5  deposit A 200 aur"));
      run("T6 ", () -> check(b.deposit(6, "Z", 100), Optional.empty(), "T6  deposit, account nahi"));
      run("T7 ", () -> check(b.transfer(7, "A", "B", 300), Optional.of(400), "T7  transfer A->B 300"));
      run("T8 ", () -> check(b.deposit(8, "B", 0), Optional.of(300), "T8  B ko mila? (deposit 0 = balance)"));
      run("T9 ", () -> check(b.transfer(9, "A", "B", 1000), Optional.empty(), "T9  paisa kam"));
      run("T10", () -> check(b.deposit(10, "A", 0), Optional.of(400), "T10 fail ke baad A wahi"));
      run("T11", () -> check(b.transfer(11, "A", "A", 100), Optional.empty(), "T11 khud ko transfer"));
      run("T12", () -> check(b.transfer(12, "A", "Z", 100), Optional.empty(), "T12 target account nahi"));
      run("T13", () -> check(b.transfer(13, "Z", "A", 100), Optional.empty(), "T13 source account nahi"));
      run("T14", () -> check(b.transfer(14, "Z", "Y", 100), Optional.empty(), "T14 dono account nahi"));
      run("T15", () -> check(b.deposit(15, "A", 0), Optional.of(400), "T15 galat transfer ke baad A wahi"));
      run("T16", () -> check(b.transfer(16, "B", "A", 300), Optional.of(0), "T16 B poora khaali (barabar amount)"));
    run("T17", () -> check(b.deposit(17, "A", 0), Optional.of(700), "T17 A ko wapas mila"));
    run("T18", () -> check(b.deposit(18, "B", 0), Optional.of(0), "T18 B ab 0"));
      System.out.println("\n" + pass + "/" + total + " PASS");
    }
  }
}
