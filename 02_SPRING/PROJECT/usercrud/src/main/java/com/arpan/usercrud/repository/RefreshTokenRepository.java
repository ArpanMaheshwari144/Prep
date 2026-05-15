package com.arpan.usercrud.repository;

import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.arpan.usercrud.model.RefreshToken;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    DB access for REFRESH TOKENS
//    Login: token DB mein save
//    Refresh: token validate
//    Logout: token DB se delete
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — SAME PATTERN as UserRepository:
//    RefreshTokenRepository (interface)
//         │ extends
//         ▼
//    JpaRepository<RefreshToken, Long>
//         │
//         ▼
//    Spring Data JPA proxy at runtime (auto-implements)
//
// 2 CUSTOM METHODS — USE CASES:
//
//    findByToken(String) — Refresh Flow
//       AuthController.refresh():
//          refreshTokenRepo.findByToken(token)
//              .orElseThrow(() -> new InvalidTokenException());
//          → SELECT * FROM refresh_tokens WHERE token = ?
//
//    deleteByToken(String) — Logout Flow
//       AuthController.logout():
//          refreshTokenRepo.deleteByToken(token);
//          → DELETE FROM refresh_tokens WHERE token = ?
//
// LIFECYCLE FLOW:
//    LOGIN  → save(newRefreshToken)   → INSERT row
//    REFRESH → findByToken(token)     → SELECT validate
//    LOGOUT  → deleteByToken(token)   → DELETE row
//
// ⚠️ WHY @Transactional on deleteByToken?
//    DEFAULT JpaRepository methods (save, delete, findById):
//       Already @Transactional internally (auto wrapped)
//
//    DERIVED DELETE methods (deleteByToken, deleteByEmail):
//       NOT auto-transactional!
//       Need explicit @Transactional
//
//    Without @Transactional:
//       ❌ TransactionRequiredException at runtime
//       "No transaction in progress"
//
//    With @Transactional:
//       ✅ Spring wraps in transaction → DELETE works
//
//    Two places to add:
//       Option 1: Method level (this file — Repository)
//       Option 2: Caller side (Service/Controller)
//       Both work — choose based on architecture
//
// 🔑 SPRING DATA JPA MAGIC RECAP:
//    Method NAME → Auto SQL
//
//    findByToken                      → SELECT WHERE token = ?
//    findByUserIdAndExpiresAtAfter    → SELECT WHERE user_id=? AND expires_at > ?
//    deleteByToken                    → DELETE WHERE token = ?
//    deleteByExpiresAtBefore          → DELETE WHERE expires_at < ?  (cleanup)
//    countByUserId                    → SELECT COUNT WHERE user_id = ?
//    existsByToken                    → SELECT 1 WHERE token = ?
//
// PRODUCTION CLEANUP METHOD (Bonus):
//    // Could add later for scheduled cleanup
//    @Transactional
//    void deleteByExpiresAtBefore(Instant cutoff);
//
//    @Scheduled(cron = "0 0 2 * * *")  // 2 AM daily
//    public void cleanupExpired() {
//        refreshTokenRepo.deleteByExpiresAtBefore(Instant.now());
//    }
//
// 🎨 PATTERN: Repository (Spring Data JPA — proxy auto-implementation)
//
// 📐 SOLID:
//    ISP — Focused interface (only RefreshToken operations)
//    DIP — Service depends on interface, proxy auto-implemented
//    SRP — Sirf refresh token DB access, koi business logic nahi
//
// 🎤 INTERVIEW LINE:
//    "RefreshTokenRepository follows Spring Data JPA pattern.
//     Custom derived methods (findByToken, deleteByToken) generate
//     SQL from method names.
//     KEY GOTCHA: derived DELETE needs @Transactional explicitly
//     (unlike inherited JpaRepository.delete() which is auto-transactional)."
// ═══════════════════════════════════════════════════════════════════════

@Repository
public interface RefreshTokenRepository extends JpaRepository<RefreshToken, Long> {

    // 🔍 Find token by string value
    // Used by: AuthController.refresh() + logout()
    // Returns: Optional — present if token exists, empty if not
    Optional<RefreshToken> findByToken(String token);

    // 🗑️ Delete by token string (used in logout)
    // @Transactional mandatory for derived delete methods
    // Returns void — silently completes (or no-op if not found)
    @Transactional
    void deleteByToken(String token);
}
