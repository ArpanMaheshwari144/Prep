// DESIGN PATTERN PRACTICE — DP2 Factory (Claude ne likha, padh ke chala)
// Idea: caller sirf TYPE bolta ("savings"), factory decide karti kaunsi class banegi.
//       caller ko SavingsAccount / CurrentAccount ka naam pata hi nahi -> sirf Account dikhta.
//
// 2 version:
//   V1  Simple Factory      switch(type)        -> naya type = switch me ek case jodo
//   V2  Map Registry        Map<type, banane wala> -> naya type = register(), factory ka code CHHUNA nahi (OCP)
//
// compile+run:  javac DP2_Factory.java && java DP2_Factory

import java.util.*;
import java.util.concurrent.*;
import java.util.function.*;

// ─── sabka common type (caller sirf isko jaanta) ───
interface Account {
    String type();
    double interestRate();
}

class SavingsAccount implements Account {
    public String type() { return "SAVINGS"; }
    public double interestRate() { return 4.0; }
}

class CurrentAccount implements Account {
    public String type() { return "CURRENT"; }
    public double interestRate() { return 0.0; }
}

class FixedDeposit implements Account {
    public String type() { return "FD"; }
    public double interestRate() { return 7.0; }
}

// ─── V1: SIMPLE FACTORY ───
class AccountFactory {
    static Account create(String type) {
        switch (type.toLowerCase()) {
            case "savings": return new SavingsAccount();
            case "current": return new CurrentAccount();
            case "fd":      return new FixedDeposit();
            default:        throw new IllegalArgumentException("Unknown account type: " + type);
        }
    }
}

// ─── V2: MAP REGISTRY (naya type bina factory chhede) ───
class AccountRegistry {
    // ConcurrentHashMap: register() alag thread se bhi ho sakta
    private static final Map<String, Supplier<Account>> REGISTRY = new ConcurrentHashMap<>();

    static {
        REGISTRY.put("savings", SavingsAccount::new);
        REGISTRY.put("current", CurrentAccount::new);
        REGISTRY.put("fd",      FixedDeposit::new);
    }

    static void register(String type, Supplier<Account> maker) {
        REGISTRY.put(type.toLowerCase(), maker);
    }

    static Account create(String type) {
        Supplier<Account> maker = REGISTRY.get(type.toLowerCase());
        if (maker == null) throw new IllegalArgumentException("Unknown account type: " + type);
        return maker.get();
    }
}

// naya type — AccountRegistry ka code chhuye bina jud jaayega
class CryptoAccount implements Account {
    public String type() { return "CRYPTO"; }
    public double interestRate() { return 0.5; }
}

public class DP2_Factory {
    public static void main(String[] args) {
        // V1
        Account a = AccountFactory.create("savings");
        Account b = AccountFactory.create("FD");                 // case ka farak nahi padta
        System.out.println("V1 savings -> " + a.type() + " " + a.interestRate() + "%");
        System.out.println("V1 fd      -> " + b.type() + " " + b.interestRate() + "%");
        try {
            AccountFactory.create("gold");
        } catch (IllegalArgumentException e) {
            System.out.println("V1 gold    -> " + e.getMessage() + "   (null nahi, saaf error)");
        }

        // V2
        AccountRegistry.register("crypto", CryptoAccount::new);  // factory ka code nahi chhua
        Account c = AccountRegistry.create("crypto");
        System.out.println("V2 crypto  -> " + c.type() + " " + c.interestRate() + "%   (register se juda)");

        // caller ne kahin bhi "new SavingsAccount()" nahi likha
        System.out.println("caller ke paas sirf Account type hai: " + (a instanceof Account));
    }
}
