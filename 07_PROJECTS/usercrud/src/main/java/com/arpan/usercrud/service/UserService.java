package com.arpan.usercrud.service;

import java.util.List;

import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.arpan.usercrud.exception.UserNotFoundException;
import com.arpan.usercrud.model.User;
import com.arpan.usercrud.repository.UserRepository;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    BUSINESS LOGIC LAYER (middle of 3-layer)
//    Controller → SERVICE → Repository → DB
//    Sirf business rules — no HTTP handling, no DB connections
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — 3-LAYER ARCHITECTURE:
//    HTTP request
//         │
//         ▼
//    ┌─────────────────────┐
//    │  Controller          │  ← @RestController
//    └──────────┬──────────┘
//               │
//               ▼
//    ┌─────────────────────┐
//    │  Service             │  ← @Service (TU YAHAN)
//    │  (business logic)    │
//    │  @Transactional      │
//    └──────────┬──────────┘
//               │
//               ▼
//    ┌─────────────────────┐
//    │  Repository          │  ← @Repository
//    └─────────────────────┘
//
// METHODS (6 total):
//    1. create()                     @Transactional
//    2. getById()                    @Transactional(readOnly=true)
//    3. getAll()                     @Transactional(readOnly=true)
//    4. update()                     @Transactional
//    5. delete()                     @Transactional
//    6. createWithSimulatedFailure() @Transactional (DEMO rollback)
//
// @Transactional PATTERNS:
//    Write methods:
//       @Transactional
//       Atomic transaction (commit/rollback)
//
//    Read methods:
//       @Transactional(readOnly = true)
//       Hibernate optimization (no dirty checking)
//       Faster, may route to read replica
//
// CONSTRUCTOR INJECTION:
//    private final UserRepository repository;
//    public UserService(UserRepository repo) { ... }
//
//    Benefits:
//       final = immutable
//       NPE impossible (must pass at construction)
//       Testing easy (mock pass)
//       Spring single-ctor → @Autowired implicit
//
// DIRTY CHECKING — repository.save() in update() = REDUNDANT
//
//    UPDATE method case:
//       User user = repository.findById(id)...   ← entity MANAGED
//       user.setName(...);                        ← modify managed
//       return repository.save(user);             ← REDUNDANT!
//
//       Dirty checking auto-UPDATE karega commit pe!
//
//    MINIMAL version (no save):
//       @Transactional
//       public User update(Long id, User updatedUser) {
//           User user = repository.findById(id).orElseThrow(...);
//           user.setName(updatedUser.getName());
//           return user;   // Hibernate auto-UPDATE on commit
//       }
//
//    Both versions produce SAME UPDATE SQL!
//
//    BUT — CREATE method REQUIRES save():
//       @Transactional
//       public User create(User user) {
//           return repository.save(user);    // MANDATORY
//       }
//
//       Reason:
//          New User = NOT managed (just Java instance)
//          save() = "Hibernate, isko nazar mein le, INSERT kar"
//          Without it: object Java memory mein, DB mein INSERT NAHI
//
// MANAGED vs DETACHED:
//    NEW USER (just created):
//       User u = new User(...);
//       = Detached (Hibernate doesn't know)
//       = save() MANDATORY
//
//    FETCHED USER (from DB in same TX):
//       User u = repository.findById(id).get();
//       = Managed (Hibernate tracks)
//       = save() OPTIONAL (dirty checking handles)
//
// WHY KEEP save() IN UPDATE (even though redundant)?
//    1. EXPLICIT INTENT — "yeh save ho raha hai" clear
//    2. SAFETY HABIT — works even if detached scenario
//    3. CODE REVIEW — reviewer dekhke samjha
//    4. CONSISTENT WITH CREATE — uniform pattern
//
// @Transactional DEFAULT BEHAVIOR:
//    • Propagation = REQUIRED (join existing or create new)
//    • Isolation = READ_COMMITTED (DB default)
//    • Rollback = RuntimeException + Error
//
//    Common customizations:
//       rollbackFor = Exception.class   ← include checked exceptions
//       readOnly = true                  ← read-only optimization
//       timeout = 5                      ← max 5 seconds
//       propagation = REQUIRES_NEW       ← always new transaction
//
// SOLID:
//    SRP — Sirf business logic
//          Data access = UserRepository
//          Auth        = AuthController + JwtService
//          Validation  = Bean Validation (@Valid)
//          Exceptions  = GlobalExceptionHandler
//
//    DIP — Depends on UserRepository INTERFACE
//          Concrete proxy injected by Spring
//
// SimpleBankSystem vs UserCRUD Service:
//    SimpleBankSystem AccountService:
//       Constructor injection (manual)
//       Manual rollback (try-catch + reverse)
//       No @Transactional
//
//    UserCRUD UserService:
//       Constructor injection (Spring detects)
//       @Transactional automatic rollback
//       readOnly optimization
//       = Spring automates what SimpleBankSystem did manually
//
// INTERVIEW LINE:
//    "UserService is business logic with @Transactional.
//     Write methods atomic, read methods readOnly optimized.
//
//     Dirty checking: inside @Transactional, fetch + mutate managed
//     entity → Hibernate auto-fires UPDATE on commit. save() in
//     update() is redundant but kept for code clarity.
//
//     create() needs save() — new entity not managed yet.
//
//     Demo method proves @Transactional rollback — save() succeeds,
//     then throw RuntimeException → entire INSERT rolled back."
// ═══════════════════════════════════════════════════════════════════════

@Service
public class UserService {

    private final UserRepository repository;

    public UserService(UserRepository repository) {
        this.repository = repository;
    }

    // ═══════════════════════════════════════════════════════════
    //  CRUD METHODS — @Transactional applied
    // ═══════════════════════════════════════════════════════════

    // ─── CREATE ─────────────────────────────────────────────────
    // @Transactional → method atomic DB transaction mein wrap
    // save() MANDATORY here — new entity not managed yet
    @Transactional
    public User create(User user) {
        return repository.save(user);
    }

    // ─── READ by ID ─────────────────────────────────────────────
    // readOnly = true → Hibernate optimization (no dirty checking)
    @Transactional(readOnly = true)
    public User getById(Long id) {
        return repository.findById(id)
                .orElseThrow(() -> new UserNotFoundException(id));
    }

    // ─── READ all ───────────────────────────────────────────────
    @Transactional(readOnly = true)
    public List<User> getAll() {
        return repository.findAll();
    }

    // ─── UPDATE ─────────────────────────────────────────────────
    // Pattern: fetch → mutate → save (explicit, but redundant)
    // Hibernate dirty checking would auto-UPDATE on commit even without save()
    // Kept for code clarity + safety habit
    @Transactional
    public User update(Long id, User updatedUser) {
        User user = repository.findById(id)
                .orElseThrow(() -> new UserNotFoundException(id));

        user.setName(updatedUser.getName());
        user.setAge(updatedUser.getAge());
        user.setEmail(updatedUser.getEmail());
        user.setPassword(updatedUser.getPassword());

        return repository.save(user);   // redundant (dirty checking handles), but explicit
    }

    // ─── DELETE ─────────────────────────────────────────────────
    // existsById check → proper 404 if missing (vs silent ignore by deleteById)
    @Transactional
    public void delete(Long id) {
        if (repository.existsById(id)) {
            repository.deleteById(id);
        } else {
            throw new UserNotFoundException(id);
        }
    }

    // ═══════════════════════════════════════════════════════════
    //  DEMO METHOD — @Transactional rollback proof
    // ═══════════════════════════════════════════════════════════
    //  Flow:
    //  1. user.save()      → row INSERT (TX ke andar)
    //  2. throw RuntimeEx  → exception
    //  3. Spring proxy     → ROLLBACK
    //  4. Result           → row gayab (jaise insert hua hi nahi)
    //
    //  Test:
    //  • POST /users/demo/rollback (controller endpoint)
    //  • Then GET /users — rolled-back user MISSING
    //  • H2 console: http://localhost:8080/h2-console
    //
    //  Experiment (proof @Transactional):
    //  • Remove @Transactional from this method
    //  • Same endpoint hit → GET /users shows user (no rollback)
    //  • Adds @Transactional back → rollback works
    @Transactional
    public User createWithSimulatedFailure(User user) {
        // Step 1: User save (INSERT fires TX ke andar)
        User saved = repository.save(user);
        System.out.println("Step 1: User saved with ID = " + saved.getId());

        // Step 2: Simulated failure
        System.out.println("Step 2: Simulating failure (RuntimeException)...");
        throw new RuntimeException(
            "Simulated failure after save() — Spring should ROLLBACK"
        );

        // Step 3: Yeh line kabhi nahi pahunchti (exception ke baad)
    }
}
