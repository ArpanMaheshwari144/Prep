package com.arpan.usercrud.model;

import java.time.Instant;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    RefreshToken Entity — DB-stored long-lived token
//    Refresh tokens DB mein persist karta
//    Taaki logout pe REVOKE kar sakein
// ═══════════════════════════════════════════════════════════════════════
//
// WHY ACCESS vs REFRESH SEPARATE?
//    ┌──────────────────────────┬──────────────────────────────┐
//    │  Access Token (15 min)   │  Refresh Token (7 days)      │
//    ├──────────────────────────┼──────────────────────────────┤
//    │  • Server pe NAHI store  │  • Server pe DB mein STORE   │
//    │  • Stateless              │  • Stateful (for revocation) │
//    │  • Signature validate     │  • DB lookup validate        │
//    │  • Har request mein bheja │  • Sirf /refresh pe bhejta   │
//    │  • Leak window: 15 min    │  • Leak window: until logout │
//    └──────────────────────────┴──────────────────────────────┘
//
// DB TABLE STRUCTURE:
//    refresh_tokens table:
//    ┌────┬─────────────┬─────────┬──────────────────────────┐
//    │ id │  token      │ user_id │  expires_at              │
//    ├────┼─────────────┼─────────┼──────────────────────────┤
//    │ 1  │ "RRRR-..."  │   5     │ 2026-05-07T10:30:00Z     │
//    │ 2  │ "XXXX-..."  │   8     │ 2026-05-07T11:15:00Z     │
//    └────┴─────────────┴─────────┴──────────────────────────┘
//
// LIFECYCLE:
//    LOGIN:
//       AuthController generates UUID → INSERT row
//
//    REFRESH (every ~15 min):
//       findByToken(token) → DB lookup → validate expiry
//                          → return new access token
//
//    LOGOUT:
//       deleteByToken(token) → row REMOVED
//       = Refresh dead = session ended
//
//    EXPIRED (7+ days offline):
//       Row stale → next refresh fails
//       Optional: scheduled cleanup job (@Scheduled)
//
// 🔑 WHY UUID (NOT JWT) FOR REFRESH?
//    TRADE-OFF DECISION:
//
//    JWT for refresh:
//       ✅ Self-validating (no DB lookup)
//       ❌ Can't revoke easily (until expiry)
//
//    UUID for refresh (humara approach):
//       ✅ DB lookup mandatory → instant revocation
//       ✅ Logout pe delete = immediate session end
//       ❌ DB hit per refresh (rare, acceptable)
//
//    Decision rationale:
//       Refresh used 1x per 15 min = DB hit OK
//       Revocation guarantee critical for security
//
// FIELDS BREAKDOWN:
//    token  — UUID string (unique=true: same token never twice)
//    userId — owner reference
//             Simple project: just Long (no FK relationship)
//             Production: @ManyToOne + @JoinColumn (proper FK)
//    expiresAt — Instant (modern Java time)
//                 set: Instant.now().plus(7, ChronoUnit.DAYS)
//                 check: expiresAt.isBefore(Instant.now()) → expired
//
//    Instant vs alternatives:
//       Instant         = modern Java 8+ (use this)
//       LocalDateTime   = no timezone
//       Date            = legacy, avoid
//
// PRODUCTION CLEANUP:
//    Expired tokens slowly accumulate in DB
//
//    Solution: scheduled job
//       @Scheduled(cron = "0 0 2 * * *")  // 2 AM daily
//       public void cleanupExpired() {
//           refreshTokenRepo.deleteByExpiresAtBefore(Instant.now());
//       }
//
// 📐 SOLID — SRP:
//    Sirf refresh token data + DB mapping
//    No business logic, no validation logic
//
// 🎤 INTERVIEW LINE:
//    "RefreshToken entity stores refresh tokens DB-side enabling
//     revocation on logout. Trade-off: stateful vs stateless.
//     Chose UUID over JWT for refresh — revocation guarantee
//     outweighs DB lookup overhead (refresh is rare, not every request).
//     Logout calls deleteByToken = immediate session termination."
// ═══════════════════════════════════════════════════════════════════════

@Entity
@Table(name = "refresh_tokens")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class RefreshToken {

    // 🔢 Primary key — auto-increment
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    // 🎫 Actual refresh token string (UUID)
    // unique=true → same token DB mein 2 baar nahi
    @Column(nullable = false, unique = true)
    private String token;

    // 👤 Owner user ID — kis user ke liye yeh token issued hua
    // (Foreign key relationship — but simple project mein direct Long)
    // Production: @ManyToOne with User entity, @JoinColumn use karte
    @Column(nullable = false)
    private Long userId;

    // ⏰ Expiration timestamp
    // Login pe set: Instant.now().plus(7, ChronoUnit.DAYS)
    // Refresh time pe check: expiresAt.isBefore(Instant.now()) → expired
    @Column(nullable = false)
    private Instant expiresAt;
}
