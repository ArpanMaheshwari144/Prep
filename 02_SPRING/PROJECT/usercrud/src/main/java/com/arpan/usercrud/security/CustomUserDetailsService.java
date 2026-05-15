package com.arpan.usercrud.security;

import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import com.arpan.usercrud.model.User;
import com.arpan.usercrud.repository.UserRepository;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    CustomUserDetailsService = HR DEPARTMENT
//    Guard (JwtFilter) puchta: "Yeh banda DB mein hai ya nahi?"
//    HR: DB lookup → UserDetails object return
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — GUARD ↔ HR FLOW:
//    🛡️ Guard (JwtFilter)         👨‍💼 HR (this class)
//         │                              │
//         │  "Arpan ka info chahiye"     │
//         │ ──────────────────────────►  │
//         │                              │
//         │                              │  📂 DB lookup
//         │                              │     UserRepository.findByEmail
//         │                              │     User entity returned
//         │                              │     id, email, password, role
//         │                              │
//         │                              │  🎫 Convert to UserDetails
//         │                              │     (Spring's standard object)
//         │                              │     ROLE_ prefix added
//         │  ✅ UserDetails object       │
//         │ ◄──────────────────────────  │
//
// 🔑 UserDetailsService INTERFACE — Spring's Contract:
//    public interface UserDetailsService {
//        UserDetails loadUserByUsername(String username);
//    }
//
//    Sirf 1 method — username (or email) do, UserDetails do
//
// 2 USE CASES (where called):
//
//    FLOW 1: Initial Login (AuthController)
//       POST /auth/login { email, password }
//            │
//            ▼
//       AuthenticationManager
//            │
//            ├─► loadUserByUsername("arpan@x.com")  ← THIS METHOD
//            │   └─► UserRepository.findByEmail()
//            │       └─► User entity → UserDetails
//            │
//            ▼
//       PasswordEncoder.matches(rawPassword, storedHash)
//            ├─► Match → ✅ generate tokens
//            └─► Mismatch → ❌ BadCredentialsException
//
//    FLOW 2: JWT Filter (Every Request)
//       JwtFilter validates token
//            │
//            ▼
//       Extract email from token
//            │
//            ▼
//       userDetailsService.loadUserByUsername(email)  ← THIS METHOD
//            │
//            ▼
//       UserDetails returned
//            │
//            ▼
//       Set in SecurityContext
//
// 🔑 ROLE_ PREFIX MAGIC:
//    DB stores:      "USER" / "ADMIN"    (clean)
//    Spring expects: "ROLE_USER" / "ROLE_ADMIN"  (with prefix)
//
//    Convert at authority creation:
//       "ROLE_" + user.getRole()
//       → "ROLE_ADMIN"
//
//    Why?
//       Spring's @PreAuthorize("hasRole('ADMIN')")
//       Internally checks: authority == "ROLE_ADMIN"?
//       = Convention from Spring — follow karna padta
//
// 🔑 SPRING'S BUILDER PATTERN:
//    org.springframework.security.core.userdetails.User.builder()
//        .username(user.getEmail())
//        .password(user.getPassword())
//        .authorities("ROLE_" + user.getRole())
//        .build();
//
//    Note: Spring's User class ≠ humara User entity
//          Same naam, alag package — full path use karte confusion bachane ke liye
//
// PASSWORD MATCH — NOT HERE!
//    Yeh class SIRF user load karta
//    NO password comparison here
//
//    Password matching:
//       PasswordEncoder.matches(rawPassword, storedHash)
//       Spring Security khud karta login flow mein
//
//    Yahan sirf: "User mila + uska hashed password kya"
//
// CUSTOM USERNAME NOTE:
//    Method param "username" = Spring convention
//    We actually use EMAIL (DB unique)
//    Spring doesn't care what string — just identifier
//    Method name STAYS "loadUserByUsername"
//
// 🎨 PATTERN: BUILDER (used via User.builder())
//
// 📐 SOLID:
//    DIP — UserDetailsService INTERFACE implement
//          Spring Security depends on interface, not concrete
//          Future LDAP swap = new implementation, Spring untouched
//
//    SRP — Sirf user load karta from DB
//          No password matching (Spring's job)
//          No token (JwtService)
//          No business logic (UserService)
//
// 🎤 INTERVIEW LINE:
//    "CustomUserDetailsService implements UserDetailsService —
//     Spring's contract. Sirf 1 method: loadUserByUsername.
//
//     Email se DB lookup via UserRepository, then convert User entity
//     → Spring's UserDetails via Builder pattern.
//
//     ROLE_ prefix added at authority creation — Spring convention
//     (hasRole('ADMIN') checks for 'ROLE_ADMIN' authority).
//
//     Password matching NOT here — Spring's PasswordEncoder.matches()
//     does that automatically in login flow."
// ═══════════════════════════════════════════════════════════════════════

@Service
public class CustomUserDetailsService implements UserDetailsService {

    // 📂 DB lookup tool — UserRepository
    // HR ke paas yeh "directory" hai DB mein dhundhne ke liye
    private final UserRepository userRepository;

    public CustomUserDetailsService(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    // ════════════════════════════════════════════════════════════
    //  📞 loadUserByUsername — Spring Security calls this
    // ════════════════════════════════════════════════════════════
    //  Spring puchti: "Iss username ke details do"
    //  Hum: "Email se dhundhte, mil gaya? UserDetails return.
    //        Nahi mila? UsernameNotFoundException."
    //
    //  Note: Method param naam "username" Spring convention hai,
    //        but hum actual mein email use kar rahe (DB unique)
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
        //    Spring ka UserDetails interface — fields chahiye:
        //    • username (identifier)
        //    • password (hashed)
        //    • authorities (roles/permissions)
        //
        //    Spring's built-in builder use karte:
        //        org.springframework.security.core.userdetails.User.builder()
        //
        //    Spring's User class ≠ humara User entity (alag package)
        //    Full path = confusion bachane ke liye
        return org.springframework.security.core.userdetails.User.builder()
                .username(user.getEmail())                        // 📛 identifier
                .password(user.getPassword())                      // 🔒 BCrypt hashed
                .authorities("ROLE_" + user.getRole())             // 🎫 ROLE_USER / ROLE_ADMIN
                .build();
    }
}
