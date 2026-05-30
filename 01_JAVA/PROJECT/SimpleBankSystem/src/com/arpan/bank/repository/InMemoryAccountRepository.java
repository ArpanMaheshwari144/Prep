package com.arpan.bank.repository;

import com.arpan.bank.model.Account;
import java.util.*;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    AccountRepository ka CONCRETE implementation
//    HashMap mein accounts store karta
//    Production mein → MySQL/Mongo wala class aayega yahaan
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL:
//    AccountRepository (interface — contract)
//               │
//               │ implements
//               ▼
//    ┌─────────────────────────────┐
//    │ InMemoryAccountRepository    │  ← TU YAHAN
//    │                                │
//    │ HashMap<String, Account>     │
//    │   "S1" → SavingsAccount       │
//    │   "C1" → CurrentAccount       │
//    │   "F1" → FixedDeposit         │
//    └─────────────────────────────┘
//
// CONCEPTS IN THIS FILE:
//    1. HashMap (Collections)
//       accountId → Account mapping
//       O(1) lookup by ID
//
//    2. Optional.ofNullable()
//       map.get() returns null if missing
//       ofNullable wraps → null-safe
//
//    3. Defensive Copy
//       new ArrayList<>(accounts.values())
//       Caller modify kare → internal HashMap safe
//
//    4. @Override
//       Interface contract enforce
//
// WHY DEFENSIVE COPY?
//    WITHOUT: caller.clear() ne internal HashMap empty kar diya
//    WITH:    new ArrayList<>(...) returns NEW list
//             Caller modify only on copy
//
// HashMap vs TreeMap — WHY HashMap?
//    ┌──────────────┬─────────────┬─────────────┐
//    │  Operation   │  HashMap    │  TreeMap    │
//    ├──────────────┼─────────────┼─────────────┤
//    │ put()        │  O(1) avg   │  O(log n)   │
//    │ get()        │  O(1) avg   │  O(log n)   │
//    │ remove()     │  O(1) avg   │  O(log n)   │
//    └──────────────┴─────────────┴─────────────┘
//
//    HashMap = FASTER (direct bucket access)
//    TreeMap = SLOWER but SORTED iteration
//
//    Visual — Why HashMap O(1)?
//       findById("S1")
//          ↓ hash("S1") % 16 = 5
//       ┌───┬───┬───┬───┬───┬─────────┐
//       │ 0 │ 1 │ 2 │ 3 │ 4 │ S1→Sav  │ ← direct bucket
//       └───┴───┴───┴───┴───┴─────────┘
//       = O(1) constant
//
//    TreeMap (Red-Black Tree):
//                50
//               /  \                  ← traverse log n levels
//             20    70
//       = O(log n)
//
//    BANK USE CASE:
//       • Save by ID, fetch by ID, delete by ID
//       • NO sorted iteration needed
//       = HashMap perfect
//
// Optional.ofNullable() INTERNAL:
//    HashMap.get(key) contract:
//       Found?    → returns value
//       Not found? → returns NULL
//
//    Optional.ofNullable handles BOTH:
//       value != null  →  Optional.of(value)    "non-empty"
//       value == null  →  Optional.empty()       "empty"
//
//    Visual flow:
//       accounts.get("S1")
//            ↓
//       Found? → Account → ofNullable → Optional.of(Account)
//       Not?   → null    → ofNullable → Optional.empty()
//
//    of vs ofNullable vs empty:
//       Optional.of(val)         → NPE if val null
//                                   "SURE hai value hai"
//       Optional.ofNullable(val) → safe handles null
//                                   "Pata nahi null hai ya nahi"
//       Optional.empty()         → explicit empty
//
//    WHY ofNullable HERE?
//       HashMap.get() can return null
//       ofNullable safely wraps it
//       Caller gets Optional → null-safe operations
//
//    Internal source (approximate):
//       public static <T> Optional<T> ofNullable(T value) {
//           return value == null ? empty() : of(value);
//       }
//
// PATTERN: Repository (concrete implementation)
//
// SOLID:
//    LSP — Substitutable for any AccountRepository impl
//          Replace karke MySQLAccountRepository daal de
//          AccountService UNCHANGED works
//    DIP — Service depends on interface, this concrete pluggable
// ═══════════════════════════════════════════════════════════════════════

public class InMemoryAccountRepository implements AccountRepository {

    // Collections — HashMap key=accountId, value=Account
    private final Map<String, Account> accounts = new HashMap<>();

    @Override
    public void save(Account account) {
        accounts.put(account.getAccountId(), account);
    }

    @Override
    public Optional<Account> findById(String accountId) {
        return Optional.ofNullable(accounts.get(accountId));
        // map.get() null bhi de sakta — ofNullable safe wrap
    }

    @Override
    public List<Account> findAll() {
        return new ArrayList<>(accounts.values());
        // defensive copy — caller modify kare toh internal safe
    }

    @Override
    public void delete(String accountId) {
        accounts.remove(accountId);
    }
}
