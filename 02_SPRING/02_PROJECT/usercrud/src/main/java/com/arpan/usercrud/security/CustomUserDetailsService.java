package com.arpan.usercrud.security;

import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import com.arpan.usercrud.model.User;
import com.arpan.usercrud.repository.UserRepository;

/* ════════════════════════════════════════════════════════════════════
 *  👨‍💼 CustomUserDetailsService — THE HR DEPARTMENT
 * ════════════════════════════════════════════════════════════════════
 *
 *  Yeh class HR department hai — Spring Security puchti "iss email
 *  ka employee hai? permissions kya hain?" — yeh DB se dhundh ke
 *  proper format mein answer deta.
 *
 *  ─── 🏢 ANALOGY: Office HR Department ─────────────────────────────
 *
 *      🛡️ Guard (JwtFilter)         👨‍💼 HR (this class)
 *           │                              │
 *           │  📞 "Arpan naam ka banda     │
 *           │      hai? Permissions kya?"  │
 *           │ ──────────────────────────►  │
 *           │                              │
 *           │                              │  📂 DB mein dhundta
 *           │                              │     (UserRepository)
 *           │                              │     ─────────────────
 *           │                              │     id: 5
 *           │                              │     email: arpan@x.com
 *           │                              │     password: $2a$... (BCrypt)
 *           │                              │     role: ADMIN
 *           │                              │
 *           │  ✅ "Haan, Arpan,            │
 *           │     role: ROLE_ADMIN"        │
 *           │ ◄──────────────────────────  │
 *           │                              │
 *           │  Log book entry banata       │
 *           ▼                              │
 *
 *      Note: HR ne password match nahi kiya — sirf details diye.
 *           Password matching alag step (Spring Security khud karta
 *           BCrypt encoder use karke).
 *
 *  ─── 📜 INTERFACE: UserDetailsService ─────────────────────────────
 *
 *      Spring Security ka contract — sirf 1 method:
 *
 *          UserDetails loadUserByUsername(String username);
 *
 *      Spring bolta: "HR, mujhe sirf yeh ek method chahiye —
 *                    username (ya email) do, mujhe UserDetails do."
 *
 *      Hum apni implementation deenge — UserRepository se fetch karke
 *      Spring's UserDetails object banate.
 *
 *  ─── 🔄 LOGIN FLOW MEIN ROLE ──────────────────────────────────────
 *
 *      Client → POST /auth/login { email, password }
 *                  │
 *                  ▼
 *      AuthenticationManager (Spring Security ka brain)
 *                  │
 *                  ├─► loadUserByUsername("arpan@x.com")  ⭐ THIS CLASS
 *                  │   └─► UserRepository.findByEmail() → DB hit
 *                  │       └─► User entity returned
 *                  │           └─► Convert to UserDetails (ROLE_ prefix)
 *                  │
 *                  ▼
 *      Spring PasswordEncoder.matches(rawPassword, storedHash)
 *                  │
 *                  ├─► Match → ✅ Authentication successful
 *                  └─► Mismatch → ❌ BadCredentialsException
 *
 *  ════════════════════════════════════════════════════════════════════
 *  🎨 DESIGN PATTERN: BUILDER (used via User.builder())
 *  📐 SOLID: DIP, SRP
 *  ════════════════════════════════════════════════════════════════════
 *
 *  Yeh class Spring's `User.builder()` use karta UserDetails
 *  object banane ke liye:
 *
 *      User.builder()
 *          .username(email)
 *          .password(hash)
 *          .authorities("ROLE_" + role)
 *          .build();         ← final immutable UserDetails
 *
 *  Step-by-step setters, `build()` final immutable object return.
 *
 *  📐 SOLID — DIP (Dependency Inversion):
 *  Yeh class `UserDetailsService` INTERFACE implement karti —
 *  Spring Security iss interface pe depend karta, hamari concrete
 *  class pe nahi. Future mein DB se LDAP switch kare? sirf nayi
 *  implementation banao — Spring Security untouched.
 *
 *  📐 SOLID — SRP (Single Responsibility):
 *  Yeh class SIRF user load karti DB se — nothing else. Password
 *  matching Spring Security khud karta, JWT validation JwtFilter,
 *  business logic UserService.
 *
 *  🎤 INTERVIEW LINE:
 *  "CustomUserDetailsService Builder pattern use karta Spring ke
 *   User.builder() ke through — UserDetails immutable object
 *   banata. UserDetailsService interface implement karta — Spring
 *   Security DIP follow karta, mera concrete class swap-able."
 *
 *  ─── 🎫 ROLE_ PREFIX KA KHEL ──────────────────────────────────────
 *
 *      DB mein store:        "USER" / "ADMIN"   (clean, simple)
 *      Spring Security:       ROLE_USER / ROLE_ADMIN  (with prefix)
 *
 *      Why? `hasRole("ADMIN")` Spring internally check karta:
 *           authority == "ROLE_ADMIN" ?
 *
 *      Hum convert kar dete:
 *           "ROLE_" + user.getRole()  →  "ROLE_ADMIN"
 *
 *      Yeh Spring convention hai — mat tod, follow kar.
 *
 *  ════════════════════════════════════════════════════════════════════
 *  🎤 INTERVIEW TALKING POINT — CustomUserDetailsService
 *  ════════════════════════════════════════════════════════════════════
 *
 *  Q: "Spring Security ko apne DB se user kaise load karwate?"
 *
 *  Tu bolega:
 *  "Spring Security ka `UserDetailsService` interface implement
 *   karta hu — sirf ek method hota `loadUserByUsername()`. Mai
 *   email se DB mein user dhundhta `UserRepository.findByEmail()`
 *   se, phir Spring's `UserDetails` object banake return karta
 *   email, BCrypt-hashed password, aur authorities ke saath.
 *
 *   Authority creation time pe `ROLE_` prefix add karta — DB
 *   mein simple 'USER'/'ADMIN' rakhta hu, but Spring internally
 *   `hasRole('ADMIN')` check `ROLE_ADMIN` se karta hai. Toh
 *   prefix yahaan add kar dete.
 *
 *   Yeh service `AuthenticationManager` automatically use karta
 *   login flow mein, aur mera `JwtFilter` bhi har request pe
 *   authenticated user load karne ke liye."
 *
 *  Q: "Username vs email — kya use karte?"
 *  → "Email — kyunki DB mein unique field hai. Method parameter
 *     naam 'username' Spring ka convention hai, but actual
 *     identifier email use karta hu."
 *
 *  Q: "Password match yahaan karte ho?"
 *  → "Nahi — yeh sirf user load karta. Password matching Spring
 *     Security khud karta `PasswordEncoder.matches()` se
 *     login flow ke andar."
 *  ════════════════════════════════════════════════════════════════════
 */
@Service
public class CustomUserDetailsService implements UserDetailsService {

    /*
     *  📂 DB lookup tool — UserRepository
     *  HR ke paas yeh "directory" hai DB mein dhundhne ke liye.
     */
    private final UserRepository userRepository;

    public CustomUserDetailsService(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    // ════════════════════════════════════════════════════════════
    //  📞 loadUserByUsername — Spring Security calls this
    // ════════════════════════════════════════════════════════════
    /*
     *  Spring puchti: "Iss username ke details do"
     *  Hum: "Email se dhundhte, mil gaya? UserDetails return.
     *        Nahi mila? UsernameNotFoundException."
     *
     *  Note: Method param naam "username" Spring convention hai,
     *  but hum actual mein email use kar rahe (DB mein unique).
     */
    @Override
    public UserDetails loadUserByUsername(String email) throws UsernameNotFoundException {

        // 📂 STEP 1: DB se user dhundo email se
        //    findByEmail() returns Optional<User> — present? extract.
        //    Empty? → exception throw.
        User user = userRepository.findByEmail(email)
                .orElseThrow(() ->
                    new UsernameNotFoundException("User not found with email: " + email));

        // 🎫 STEP 2: User entity → Spring's UserDetails object
        //
        //    Spring ka UserDetails interface hai — kuch fields chahiye:
        //    • username (identifier)
        //    • password (hashed)
        //    • authorities (roles/permissions)
        //
        //    Hum Spring ke built-in builder use kar rahe:
        //        org.springframework.security.core.userdetails.User.builder()
        //
        //    NAUKAR-CHAKAR alert: Yeh Spring ka User class HUMARE User
        //    entity se ALAG hai. Same naam, different package.
        //    Confusion bachne ke liye full path use karte hain.
        return org.springframework.security.core.userdetails.User.builder()
                .username(user.getEmail())                        // 📛 identifier (email)
                .password(user.getPassword())                      // 🔒 BCrypt hashed (DB mein already)
                .authorities("ROLE_" + user.getRole())             // 🎫 ROLE_USER / ROLE_ADMIN
                .build();
    }
}
