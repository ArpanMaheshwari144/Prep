package com.arpan.usercrud.service;

import java.util.List;

import org.springframework.stereotype.Service;

import com.arpan.usercrud.exception.UserNotFoundException;
import com.arpan.usercrud.model.User;
import com.arpan.usercrud.repository.UserRepository;

/* ════════════════════════════════════════════════════════════════
 *  📌 SERVICE — Business Logic Layer
 * ════════════════════════════════════════════════════════════════
 *  3-Layer architecture ka middle layer:
 *
 *      Controller  →  Service  →  Repository  →  DB
 *      (HTTP)        (Logic)    (Data Access)
 *
 *  Yahaan business rules likhte hain (validation, transformations,
 *  multi-step workflows). Controller patla rakhte hain — bus
 *  request lo aur service ko bhejo. Repository sirf DB queries.
 *
 *  ─── @Service kya karta hai? ────────────────────────────────────
 *  • @Component ka specialized version hai (semantic difference)
 *  • Spring iss class ka ek bean banata hai (singleton scope by default)
 *  • Controller ko inject ho sakta hai
 *  • Code padhne wala turant samjhe — yeh business logic class hai
 *
 *  ─── DEPENDENCY INJECTION — 3 types (interview gold) ────────────
 *
 *  1. Constructor Injection (yeh wala — BEST PRACTICE)
 *     ✅ Field final ban sakta hai (immutable)
 *     ✅ NullPointerException impossible (object banega hi nahi
 *        bina dependency ke)
 *     ✅ Testing easy — mock pass karke constructor call kar de
 *     ✅ Circular dependencies compile-time pe pakdi jati hain
 *
 *  2. Setter Injection — optional dependencies ke liye
 *     @Autowired
 *     public void setRepository(UserRepository r) { this.repo = r; }
 *
 *  3. Field Injection — DON'T USE (anti-pattern)
 *     @Autowired private UserRepository repository;
 *     ❌ Field final nahi ho sakta
 *     ❌ Testing mein reflection chahiye
 *     ❌ Hidden dependencies — class signature se nahi dikhta
 *
 *  Note: Single constructor ho toh @Autowired bhi optional hai
 *  (Spring auto-detect karega).
 *
 *  ─── orElseThrow() pattern — Optional ka clean handling ─────────
 *  repository.findById(id)            → Optional<User>
 *      .orElseThrow(() -> new ...)    → User mila? unwrap kar do.
 *                                       Nahi mila? exception throw.
 *
 *  Lambda () -> new UserNotFoundException(id) — Supplier interface.
 *  Sirf tabhi execute hota hai jab Optional empty ho (lazy).
 * ════════════════════════════════════════════════════════════════
 */
@Service
public class UserService {

    // final = immutable reference, constructor mein hi set hoga
    private final UserRepository repository;

    // Constructor injection — Spring automatically UserRepository
    // bean dhundhke yahaan inject kar dega (single-constructor rule)
    public UserService(UserRepository repository) {
        this.repository = repository;
    }

    // ─── CREATE ─────────────────────────────────────────────────
    // save() = INSERT (agar id null hai) ya UPDATE (agar id exists)
    // Returns: managed entity with generated ID populated
    public User create(User user) {
        return repository.save(user);
    }

    // ─── READ by ID ─────────────────────────────────────────────
    // Mila toh User return; nahi mila toh custom exception
    // → GlobalExceptionHandler 404 response banayega
    public User getById(Long id) {
        return repository.findById(id)
                .orElseThrow(() -> new UserNotFoundException(id));
    }

    // ─── READ all ───────────────────────────────────────────────
    // Production tip: large tables ke liye Pageable use karo,
    // findAll() khatarnak hai (memory blow up)
    public List<User> getAll() {
        return repository.findAll();
    }

    // ─── UPDATE ─────────────────────────────────────────────────
    // Pattern: fetch → mutate → save
    // Yeh "managed entity update" pattern hai. Hibernate ki dirty
    // checking automatic detect karegi changes aur UPDATE chalegi.
    public User update(Long id, User updatedUser) {
        User user = repository.findById(id)
                .orElseThrow(() -> new UserNotFoundException(id));

        user.setName(updatedUser.getName());
        user.setAge(updatedUser.getAge());
        user.setEmail(updatedUser.getEmail());
        user.setPassword(updatedUser.getPassword());

        return repository.save(user);
    }

    // ─── DELETE ─────────────────────────────────────────────────
    // Pehle existence check, varna deleteById() silently chal
    // jata hai non-existing ID pe — explicit exception better
    public void delete(Long id) {
        if (repository.existsById(id)) {
            repository.deleteById(id);
        } else {
            throw new UserNotFoundException(id);
        }
    }
}
