package com.arpan.usercrud.service;

import java.util.List;

import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.arpan.usercrud.exception.UserNotFoundException;
import com.arpan.usercrud.model.User;
import com.arpan.usercrud.repository.UserRepository;

/* ════════════════════════════════════════════════════════════════
 *  📌 SERVICE — Business Logic Layer (with @Transactional)
 * ════════════════════════════════════════════════════════════════
 *  3-Layer architecture ka middle layer:
 *
 *      Controller  →  Service  →  Repository  →  DB
 *      (HTTP)        (Logic)    (Data Access)
 *
 *  ─── @Transactional kahan kahan lagaya ──────────────────────────
 *  • Write operations (create/update/delete) → @Transactional
 *      (atomic — fail hua toh rollback)
 *  • Read operations (getById/getAll) → @Transactional(readOnly=true)
 *      (Hibernate optimization — dirty checking skip)
 *  • createWithSimulatedFailure() → DEMO method to PROVE rollback
 *
 *  ─── DEPENDENCY INJECTION (already covered) ─────────────────────
 *  Constructor injection use ho raha — final field, single constructor,
 *  Spring auto-detect karega (@Autowired implicit).
 * ════════════════════════════════════════════════════════════════
 */
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
    // @Transactional → method ek atomic DB transaction mein wrap
    // Default propagation = REQUIRED, default rollback = RuntimeException
    @Transactional
    public User create(User user) {
        return repository.save(user);
    }

    // ─── READ by ID ─────────────────────────────────────────────
    // readOnly = true → Hibernate optimization (no dirty checking,
    // no UPDATE generation). Best practice for SELECT-only methods.
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
    // Pattern: fetch → mutate → save (Hibernate dirty checking
    // automatic UPDATE generate karta hai)
    @Transactional
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
    @Transactional
    public void delete(Long id) {
        if (repository.existsById(id)) {
            repository.deleteById(id);
        } else {
            throw new UserNotFoundException(id);
        }
    }

    // ═══════════════════════════════════════════════════════════
    //  🧪 DEMO METHOD — @Transactional rollback proof
    // ═══════════════════════════════════════════════════════════
    /*
     *  Yeh method intentionally fail karta hai save() ke BAAD —
     *  toh tu apni aankhon se dekh sake @Transactional ka jaadu.
     *
     *  Flow:
     *  1. user.save()      → row INSERT ho gayi (TX ke andar)
     *  2. throw RuntimeEx  → exception
     *  3. Spring proxy     → ROLLBACK ⏪
     *  4. Result           → row gayab (jaise insert hua hi nahi)
     *
     *  Test karne ke liye:
     *  • Endpoint hit kar (controller mein add kiya hai):
     *      POST /users/demo/rollback
     *  • Phir GET /users — rolled-back user MISSING dikhega
     *  • H2 console (http://localhost:8080/h2-console) bhi check kar
     *
     *  ─── Aur ek experiment (interview understanding ke liye) ───
     *  • Yeh @Transactional hata de
     *  • Same endpoint hit kar
     *  • Ab GET /users — user DIKHEGA (no rollback, save committed)
     *  • This proves @Transactional ka asli kaam
     */
    @Transactional
    public User createWithSimulatedFailure(User user) {
        // Step 1: User save (INSERT fire hota hai TX ke andar)
        User saved = repository.save(user);
        System.out.println("✅ Step 1: User saved with ID = " + saved.getId());

        // Step 2: Simulated failure
        System.out.println("💥 Step 2: Simulating failure (RuntimeException)...");
        throw new RuntimeException(
            "Simulated failure after save() — Spring should ROLLBACK"
        );

        // Step 3: Yeh line kabhi nahi pahunchti
        // (exception ke baad code execute nahi hota)
    }
}
